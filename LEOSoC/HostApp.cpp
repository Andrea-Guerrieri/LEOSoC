#include <iostream>

#include <pthread.h>
//#include <fcntl.h>
//#include <errno.h>
//#include <termios.h>
#include <sys/resource.h>
#include <sys/prctl.h>
#include <sys/signal.h>

//#include <arpa/inet.h>
//#include <netinet/in.h>
//#include <sys/socket.h>
//#include <netdb.h>
//#include <time.h>

//#define _GNU_SOURCE              // to get macros like CPU_ZERO
#include <sched.h>              // to set affinity

#include "swift.h"


static void term_handler ( int sig );
static void sig_handler_init ( void );

static void application_init ( int argc, char *argv[] );
static void application_end ( void );
void application ( void );
void arguments_parser ( int argc, char *argv[] );

void core_dump_en ( void );


enum
{
  FALSE,
  TRUE
};



// // cpu_num argument below should be 0 or 1 for Zynq
// // This is for processor affinity
// void set_cpu(int cpu_num) 
// {
// 	cpu_set_t mask;          // for setting CPU affinity
// 
// 	// Always keeping logging/monitoring process in one CPU
// 	CPU_ZERO(&mask);
// 	CPU_SET(cpu_num, &mask);
// 	sched_setaffinity(0, sizeof(mask), &mask);       // setting current process
// }

void application_init ( int argc, char *argv[] )
{
  sig_handler_init();
  
  core_dump_en();
        
//  set_cpu( 0 );
  swift_init ( argc, argv );

}

void application_end ()
{
  swift_end ( );
  //printf ("swift exit\n\r");
}


void core_dump_en ( void )
{
    int ret;
    struct rlimit rlim;

    /* Get the Dumpable state */
    ret = prctl( PR_GET_DUMPABLE, 0, 0, 0, 0 );
    //printf( "prctl(): PR_GET_DUMPABLE returned %d\n", ret );
    
    /* Set the Dumpable state to be enabled */
    ret = prctl( PR_SET_DUMPABLE, 1, 0, 0, 0 );
    //printf( "prctl(): PR_SET_DUMPABLE returned %d\n", ret );
    
    /* Get the Dumpable state again, make sure it was set */
    ret = prctl( PR_GET_DUMPABLE, 0, 0, 0, 0 );
    //printf( "prctl(): PR_GET_DUMPABLE returned %d\n", ret );	/* Get the core dump limitation */

    ret = getrlimit ( RLIMIT_CORE, &rlim );
    //printf ( "getrlimit(): RLIMIT_CORE returned %d (%d, %d)\n", ret, rlim.rlim_cur, rlim.rlim_max );

    /* Set the core dump limitation to be unlimited */
    rlim.rlim_cur = RLIM_INFINITY;
    rlim.rlim_max = RLIM_INFINITY;
    ret = setrlimit ( RLIMIT_CORE, &rlim );
    //printf ( "setrlimit(): RLIMIT_CORE returned %d\n", ret );

    /* Get the core dump limitation again */
    ret = getrlimit ( RLIMIT_CORE, &rlim );
    //printf ( "getrlimit(): RLIMIT_CORE returned %d (%d, %d)\n", ret, rlim.rlim_cur, rlim.rlim_max );

}

int main( int argc, char *argv[] ) 
{
  
    application_init ( argc, argv );
       
    application_end ();
      
    //exit ( 0 );
}


void sig_handler_init ( void )
{
	struct sigaction sig_term;

	// install the term signal handler
	sig_term.sa_handler = term_handler;
	sig_term.sa_flags = SA_RESTART;
	sig_term.sa_restorer = NULL;
	sigaction ( SIGTERM, &sig_term, NULL );
	sigaction ( SIGQUIT, &sig_term, NULL );
	sigaction ( SIGINT, &sig_term, NULL );
}

void term_handler ( int sig )
{
	switch ( sig )
	{
		case SIGTERM:
		case SIGQUIT:
		case SIGINT:
			app_run = FALSE;
			break;
	}
}
