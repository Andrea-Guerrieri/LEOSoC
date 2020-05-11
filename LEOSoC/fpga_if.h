#ifndef _FPGA_H
#define _FPGA_H

#define PAGE_SIZE	1
#define PAGE_MASK ((uint64_t)(long)~(PAGE_SIZE - 1))


#ifdef SOC
#define PROGRAM_FPGA(_cmd)	system (_cmd)
#define CHECK_FPGA(_cmd)	system (_cmd)
#define SET_PR(_cmd)        system (_cmd)
#else
#define PROGRAM_FPGA(_cmd)	printf ("")
#define CHECK_FPGA(_cmd)	printf ("")
#define SET_PR(_cmd)        printf ("")
#endif


#define PR_SET	"echo 1 > /sys/devices/soc0/amba/f8007000.devcfg/is_partial_bitstream"
#define FULL_SET "echo 0 > /sys/devices/soc0/amba/f8007000.devcfg/is_partial_bitstream"

#define XILINX_DEV  "ls /dev/xdevcfg 2> /dev/null "
#define ALTERA_DEV  "ls /dev/fpga0 2> /dev/null "

#define XILINX_CMD  "cat leosoc_pl_4_0_0 > /dev/xdevcfg"
#define ALTERA_CMD  "echo 0 > /sys/class/fpga-bridge/hps2fpga/enable \
                     echo 0 > /sys/class/fpga-bridge/fpga2hps/enable \
                     echo 0 > /sys/class/fpga-bridge/lwhps2fpga/enable \
                     dd if=leosoc_pl_4_0_0.rbf of=/dev/fpga0 bs=1M || exit 1 \
                     echo 1 > /sys/class/fpga-bridge/hps2fpga/enable \
                     echo 1 > /sys/class/fpga-bridge/fpga2hps/enable \
                     echo 1 > /sys/class/fpga-bridge/lwhps2fpga/enable \ 
                     "

#define CMD	"cat"

#define PL_INIT_ID	0x00

#define SWIFT_PL_1	"leosoc_pl_1"
#define SWIFT_PL_2	"leosoc_pl_2"
#define SWIFT_PL_4	"leosoc_pl_4"
#define SWIFT_PL_8	"leosoc_pl_8"

#define PR_REGIONS  8


uint32_t fpi_program_full ( unsigned char type );
uint32_t fpi_program_partial ( unsigned char module,  unsigned char region );
uint32_t read_reg ( uint32_t address );
uint32_t write_reg ( uint32_t address, uint32_t value );



#endif
