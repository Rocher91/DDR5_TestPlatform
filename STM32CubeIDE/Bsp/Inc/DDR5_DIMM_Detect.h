/*
 * DDR5_DIMM_Detect.h
 *
 *  Created on: 6 May 2026
 *      Author: bcn-lab
 */

#ifndef INC_DDR5_DIMM_DETECT_H_
#define INC_DDR5_DIMM_DETECT_H_

#include <stdint.h>
#include "stm32h5xx.h"

#define DDR5_DIMM_COUNT          8U
#define DDR5_MR_COUNT            128U

typedef struct
{
   uint8_t SPD_Address;
   uint8_t TS0_Address;
   uint8_t TS1_Address;
   uint8_t PMIC_Address;
   uint8_t RCD_Address;
   uint8_t SPD_Ack;
   uint8_t TS0_Ack;
   uint8_t TS1_Ack;
   uint8_t PMIC_Ack;
   uint8_t RCD_Ack;
   uint8_t SPD_Regs[DDR5_MR_COUNT];
   uint8_t TS0_Regs[DDR5_MR_COUNT];
   uint8_t TS1_Regs[DDR5_MR_COUNT];
   uint8_t PMIC_Regs[DDR5_MR_COUNT];
   uint8_t RCD_Regs[DDR5_MR_COUNT];
   uint8_t DIMM_Status;
} DDR5_DIMM_t;

typedef struct
{
   DDR5_DIMM_t DIMM[DDR5_DIMM_COUNT];
} DDR5_DIMM_Table_t;


void DDR5_DIMM_Table_Init(DDR5_DIMM_Table_t *table);
void DDR5_DIMM_CheckAll(I2C_TypeDef *I2Cx,DDR5_DIMM_Table_t *table);


#endif

