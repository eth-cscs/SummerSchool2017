#!/bin/bash
module load datawarp

gcc -c `pkg-config cray-datawarp --cflags` datawarp_stager.c
gcc `pkg-config cray-datawarp --libs` -o datawarp_stager datawarp_stager.o

rm datawarp_stager.o

