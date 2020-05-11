/*
*  Cpp Implementation: LEOSoC
*
* Description:
* This module implements the fpga interface
*
* Rev. 0.1
*
* Author: Andrea Guerrieri <andrea.guerrieri@epfl.ch (C) 2017
*
* Copyright: See COPYING file that comes with this distribution
*
*/

/**
 * @file fpga_if.cpp
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
#include "amu.h"
#include "fpga_if.h"


#define DEBUG_MODULE(_X) 	//_X

static const char *fpga_prog_dev[PLATFORM_MAX]= {
    XILINX_DEV,
    ALTERA_DEV,
};

static const char *fpga_program_cmd[PLATFORM_MAX]= {
    XILINX_CMD,
    ALTERA_CMD,
};


long long current_timestamp ( void )
{
    struct timeval te;
    gettimeofday ( &te, NULL ); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    DEBUG_MODULE ( printf ( "milliseconds: %lld\n", milliseconds ) );
    return milliseconds;
}


uint32_t write_reg ( uint32_t address, uint32_t value )
{
    
    address = amu_get_address( address );

    DEBUG_MODULE ( printf ( "address %X \n\r", address ));
#ifdef SOC
    uint32_t fd;
    volatile uint8_t *acc_p;
    uint32_t errno;

    fd = open ( "/dev/mem", O_RDWR|O_SYNC );
    if ( fd < 0 ) 
    {
        fprintf ( stderr, "open(/dev/mem) failed (%d)\n", errno );
        return 1;
    }

    acc_p = mmap ( NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, address );

    * ( volatile uint32_t * ) ( acc_p ) = 0 ;
    * ( volatile uint32_t * ) ( acc_p ) = 1 ;
    * ( volatile uint32_t * ) ( acc_p ) = 0 ;
    * ( volatile uint32_t * ) ( acc_p ) = 1 ;

    exit ( 0 );

    if ( acc_p == MAP_FAILED ) 
    {
        fprintf ( stderr, "mmap64(0x%x@0x%x) failed (%d)\n", PAGE_SIZE, ( uint32_t ) ( address ), errno );
        return 1;
    }

    * ( volatile uint32_t * ) ( acc_p ) = value ;
#endif
}


uint32_t read_reg ( uint32_t address )
{
    #ifdef SOC

    address = amu_get_address( address );

    DEBUG_MODULE (printf ( "address %X \n\r", address ));
    uint32_t fd;
    volatile uint8_t *reg_p;
    uint32_t errno;

    fd = open ( "/dev/mem", O_RDWR|O_SYNC );
    if ( fd < 0 ) {
        fprintf ( stderr, "open(/dev/mem) failed (%d)\n", errno );
        return 1;
    }

    reg_p = mmap ( NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, address );
    if ( reg_p == MAP_FAILED ) {
        fprintf ( stderr, "mmap64(0x%x@0x%x) failed (%d)\n", PAGE_SIZE, ( uint32_t ) ( address ), errno );
        return 1;
    }

    return ( * ( volatile uint32_t * ) ( reg_p ) );
#endif
}

uint32_t reset_tlb ( unsigned char region )
{
    write_reg ( AAW0C_BASE + ( 0x10000 * region ), RESET_TLB );
    DEBUG_MODULE ( printf ( "[DEBUG_MODULE] reset_tlb \n\r" ) );
}

uint32_t reset_pr ( unsigned char region )
{
    DEBUG_MODULE (printf ( "[DEBUG_MODULE] reset_pr \n\r" ));
    write_reg ( AAW0C_BASE + ( 0x10000 * region ), RESET_PR );
}

uint32_t set_isolator ( unsigned char region )
{
    DEBUG_MODULE (printf ("[DEBUG_MODULE] set_isolator %x \n\r", AAW0C_BASE + ( 0x10000 * region ) ));
    write_reg ( AAW0C_BASE + ( 0x10000 * region ), ISOLATE_VALUE );
}

uint32_t reset_isolator ( unsigned char region )
{
    DEBUG_MODULE ( printf ( "[DEBUG_MODULE] reset_isolator %x \n\r ", AAW0C_BASE + ( 0x10000 * region ) ));
    write_reg ( AAW0C_BASE + ( 0x10000 * region ), DEISOLATE_VALUE );
}


uint32_t fpi_program_full ( unsigned char type )
{
    long long start_time, end_time;
    uint32_t elapsed_time;
    start_time = current_timestamp();

    DEBUG_MODULE ( printf ( "%s\n\r", fpga_program_cmd[run_env] ) );
    if ( ! ( PROGRAM_FPGA ( fpga_program_cmd[run_env] ) ) ) 
    {
        DEBUG_MODULE ( printf ( " [DEBUG_MODULE] DFR ok \n\r" ) );
    } 
    else 
    {
        DEBUG_MODULE ( printf ( "[DEBUG_MODULE] DFR KO \n\r" ) );
    }

    end_time = current_timestamp();
    elapsed_time = ( uint32_t ) ( end_time - start_time ) ;
    DEBUG_MODULE ( printf ( "prog.time[ms] %d \n\r", elapsed_time ));

}


uint32_t fpi_program_partial ( unsigned char module,  unsigned char region )
{
    char cmd[64];

    set_isolator ( region );

    //SET_PR ( PR_SET );

    //sprintf ( cmd, "%s %s_%d_%d > %s", fpga_program_cmd[run_env] , pl.bit.full_bt_name, region, module );

    while ( PROGRAM_FPGA ( cmd ) )
    {
        usleep ( 1000 );
    }


    reset_isolator ( region );
    reset_tlb ( region );
    reset_pr ( region );

}
