#!/bin/sh
#Author Andrea Guerrieri andrea.guerrieri@epfl.ch

make clean
make CROSS_COMPILE=~/software/ARM/compiler/gcc-linaro-5.4.1-2017.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
mv bin/test bin/test_de0 

make clean
make CROSS_COMPILE=~/software/ARM/compiler/Sourcery_CodeBench_Lite_for_Xilinx_GNU_Linux/bin/arm-xilinx-linux-gnueabi-
mv bin/test bin/test_ZC706
