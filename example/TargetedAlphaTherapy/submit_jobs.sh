#!/bin/env bash

##############################################################
# This script run sequentially the N plitted files with Geant4. Execute with:
# ./submit_jobs.sh

export name_Distrib=Nucleus

mkdir -p log/log$name_Distrib
list_run=`ls split/split$name_Distrib/*`
n=0;

for i in $list_run
do
  echo $i
  ./targetedAlphaTherapy -m $i -t 1 > log/log$name_Distrib/log$n.out 2>&1 && echo OK
  n=`expr "$n" + 1`;
done
