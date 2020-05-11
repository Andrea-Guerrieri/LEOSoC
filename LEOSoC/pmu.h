#ifndef _PMU_H
#define _PMU_H


extern unsigned char run_env;


uint8_t pmu_detect_platform ( void );


enum
{
  ENV_XILINX,
  ENV_ALTERA,
  ENV_NONE
};


enum
{
  PLATFORM_0,
  PLATFORM_1,
  PLATFORM_MAX
};




#endif
