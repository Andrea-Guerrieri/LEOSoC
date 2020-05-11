#ifndef _PL
#define _PL


/*
 * User Peripherals physical base addresses.
 */


#define	AXI_GP0_BASE_ALTERA		0xFF200000
#define	AXI_GP0_BASE_XILINX		0x40000000

#define AXI_GP0_BASE			AXI_GP0_BASE_ALTERA	

#define RESET_PL_REG			0xF8000240	
#define RESET_PL_ASSERT			0x1 		//Bi-stable signal, have to be de-asserted
#define RESET_PL_DEASSERT		0x0 		//Bi-stable signal, have to be de-asserted

//AXI Interrupt Controller
#define AIC_OFFS			0x00
#define AIC_EN_REG_0_OFFS		0x00
#define AIC_EN_REG_1_OFFS		0x04
#define AIC_INTERRUPTS_REG_0_OFFS	0x08
#define AIC_INTERRUPTS_REG_1_OFFS	0x0C
#define AIC_REQ_OFFS			0x10
#define AIC_REQ_TLB_OFFS		0x10
#define AIC_REQ_PR_OFFS			0x10
#define AIC_REQ_PR_CODE_OFFS		0x10


#define AIC_BASE		( AIC_OFFS )
#define AIC_ENABLE_REG_0	( AIC_BASE + AIC_EN_REG_0_OFFS)
#define AIC_ENABLE_REG_1	( AIC_BASE + AIC_EN_REG_1_OFFS )
#define AIC_INTERRUPTS_REG_0	( AIC_BASE + AIC_INTERRUPTS_REG_0_OFFS )
#define AIC_INTERRUPTS_REG_1	( AIC_BASE + AIC_INTERRUPTS_REG_1_OFFS )

#define AIC_REQ_BASE		( AIC_BASE + AIC_REQ_OFFS )

#define AIC_REQ_TLB		( AIC_BASE + AIC_REQ_TLB_OFFS ) 
#define AIC_REQ_PR		( AIC_BASE + AIC_REQ_PR_OFFS ) 
#define AIC_REQ_PR_CODE		( AIC_BASE + AIC_REQ_PR_CODE_OFFS )

#define AIC_REQ_TLB_MASK	0x000000FF
#define AIC_REQ_PR_MASK		0x0000FF00
#define AIC_REQ_PR_CODE_MASK	0x00FF0000

//AXI Accelerator Wrapper
#define AAW_OFFS		0x10000

#define AAW0_BASE	( AAW_OFFS )

//AXI Accelerator Wrapper Controller
#define AAWC_OFFS		0x04

#define AAW0C_BASE	( AAW0_BASE  )
#define AAW0C_STATUS	( AAW0_BASE  + AAWC_OFFS )

#define REQ_ISOL_EN		0x001
#define REQ_ISOL_DIS		0x002
#define AXI_LITE_ISOL_EN	0x004
#define AXI_LITE_ISOL_DIS	0x008
#define AXI_FULL_ISOL_EN	0x010
#define AXI_FULL_ISOL_DIS	0x020
#define IRQ_EN			0x040
#define IRQ_DIS			0x080
#define IRQ_ACK			0x100
#define RESET_TLB		0x200
#define RESET_PR		0x400

#define ISOLATE_VALUE		( REQ_ISOL_EN | AXI_LITE_ISOL_EN | AXI_FULL_ISOL_EN  )
#define DEISOLATE_VALUE		( REQ_ISOL_DIS | AXI_LITE_ISOL_DIS | AXI_FULL_ISOL_DIS  )

//AXI Accelerator Wrapper Programmable Region
#define ACC_OFFSET		0x8000
#define AAW0PR_BASE     ( AAW0_BASE + ACC_OFFSET )

//AXI Accelerator Wrapper Translation Looakside buffer
#define TLB_OFFSET		0x100
#define AAW0TLB_BASE	( AAW0_BASE + TLB_OFFSET )

enum
{
  AAW_0,
  AAW_1,
  AAW_2,
  AAW_3,
  AAW_4,
  AAW_5,
  AAW_6,
  AAW_7,
  AAW_8,
  AAW_9,
  AAW_10,
  AAW_11,
  AAW_12,
  AAW_13,
  AAW_14,
  AAW_15,
  AAW_16,
  AAW_17,
  AAW_18,
  AAW_19,
  AAW_20,
  AAW_21,
  AAW_22,
  AAW_23,
  AAW_24,
  AAW_25,
  AAW_26,
  AAW_27,
  AAW_28,
  AAW_29,
  AAW_30,
  AAW_31,
  AAW_MAX
};

enum
{
  ISOLATOR_0,
  ISOLATOR_1,
  ISOLATOR_2,
  ISOLATOR_MAX,
};

enum
{
  TYPE_0,
  TYPE_1,
  TYPE_2,
  TYPE_MAX,
};

typedef struct aawc_t
{
  
} AAWC_T;

typedef struct isolator_t
{
  uint32_t status;
} ISOLATOR_T;

typedef struct tlb_t
{
  
} TLB_T;

typedef struct pr_t
{
  uint32_t id;
  uint32_t module;
  uint32_t status;
  uint32_t frequency;
  bool status_changed;
  uint32_t timeout;
  uint8_t timeout_count;

} PR_T;

typedef struct aaw_t
{
    AAWC_T	aawc;
    ISOLATOR_T	isolator[ISOLATOR_MAX];
    TLB_T	tlb;
    PR_T	pr;
} AAW_T;

typedef struct bitstream_t
{
   char *full_bt_name;
   char pr_regions;
   char *pr_bt[AAW_MAX];
   char pr_modules;
   char full_bt[];
  
} BITSTREAM_T;

typedef struct pl_t
{
  uint32_t id;
  uint32_t status;
  AAW_T	aaw[AAW_MAX];
  BITSTREAM_T bit;
} PL_T;

enum
{
  STATUS_NONE,
  STATUS_BLANK,
  STATUS_PROGRAMMED,
  STATUS_RUNNING,
  STATUS_ENDED
};

#endif
