#Created by Shu, 20240314---

Purposes of different scripts:

backup_slice_fit.cxx: 
The old version of slice_fit.cxx

command.sh: 
Extract information from initial root data. Draw performance of single OpCh;
Inlcude (total #photon) vs E_mu, (#photon / event) vs distance;

hist_combine.cxx:
usually combine root file from different command.sh

energyD_combine.cxx
combine energy related distributions of two different Combine_rslxx_1000numx_e67_hist.root, the product of ./command.sh

slice_fit.sh:
For certain slice, do fitting; cathode

rsl_compare.cxx:
Compare fitting results of different RSLs; cathode

slice_fit_m1/m2/p1/p2.cxx
For certain slice, do fitting; cathode, (membrane XAs close)/(membrane XAs away from)/(pmts close)/(pmts away from) to track

rsl_compare_mmpp.cxx:
Compare fitting results of different RSLs; (membrane XAs close)/(membrane XAs away from)/(pmts close)/(pmts away from) close to track





