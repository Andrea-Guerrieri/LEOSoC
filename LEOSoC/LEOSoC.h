#ifndef _LEOSOC_H
#define _LEOSOC_H

#include "pl.h"


#define SYS_ERR		-1



#define PAGE_SIZE	1
#define PAGE_MASK ((uint64_t)(long)~(PAGE_SIZE - 1))



enum
{
  FALSE,
  TRUE
};






int leosoc_init ( int id ); 
int leosoc_create ( int id , void *input_data, void *output_data );
int leosoc_join ( int id, int timeout );
uint32_t leosoc_write_reg ( uint32_t address );
uint32_t leosoc_read_reg ( uint32_t address );
int leosoc_cancel ( int id );
int leosoc_end ( void ); 

#endif
