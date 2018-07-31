#!/bin/bash

###########################################################
# Submit sbncode LArSoft jobs to the grid.
#
# Note: Please check the OUTDIR path!
#
# Script arguments are passed to "lar"
#
# A. Mastbaum <mastbaum@uchicago.edu>, 2018/07/05
###########################################################

OUTDIR="/pnfs/sbnd/scratch/users/siqini/sbncode_out"
TARDIR="/pnfs/sbnd/persistent/users/mastbaum/tars"
SBNCODE_TAR="${TARDIR}/sbncode-v06_80_00-ub.tar.gz"

# Log file
LOG="${CLUSTER}_${PROCESS}.log"
echo "Running ${0} on ${HOSTNAME}" >>${LOG} 2>&1
echo "Cluster: ${CLUSTER}" >>${LOG} 2>&1
echo "Process: ${PROCESS}" >>${LOG} 2>&1
echo "sbncode: ${SBNCODE_TAR}" >>${LOG} 2>&1
echo "lar options: $@" >>${LOG} 2>&1
date >>${LOG} 2>&1

# Environment
source /cvmfs/icarus.opensciencegrid.org/products/icarus/setup_icarus.sh
setup icaruscode v06_80_00 -q e15:prof >>${LOG} 2>&1
unsetup cetpkgsupport
source /cvmfs/sbnd.opensciencegrid.org/products/sbnd/setup_sbnd.sh
setup sbndcode v06_80_00 -q e15:prof >>${LOG} 2>&1
unsetup mrb
unsetup git
unsetup gitflow
unsetup cetpkgsupport
source /cvmfs/uboone.opensciencegrid.org/products/setup_uboone.sh

# Get sbncode tarball, unpack, and configure
ifdh cp -D ${SBNCODE_TAR} . >>${LOG} 2>&1
tar xzvf $(basename ${SBNCODE_TAR}) >>${LOG} 2>&1

export MRB_PROJECT="larsoft"
export MRB_PROJECT_VERSION="06_80_00"
export MRB_QUALS="e15:prof"
export MRB_TOP="${PWD}"
export MRB_SOURCE="${MRB_TOP}"
export MRB_INSTALL="${MRB_TOP}"
export PRODUCTS="${MRB_INSTALL}:${PRODUCTS}"

mrbslp >>${LOG} 2>&1

# Run LArSoft job
lar "$@" >>${LOG} 2>&1
lar -n 100 -c prodgenie_bnb_nu_sbn_sbnd.fcl >>${LOG} 2>&1
lar -c standard_g4_sbn_sbnd.fcl >>${LOG} 2>&1


# Transfer output files to dCache
OUT="${CLUSTER}_${PROCESS}"
mkdir -p ${OUT} >>${LOG} 2>&1
mv *.root ${OUT}
mv ${LOG} ${OUT}

ifdh mkdir ${OUTDIR}/${OUT}
ifdh cp -r ${OUT} ${OUTDIR}/${OUT}/

#To run a simulation, replace the usual
#
#$ lar -c <config.fcl> -n <nevents> …
#$ lar -c standard_g4_sbn_sbnd.fcl prodgenie_bnb_nu_sbn_sbnd_*_gen.root

#with

#$ jobsub_submit --memory=2000MB --group=sbnd \
#--resource-provides=usage_model=OPPORTUNISTIC -N <NJOBS> \
#file:///PATH/TO/submit.sh -c <config.fcl> -n <nevents> …

#Note: so far this is the only command that works! Will fix job_submission.xml though.
#$ jobsub_submit --memory=2000MB --group=sbnd \ --resource-provides=usage_model=OPPORTUNISTIC -N 1 \file:///sbnd/app/users/siqini/sbnanalysis/submit2.sh 

#$ jobsub_submit --memory=2000MB --group=sbnd \
# --resource-provides=usage_model=OPPORTUNISTIC -N 1 \
#	file:///sbnd/app/users/siqini/sbnanalysis/submit.sh -c standard_g4_sbn_sbnd.fcl /pnfs/sbnd/scratch/users/siqini/sbncode_out/0726_100GENIE/10409404_0/prodgenie_bnb_nu_sbn_sbnd_20180726T234638_gen.root
