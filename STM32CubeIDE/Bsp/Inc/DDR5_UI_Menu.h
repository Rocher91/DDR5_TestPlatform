/*
 * DDR5_UI_Menu.h
 *
 *  Created on: May 20, 2026
 *      Author: Xavi
 */

#ifndef BSP_INC_DDR5_UI_MENU_H_
#define BSP_INC_DDR5_UI_MENU_H_

#include "nhd0420_i2c.h"
#include "DDR5_DIMM_Detect.h"

void DDR5_UI_Menu_Init(nhd0420_t *lcd,
                       DDR5_DIMM_Table_t *dimm_table);

void DDR5_UI_Menu_FSM_Update(void);


#endif /* BSP_INC_DDR5_UI_MENU_H_ */
