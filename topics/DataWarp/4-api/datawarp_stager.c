#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <datawarp.h>

void usage(char *cmd) {
	fprintf(stderr, "Error: Expected usage:\n"
		"%s [in | out ] [target] [src]            #stage file\n"
		"%s [dirin | dirout ] [target] [src]      #stage directory\n"
		"%s [defout | defdirout ] [target] [src]  #deferred stageout\n"
		"%s [actout | actdirout ] [target]        #activate deferred\n"
		"%s [revout | revdirout ] [target]        #revoke deferred\n"
		"%s [listin ]             [target] [src]* #stage list in\n"
		"%s [listout | deflistout] [target]* [src]#stage list out\n"
		"%s [actlistout | revlistout ] [target]*  #revoke list out\n"
		"%s [qfile | qdir ] [target]              #query\n"
		"%s [wfile | wdir ] [target]              #wait\n"
		"%s [tfile | tdir ] [target]              #terminate\n",
		cmd, cmd, cmd, cmd, cmd, cmd, cmd, cmd, cmd, cmd, cmd);
}

/*
 * prepare_liststage_list(argv, start, num)
 *
 * Prepare a char **list for listin.
 *
 * capture argv[start]..argv[start+num-1] into list, and list[num]=NULL
 *
 * commandline is e.g.:
 * stager listin     dvs_dir   pfs_file*  --  start = 3, num = argc-3
 * stager listout    dvs_file* pfs_dir    --  start = 2, num = argc-3
 * stager actlistout dvs_file*            --  start = 2, num = argc-2
 * deflistout works the same as listout, revlistout the same as actlistout.
 *
 * On success, returns list, which is caller's responsibility to free().
 * On error, returns NULL with errno set appropriately.
 */
static const char **prepare_liststage_list(char **argv, int start, int num)
{
	const char **list;
	int i;

	list = malloc((num+1) * sizeof(const char*));
	if (!list)
		return NULL;

	for (i = 0; i < num; i++)
		list[i] = argv[start+i];
	list[num] = NULL;

	return list;
}


int main(int argc, char **argv)
{
	int ret;
	static int ci, pi, di, fi;

	if (argc < 2) {
		usage(argv[0]);
		ret = -EINVAL;
		goto out;
	}	

	if (strcmp(argv[1], "in") == 0 && argc == 4) {
		//do stage in
		ret = dw_stage_file_in(argv[2], argv[3]);
	} else if (strcmp(argv[1], "out") == 0 && argc == 4) {
		//do stage out
		ret = dw_stage_file_out(argv[2], argv[3], DW_STAGE_IMMEDIATE);
	} else if (strcmp(argv[1], "defout") == 0 && argc == 4) {
		//do stage out, deferred to job end
		ret = dw_stage_file_out(argv[2], argv[3],
					DW_STAGE_AT_JOB_END);
	} else if (strcmp(argv[1], "dirin") == 0  && argc == 4) {
		ret = dw_stage_directory_in(argv[2], argv[3]);
	} else if (strcmp(argv[1], "dirout") == 0  && argc == 4) {
		ret = dw_stage_directory_out(argv[2], argv[3], DW_STAGE_IMMEDIATE);
	} else if (strcmp(argv[1], "defdirout") == 0 && argc == 4) {
		ret = dw_stage_directory_out(argv[2], argv[3],
					     DW_STAGE_AT_JOB_END);
	} else if (strcmp(argv[1], "actout") == 0 && argc == 3) {
		ret = dw_stage_file_out(argv[2], NULL,
					DW_ACTIVATE_DEFERRED_STAGE);
	} else if (strcmp(argv[1], "actdirout") == 0 && argc == 3) {
		ret = dw_stage_directory_out(argv[2], NULL,
					     DW_ACTIVATE_DEFERRED_STAGE);
	} else if (strcmp(argv[1], "revout") == 0 && argc == 3) {
		ret = dw_stage_file_out(argv[2], NULL,
					DW_REVOKE_STAGE_AT_JOB_END);
	} else if (strcmp(argv[1], "revdirout") == 0 && argc == 3) {
		ret = dw_stage_directory_out(argv[2], NULL,
					     DW_REVOKE_STAGE_AT_JOB_END);
	} else if (strcmp(argv[1], "listin") == 0 && argc >= 4) {
		const char **list = prepare_liststage_list(argv, 3, argc - 3);
		if (!list)
			return -errno;
		ret = dw_stage_list_in(argv[2], list);
		free(list);
	} else if (strcmp(argv[1], "listout") == 0 && argc >= 4) {
		const char **list = prepare_liststage_list(argv, 2, argc - 3);
		if (!list)
			return -errno;
		ret = dw_stage_list_out(list, argv[argc-1], DW_STAGE_IMMEDIATE);
		free(list);
	} else if (strcmp(argv[1], "deflistout") == 0 && argc >= 4) {
		const char **list = prepare_liststage_list(argv, 2, argc - 3);
		if (!list)
			return -errno;
		ret = dw_stage_list_out(list, argv[argc-1], DW_STAGE_AT_JOB_END);
		free(list);
	} else if (strcmp(argv[1], "actlistout") == 0 && argc >= 3) {
		const char **list = prepare_liststage_list(argv, 2, argc - 2);
		if (!list)
			return -errno;
		ret = dw_stage_list_out(list, argv[argc-1], DW_ACTIVATE_DEFERRED_STAGE);
		free(list);
	} else if (strcmp(argv[1], "revlistout") == 0 && argc >= 3) {
		const char **list = prepare_liststage_list(argv, 2, argc - 2);
		if (!list)
			return -errno;
		ret = dw_stage_list_out(list, argv[argc-1], DW_REVOKE_STAGE_AT_JOB_END);
		free(list);
	} else if (strcmp(argv[1], "qfile") == 0  && argc == 3) {
		ret = dw_query_file_stage(argv[2], &ci, &pi, &di, &fi);
		printf("query results: complete=%d pending=%d deferred=%d failed=%d\n", ci, pi, di, fi);
	} else if (strcmp(argv[1], "qdir") == 0  && argc == 3) {
		ret = dw_query_directory_stage(argv[2], &ci, &pi, &di, &fi);
		printf("query results: complete=%d pending=%d deferred=%d failed=%d ",
		       ci, pi, di, fi);
	} else if (strcmp(argv[1], "wfile") == 0  && argc == 3) {
		ret = dw_wait_file_stage(argv[2]);
	} else if (strcmp(argv[1], "wdir") == 0  && argc == 3) {
		ret = dw_wait_directory_stage(argv[2]);
	} else if (strcmp(argv[1], "tfile") == 0  && argc == 3) {
		ret = dw_terminate_file_stage(argv[2]);
	} else if (strcmp(argv[1], "tdir") == 0  && argc == 3) {
		ret = dw_terminate_directory_stage(argv[2]);
	} else {
		// invalid option
		usage(argv[0]);
		ret = -EINVAL;
		goto out;
	}

	if (ret < 0) {
		printf("%s:  command failure! - %d: %s\n", argv[0], ret,
		       strerror(-ret));
		goto out;
	}

	printf("%s:  command success!\n", argv[0]);


out:
	return ret;
}
