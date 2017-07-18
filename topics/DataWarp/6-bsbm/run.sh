
script_dw=$SCRATCH/script_dw.sh
script_cdw=$SCRATCH/script_cdw.sh

cat submit-common.sh | sed 's/^##DW/#DW/' > $script_dw
cat $script_dw | sed 's/=striped/=striped(client_cache=yes)/' > $script_cdw
cp submit-common.sh $SCRATCH

if [ ! -d $SCRATCH/sw ]
then
    mkdir $SCRATCH/sw
    cp -r /users/mvalle/prj/apache-jena-3.2.0 $SCRATCH/sw
    cp -r /users/mvalle/prj/apache-jena-fuseki-2.5.0 $SCRATCH/sw
    cp -r /users/mvalle/dw/summer-school/6-bsbm/bsbmtools-0.2 $SCRATCH/sw
fi

cd $SCRATCH

sbatch -C mc --job-name=BSBM-scratch-map --output=mp-bsbm.out  submit-common.sh mapped
sbatch -C mc --job-name=BSBM-scratch-dir --output=sc-bsbm.out  submit-common.sh scratch
sbatch -C mc --job-name=BSBM-DataWarp    --output=dw-bsbm.out  $script_dw dw
sbatch -C mc --job-name=BSBM-CachedDW    --output=cdw-bsbm.out $script_cdw dw
