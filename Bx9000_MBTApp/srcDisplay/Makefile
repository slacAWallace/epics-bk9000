#  -*- Makefile -*-
T_A = $EPICS_HOST_ARCH

TOP = ../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE


# EDM Screens to install
EDLS += Bx9000Info.edl
EDLS += Bx9000InstalledBTs.edl
EDLS += Bx9000Info-lcls.edl 
EDLS += KL3202-RTD-lcls.edl
EDLS += KL3314-TC-lcls.edl
EDLS += KL3162_sum.edl
EDLS += KL3162.edl

# Install images to be used by EDM (Gif and Png are supported)
#GIFS += Myfirst.gif
#PNGS += Myfirst.png

# EDM Screens to install
# # If you want to install all screens and are
# # lazy use the wildcard method. :)
## EDLS+=$(wildcard *.edl)
# #PNGS+=$(wildcard *.png)
# #GIFS+=$(wildcard *.gif)


include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE


