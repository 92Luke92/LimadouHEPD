#!/bin/bash
# ========================================================================================
#
# Created by Alessandro Sotgiu @ INFN Roma2: alessandro.sotgiu@roma2.infn.it
#
# ========================================================================================

EXT="merged.root"

if [ "$#" -eq 1 ]; then
    if [ $1 = "-h" ]; then
        echo " __________________________________________________________________________"  
        echo "|                                                                          |"
        echo '| export PATH=/your/path/LimadouHEPD/QuickLook/bin:$PATH   (required)      |'
        echo "|                                                                          |" 
        echo "| usage:  HEPDquickLook  [level0 root file dir]  [output dir] (abs paths)  |"
        echo "|                                                                          |"
        echo "|__________________________________________________________________________|"
        echo ""                                       
        exit -1
    else
        echo ""
        echo " ERROR: Illegal number of parameters"
        echo " Type HEPDquickLook -h for help"
        echo ""
        exit -1
    fi
fi

if [ "$#" -ne 2 ]; then
    echo ""
    echo " ERROR: Illegal number of parameters"
    echo " Type HEPDquickLook -h for help"
    echo ""
    exit -1
fi                                                     

BINDIR=`dirname $0`
SOURCEDIR=$1

cd $SOURCEDIR/..
echo "pws" $PWD
NAMEDIR=${PWD##*/}
cd -

echo ""
echo "HEPDquicklook.sh ###############  executable dir = " $BINDIR
echo "HEPDquicklook.sh ###############  source dir = " $SOURCEDIR

#NAME=$(ls $SOURCEDIR*.root | sort -V | head -1)
#NAME=${NAME##*/} # to get basename
#MERGEDFILE="${NAME%???????????}"
#MERGEDFILE=$MERGEDFILE$EXT

MERGEDFILE=$NAMEDIR$EXT
OUTPATH=$2
OUTDIR="/QL_outdir/"
DEST=$OUTPATH$OUTDIR   

if [ ! -d $DEST ]; then
    mkdir $DEST
fi
 
echo "HEPDquicklook.sh ###############  output dir = " $DEST
echo "HEPDquicklook.sh ###############  output file = " $MERGEDFILE
SOURCEDIR=$SOURCEDIR"/Data-2*Events*.root"

if [ -f $DEST$MERGEDFILE ]; then
   printf "HEPDquicklook.sh ###############  Merged file already created\n\n"
else
    hadd $DEST$MERGEDFILE $SOURCEDIR
fi



$BINDIR/QuickLook $DEST$MERGEDFILE -x "xslTemplates/" -o $DEST        
cp -r $BINDIR"/../xslTemplates" $DEST                                                       

#for i in $(ls -d HEPD001*); do HEPDquickLook.sh $i/L0_files $i ;  done
