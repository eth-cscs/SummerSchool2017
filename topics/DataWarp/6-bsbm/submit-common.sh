#!/bin/bash
#SBATCH --job-name="DW-bsbm"
#SBATCH --nodes=1
#SBATCH --threads-per-core=1
#SBATCH --cpus-per-task=12
#SBATCH --time=1-00:00:00
#SBATCH --output=common-bsbm.out
##DW jobdw access_mode=striped capacity=400GiB type=scratch
#-----------------------------------------------------------

loader=$SCRATCH/sw/apache-jena-3.2.0/bin/tdbloader
loader2=$SCRATCH/sw/apache-jena-3.2.0/bin/tdbloader2
fuseki=$SCRATCH/sw/apache-jena-fuseki-2.5.0

case $1 in
scratch)
	echo "====== In SCRATCH direct"
	ROOT=$SCRATCH/bsbm-dir
	rm -rf ${ROOT}
	mkdir ${ROOT}
	direct="--set tdb:fileMode=direct"
	;;
dw)
	echo "====== In DataWarp"
	ROOT=$DW_JOB_STRIPED
	direct="--set tdb:fileMode=direct"
	;;
mapped)
	echo "====== In SCRATCH mapped"
	ROOT=$SCRATCH/bsbm-map
	rm -rf ${ROOT}
	mkdir ${ROOT}
	loader=$loader2
	direct=""
	;;
*)
	echo Invalid mode $1
	;;
esac

#-----------------------------------------------------------
export TMPDIR=$ROOT/tmp
export TMP=$ROOT/tmp
mkdir ${TMP}

num=142061
#num=142413
#num=284826
#num=2878260

#-----------------------------------------------------------
echo
echo "Generate data..."

cp -r $SCRATCH/sw/bsbmtools-0.2 $ROOT
mkdir $ROOT/qry 
cd $ROOT/bsbmtools-0.2 
time ./generate -dir $ROOT/qry -s ttl -pc $num -fc -fn bmark -nof 1

mkdir $ROOT/data 
mv bmark*.ttl $ROOT/data

#-----------------------------------------------------------
echo
echo "Load database..."

DB=${ROOT}/tdb
mkdir ${DB}
cd $ROOT
time ${loader} --loc=${DB} $direct ${ROOT}/data/*.ttl
if [ $? -gt 0 ]
then
	echo
	du -ch ${DB}
	exit
fi

echo
du -ch ${DB}

#-----------------------------------------------------------
echo
echo "Start SPARQL server..."

cd $fuseki
./fuseki-server --loc=$DB $direct --port 3030 -q /db >/dev/null &

sleep 120

#-----------------------------------------------------------
echo
echo "Run queries..."

cd $ROOT/bsbmtools-0.2 
for n in 1 4 8 12
do
  echo
  echo "%%%%%%%%%%% Nodes: $n"
  time sh testdriver -idir ${ROOT}/qry -o /dev/null -mt $n http://localhost:3030/db
done
