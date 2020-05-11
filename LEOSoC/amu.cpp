/*
*  Cpp Implementation: LEOSoC
*
* Description:
* This module implements the address management unit
*
* Rev. 0.1
*
* Author: Andrea Guerrieri <andrea.guerrieri@epfl.ch (C) 2017
*
* Copyright: See COPYING file that comes with this distribution
*
*/

/**
 * @file amu.cpp
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


static const unsigned int fpga_address_base[PLATFORM_MAX]= {
    AXI_GP0_BASE_XILINX,
    AXI_GP0_BASE_ALTERA,
};

uint32_t amu_get_address ( uint32_t address )
{

  return ( address += fpga_address_base[run_env] );    
}
