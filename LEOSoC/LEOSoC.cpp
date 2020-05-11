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


#define DEBUG_MODULE(_X)	//_X

using namespace std;

unsigned char app_run;


PL_T pl;

pthread_t interrupt;
pthread_t timer[4];

void* interrupt_thread( void *i )
{
    
    uint32_t interrupts_pendings;
    unsigned char indx;


  while ( app_run )
    {
      usleep ( 1 );
      //printf ( "Checking interrupts \n\r" );

      interrupts_pendings = leosoc_read_reg ( AIC_INTERRUPTS_REG_0 );

      //interrupts_pendings = 0x01; //
      
      if ( interrupts_pendings )
        {
          for ( indx = 0; indx < 4; indx++ )
            {
              if ( interrupts_pendings & ( 1 << ( indx - 1 ) ) )
                {
                    pl.aaw[indx].pr.status = STATUS_ENDED;
                    printf ( "thread %d terminate \n\r", indx );
                    interrupts_pendings = 0x00; //

                }
            }
        }

    }
    
    
}

void* timer_thread( void *id )
{

    uint32_t indx; 
    
    while ( app_run )
    {
        for ( indx = 0; indx < PR_REGIONS; indx++ )
        {
            if ( pl.aaw[indx].pr.status == STATUS_PROGRAMMED )
            {
                pl.aaw[indx].pr.timeout--;
                if ( ( pl.aaw[indx].pr.timeout ) )
                {
                    if ( pl.aaw[indx].pr.status == STATUS_ENDED )
                    {
                        printf ( "thread %d counting \n\r", indx );
                        //break;
                    }
                }
                else
                {
                    pl.aaw[indx].pr.status = STATUS_ENDED;
                    printf ( "thread %d timeout exit\n\r", indx );
                }

            }
            
        }
        usleep ( 1 );
    }    

    
}

uint32_t leosoc_write_reg ( uint32_t address, uint32_t value )
{
  return write_reg ( address, value );
}


uint32_t leosoc_read_reg ( uint32_t address )
{
  return read_reg ( address );
}

int leosoc_init ( int id )
{

    printf ( "LEOSoC start\n\r" );

    for ( int indx = 0; indx < PR_REGIONS; indx++ )
    {
            pl.aaw[indx].pr.timeout = 5;
    }

    if ( pmu_detect_platform () )
    {
        fpi_program_full ( id );
        pthread_create( &interrupt , NULL, interrupt_thread, ( void * ) NULL );
        pthread_create( &timer[id] , NULL, timer_thread, ( void * ) id );


        app_run = TRUE;
    }
    else
    {
        app_run = FALSE;
        leosoc_end ();
    }
}


void *input_data_p, *output_data_p;

int leosoc_create ( int id, void *input_data, void *output_data )
{

  unsigned int indx;
  input_data_p = input_data;	//copy the address into a local pointer
  output_data_p = output_data;


  for ( indx = 0; indx < PR_REGIONS; indx++ )
    {
      if ( pl.aaw[indx].pr.status != STATUS_RUNNING && pl.aaw[indx].pr.status != STATUS_PROGRAMMED )
        {

          fpi_program_partial ( id, indx );

          pl.aaw[indx].pr.id = id;
          pl.aaw[indx].pr.module = id;
          pl.aaw[indx].pr.status = STATUS_PROGRAMMED;


          //User-Code
          //setup_start_accelerator (  ); //

          //pl.aaw[indx].pr.status = STATUS_RUNNING; //Test purpose

          break;
        }
    }

  printf ( "hw thread id %d created \n\r", id );


}



int leosoc_join ( int id, int timeout )
{
    printf ( "waiting for hw thread id %d \n\r", id );

  
    pl.aaw[id].pr.timeout = timeout*100000;

    while ( pl.aaw[id].pr.status != STATUS_ENDED )
    {
        usleep (1);
    }

  return 0;
}


int leosoc_cancel ( int id )
{
  uint8_t indx;

  for ( indx = 0; indx < PR_REGIONS; indx++ )
    {
      if ( pl.aaw[indx].pr.id == id )
        {

          pl.aaw[indx].pr.status = STATUS_ENDED;

          break;
        }
    }

}

int leosoc_end ( void )
{
  printf ( "LEOSoC end\n\r" );
  exit ( 0 );
}
