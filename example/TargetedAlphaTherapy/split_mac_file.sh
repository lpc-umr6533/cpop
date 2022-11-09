#!/bin/env bash

##############################################################
# This script split "run.mac" (example for 10 jobs). Execute with:
# ./split_mac_file.sh

N=20;
export name_Distrib=Nucleus
#Membrane, Cytoplasm, Homogeneous or Nucleus

N=`expr "$N" - 1`;

export split_folder_name=split$name_Distrib
export output_folder_name=output$name_Distrib

mkdir -p split/$split_folder_name
mkdir -p output/$output_folder_name

for i in $(seq 0 $N)
do
  cp data/runDistrib.mac split/$split_folder_name/run_Distrib$i.mac
done

for i in $(seq 0 $N)
do
  sed -i -e "s#/analysis/setFileName output/output.root#/analysis/setFileName output/$output_folder_name/output_$i.root#" split/$split_folder_name/run_Distrib$i.mac
  sed -i -e "s#/random/setSeeds 123456 1#/random/setSeeds 123456 $i#" split/$split_folder_name/run_Distrib$i.mac
done
