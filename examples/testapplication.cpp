#include<iostream>
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

int input_data, output_data;


#define TEST_ID 1
#define TIMEOUT 5


int main ( int argc, char *argv[] )
{
        
    leosoc_init( TEST_ID );
    
    leosoc_create ( 0, &input_data, &output_data );
    leosoc_create ( 1, &input_data, &output_data );
    leosoc_create ( 2, &input_data, &output_data );
    
    leosoc_join ( 0, TIMEOUT );
    sleep ( 2 );
    leosoc_join ( 1, TIMEOUT );
    
}
