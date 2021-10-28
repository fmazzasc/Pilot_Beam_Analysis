#!/usr/bin/env bash

set -x

MODULES="PIPE ITS"
BKGEVENTS=200
SIGEVENTS=2000
NWORKERS=$(grep 'cpu[0-9]' /proc/stat | wc -l)

export O2DPG_ROOT=$PWD

# generate background


# embed signal into background

o2-sim -j ${NWORKERS} -n ${SIGEVENTS} -g boxgen -m ${MODULES} --field +2 \
       --configKeyValues 'BoxGun.pdg=310;BoxGun.eta[0]=-0.9;BoxGun.eta[1]=0.9;BoxGun.prange[0]=0.5;BoxGun.prange[1]=5.' \
        \
       > logsgn 2>&1
o2-sim-digitizer-workflow --sims sgn -b
o2-trd-trap-sim -b
