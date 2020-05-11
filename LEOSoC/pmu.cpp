/*
*  Cpp Implementation: LEOSoC
*
* Description:
* This module implements the LEOSoC APIs
*
* Rev. 0.1
*
* Author: Andrea Guerrieri <andrea.guerrieri@epfl.ch (C) 2017
*
* Copyright: See COPYING file that comes with this distribution
*
*/

/**
 * @file LEOSoC.cpp
 * @author Andrea Guerrieri - Processor Architecture Laboratory <andrea.guerrieri@epfl.ch>
 * @date Set 2018
 * @version 0.1
 * @copyright 2018 See COPYING file that comes with this distribution
 * @brief This module implements the LEOSoC APIs
 **/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <stdint.h>

#include <cstdlib>
#include <pthread.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/signal.h>


#include "LEOSoC.h"
#include "pl.h"
#include "pmu.h"
#include "fpga_if.h"



#define DEBUG_MODULE(_X) 	//_X

unsigned char run_env;


uint8_t pmu_detect_platform ( )
{
    run_env = ENV_NONE;
    uint32_t indx;

    for ( indx = 0; indx < PLATFORM_MAX; indx++ ) 
    {
        if ( ! ( CHECK_FPGA ( fpga_prog_dev[indx] ) ) ) 
        {
            run_env = indx;
            break;
        }

    }

    switch ( run_env ) 
    {
        case ENV_XILINX:
            printf ( "Xilinx Running Environment recognized\n\r" );
            break;

        case ENV_ALTERA:
            printf ( "Altera Running Environment recognized\n\r" );
            break;

        default:
            printf ( "No FPGA Environment recognized\n\r" );
            return FALSE;

            break;
    }

    return TRUE;


}
