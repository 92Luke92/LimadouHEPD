#!/bin/bash
# ========================================================================================
#
# Created by Alessandro Sotgiu @ INFN Roma2: alessandro.sotgiu@roma2.infn.it
#
# ========================================================================================


EXT="merged.root"


#f [ "$#" -ne 2 ]; then
    if [ "$#" -ne 2 ]; then
    echo ""
    echo "Illegal number of parameters"
    echo ""
    echo " __________________________________________________________________________"
    echo "|                                                                          |"     
    echo "|  use: HEPDquickLook  <root files directory>  <out dir>                   |"
#    echo "|  use: HEPDquickLook  <input root file>  <path with xsl template>         |"
    echo "|__________________________________________________________________________|"
    echo ""
exit -1
fi

cd $1

NAME=$(ls *.root | sort -V | head -1)
#OUTDIR="${NAME%.root}"
MERGEDFILE="${NAME%???????????}"
#MERGEDFILE="${NAME%Channel*}"
MERGEDFILE=$MERGEDFILE$EXT
OUTDIR=$2

hadd $MERGEDFILE *.root 

#QuickLook $MERGEDFILE -x "/home/fool/LIMADOU/Data_Acquisition/bin2root/HEPD_Quicklook/Housekeeping_QL"

QuickLook $MERGEDFILE -x "xslTemplates/"

cp -r /home/fool/LIMADOU/recon_software/LimadouHEPD/QuickLook/xslTemplates $OUTDIR
#mkdir $OUTDIR
#mv *.pdf *.xml  $OUTDIR 
