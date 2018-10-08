
FILENAME=$1
NJOBS=$2
NEVENTS=$3
RANDOMVAR=$4

for i in `seq 1 $NJOBS`;
do

    MACFILE=$FILENAME"_"$i".mac"
    MACPATH="job/"$MACFILE
    cp base.mac $MACPATH
    RANDOM=$RANDOMVAR
    echo "/random/setSeeds $((1 + RANDOM % 1000000)) $((1 + RANDOM % 1000000))" >> $MACPATH
    echo "/hepd/producer/SetRootFile "$FILENAME"_"$i".root HEPD" >> $MACPATH
    echo "/run/beamOn "$NEVENTS >> $MACPATH
    echo "/hepd/producer/StoreRootFile" >> $MACPATH

    bsub -q limadou -e err$i/ -o log$i/ hepdsw $MACPATH
done
