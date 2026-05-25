#ifndef BSP_INC_DDR5_LCD_DIMM_H_
#define BSP_INC_DDR5_LCD_DIMM_H_

#include "DDR5_DIMM_Detect.h"
#include "nhd0420_i2c.h"

void DDR5_LCD_DIMM_Init(nhd0420_t *lcd,
                        DDR5_DIMM_Table_t *table);

void DDR5_LCD_DIMM_ShowPage(uint8_t page);

void DDR5_LCD_DIMM_FSM_Update(void);
void DDR5_LCD_DIMM_NextPage(void);
void DDR5_LCD_DIMM_PreviousPage(void);

#endif
