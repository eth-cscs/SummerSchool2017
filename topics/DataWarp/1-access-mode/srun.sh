#!/bin/bash

env | grep DW_

touch ${DW_JOB_STRIPED}/HelloWorld-s${SLURM_PROCID}
touch ${DW_JOB_PRIVATE}/HelloWorld-p${SLURM_PROCID}
ls -l ${DW_JOB_STRIPED}
ls -l ${DW_JOB_PRIVATE}
