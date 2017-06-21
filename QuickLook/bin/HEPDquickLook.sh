#!/bin/bash
# ========================================================================================
#
# Created by Alessandro Sotgiu @ INFN Roma2: alessandro.sotgiu@roma2.infn.it
#
# ========================================================================================

EXT="merged.root"

if [ "$#" -eq 1 ]; then
    if [ $1 = "-h" ]; then
        echo " _________________________________________________________________________"  
        echo "|                                                                         |"
        echo "|  export PATH=/your/path/LimadouHEPD/QuickLook/bin:$PATH   (required)    |"
        echo "|                                                                         |" 
        echo "|  use:  HEPDquickLook  <root files dir>  <output dir> (absolute paths)   |"
        echo "|                                                                         |"
        echo "|_________________________________________________________________________|"
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

SOURCEDIR=$1
cd $SOURCEDIR
echo "source dir = " $SOURCEDIR
NAME=$(ls *.root | sort -V | head -1)
#OUTDIR="${NAME%.root}"
MERGEDFILE="${NAME%???????????}"
#MERGEDFILE="${NAME%Channel*}"
MERGEDFILE=$MERGEDFILE$EXT
OUTPATH=$2
OUTDIR="/QL_outdir/"
DEST=$OUTPATH$OUTDIR   
cd -
mkdir $DEST 
BINDIR=`dirname $0`
echo "binary file dir = " $BINDIR
echo "output dir = " $DEST                
SOURCEDIR=$SOURCEDIR"/*.root"
#echo "source dir = " $SOURCEDIR
hadd $DEST$MERGEDFILE $SOURCEDIR

QuickLook $DEST$MERGEDFILE -x "xslTemplates/" #-o $DEST        
cp -r $BINDIR"/../xslTemplates" $DEST                                                       
