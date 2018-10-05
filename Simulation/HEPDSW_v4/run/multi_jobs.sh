#for instance, FILENAME = p_125MeV_3C
FILENAME=$1

for i in `seq 1 3`;
do
    MACFILE=$FILENAME"_"$i".mac"
    MACPATH="job/"$MACFILE
    cp base.mac $MACPATH
    echo "/random/setSeeds $((1 + RANDOM % 1000000)) $((1 + RANDOM % 1000000))" >> $MACPATH
    echo "/hepd/producer/SetRootFile "$FILENAME"_"$i".root HEPD" >> $MACPATH
    echo "/run/beamOn 1" >> $MACPATH
    echo "/hepd/producer/StoreRootFile" >> $MACPATH

    bsub -q limadou -e err$i/ -o log$i/ hepdsw $MACPATH
done
