/*
 * DDR5_UI_Menu.c
 *
 *  Created on: May 20, 2026
 *      Author: Xavi
 */

#include "DDR5_UI_Menu.h"

#include "DDR5_Board.h"
#include "DDR5_LCD_DIMM.h"
#include "DDR5_I3C.h"
#include "DDR5_Time.h"

#include <stdio.h>
#include <string.h>

#define UI_MESSAGE_TIME_MS   2000U

typedef enum
{
    UI_BTN_NONE = 0,
    UI_BTN_UP,
    UI_BTN_DOWN,
    UI_BTN_LEFT,
    UI_BTN_RIGHT,
    UI_BTN_OK,
    UI_BTN_CANCEL
} UI_ButtonEvent_t;

typedef enum
{
    DDR5_UI_STATE_MAIN = 0,
    DDR5_UI_STATE_DIMM_TEST,
    DDR5_UI_STATE_CCC_MENU,
    DDR5_UI_STATE_READ_MENU,
    DDR5_UI_STATE_WRITE_MENU,

    DDR5_UI_STATE_READ_EDIT_ADDR,
    DDR5_UI_STATE_READ_EDIT_REG,
    DDR5_UI_STATE_READ_RESULT
} DDR5_UI_State_t;

static nhd0420_t *g_ui_lcd = NULL;
static DDR5_DIMM_Table_t *g_ui_dimm_table = NULL;

static DDR5_UI_State_t g_ui_state = DDR5_UI_STATE_MAIN;

static uint8_t g_main_index = 0U;
static uint8_t g_ccc_index = 0U;
static uint8_t g_read_index = 0U;
static uint8_t g_write_index = 0U;

static uint8_t g_i3c_enabled = 0U;
static uint8_t g_hid = 0x01U;
static uint8_t g_read_bus_i3c = 0U;
static uint8_t g_read_addr = 0x50U;
static uint8_t g_read_reg = 0x00U;
static uint8_t g_read_value = 0x00U;
static HAL_StatusTypeDef g_read_status = HAL_ERROR;

static void UI_WriteLine20(uint8_t row, const char *text)
{
    char line[21];

    for (uint8_t i = 0; i < 20U; i++)
    {
        char c = ' ';

        if ((text != NULL) && (text[i] != '\0'))
        {
            c = text[i];

            if ((c < 0x20) || (c > 0x7E))
                c = ' ';
        }

        line[i] = c;
    }

    line[20] = '\0';
    nhd0420_write_line_20(g_ui_lcd, row, line);
}

static UI_ButtonEvent_t UI_GetButtonEvent(void)
{
    if (DDR5_UPButtonPressed())
    {
        DDR5_Delay_ms(120);
        if (DDR5_UPButtonPressed())
        {
            while (DDR5_UPButtonPressed()) {}
            return UI_BTN_UP;
        }
    }

    if (DDR5_DOWNButtonPressed())
    {
        DDR5_Delay_ms(120);
        if (DDR5_DOWNButtonPressed())
        {
            while (DDR5_DOWNButtonPressed()) {}
            return UI_BTN_DOWN;
        }
    }

    if (DDR5_LEFTButtonPressed())
    {
        DDR5_Delay_ms(120);
        if (DDR5_LEFTButtonPressed())
        {
            while (DDR5_LEFTButtonPressed()) {}
            return UI_BTN_LEFT;
        }
    }

    if (DDR5_RIGHTButtonPressed())
    {
        DDR5_Delay_ms(120);
        if (DDR5_RIGHTButtonPressed())
        {
            while (DDR5_RIGHTButtonPressed()) {}
            return UI_BTN_RIGHT;
        }
    }

    if (DDR5_OKButtonPressed())
    {
        DDR5_Delay_ms(120);
        if (DDR5_OKButtonPressed())
        {
            while (DDR5_OKButtonPressed()) {}
            return UI_BTN_OK;
        }
    }

    if (DDR5_CANCELButtonPressed())
    {
        DDR5_Delay_ms(120);
        if (DDR5_CANCELButtonPressed())
        {
            while (DDR5_CANCELButtonPressed()) {}
            return UI_BTN_CANCEL;
        }
    }

    return UI_BTN_NONE;
}

static void UI_ShowMain(void)
{
    nhd0420_clear(g_ui_lcd);
    DDR5_Delay_ms(20);

    UI_WriteLine20(0, "MAIN MENU           ");
    UI_WriteLine20(1, (g_main_index == 0U) ? "> DIMM TEST         " : "  DIMM TEST         ");
    UI_WriteLine20(2, (g_main_index == 1U) ? "> CCC CMD           "   : "  CCC CMD           ");
    UI_WriteLine20(3, (g_main_index == 2U) ? "> READ REG          "  : "  READ REG           ");
}

static void UI_ShowMainPage2(void)
{
    nhd0420_clear(g_ui_lcd);
    DDR5_Delay_ms(20);

    UI_WriteLine20(0, "MAIN MENU           ");
    UI_WriteLine20(1, (g_main_index == 3U) ? "> WRITE REG         " : "  WRITE REG        ");
    UI_WriteLine20(2, g_i3c_enabled ? "I3C: ENABLED        " : "I3C: DISABLED       ");
    UI_WriteLine20(3, "OK select CAN back  ");
}

static void UI_ShowMainAuto(void)
{
    if (g_main_index <= 2U)
        UI_ShowMain();
    else
        UI_ShowMainPage2();
}

static void UI_ShowCCC(void)
{
    nhd0420_clear(g_ui_lcd);
    DDR5_Delay_ms(20);

    UI_WriteLine20(0, "CCC COMMANDS        ");
    UI_WriteLine20(1, (g_ccc_index == 0U) ? "> SETHID            "  : "  SETHID            ");
    UI_WriteLine20(2, (g_ccc_index == 1U) ? "> SETAASA           " : "  SETAASA           ");
    UI_WriteLine20(3, (g_ccc_index == 2U) ? "> RSTDAA            "  : "  RSTDAA            ");
}

static void UI_ShowRead(void)
{
    nhd0420_clear(g_ui_lcd);
    DDR5_Delay_ms(20);

    UI_WriteLine20(0, "READ REGISTER       ");
    UI_WriteLine20(1, (g_read_index == 0U) ? "> READ I2C          " : "  READ I2C         ");

    if (g_i3c_enabled)
        UI_WriteLine20(2, (g_read_index == 1U) ? "> READ I3C          " : "  READ I3C          ");
    else
        UI_WriteLine20(2, "  READ I3C LOCKED   ");

    UI_WriteLine20(3, "OK select CAN back  ");
}

static void UI_ShowReadAddr(void)
{
    char line[21];

    nhd0420_clear(g_ui_lcd);
    DDR5_Delay_ms(20);

    UI_WriteLine20(0, g_read_bus_i3c ? "READ I3C ADDR" : "READ I2C ADDR");

    snprintf(line, sizeof(line), "ADDR: 0x%02X", g_read_addr);
    UI_WriteLine20(1, line);

    UI_WriteLine20(2, "UP/DN +/-1");
    UI_WriteLine20(3, "OK next CAN back");
}

static void UI_ShowReadReg(void)
{
    char line[21];

    nhd0420_clear(g_ui_lcd);
    DDR5_Delay_ms(20);

    UI_WriteLine20(0, g_read_bus_i3c ? "READ I3C REG" : "READ I2C REG");

    snprintf(line, sizeof(line), "REG : 0x%02X", g_read_reg);
    UI_WriteLine20(1, line);

    UI_WriteLine20(2, "UP/DN +/-1");
    UI_WriteLine20(3, "OK read CAN back");
}

static void UI_ShowReadResult(void)
{
    char line[21];

    nhd0420_clear(g_ui_lcd);
    DDR5_Delay_ms(20);

    UI_WriteLine20(0, g_read_bus_i3c ? "READ I3C RESULT" : "READ I2C RESULT");

    snprintf(line, sizeof(line), "A:0x%02X R:0x%02X", g_read_addr, g_read_reg);
    UI_WriteLine20(1, line);

    if (g_read_status == HAL_OK)
    {
        snprintf(line, sizeof(line), "DATA: 0x%02X", g_read_value);
        UI_WriteLine20(2, line);
    }
    else
    {
        UI_WriteLine20(2, "READ FAILED");
    }

    UI_WriteLine20(3, "OK again CAN back");
}
static void UI_ShowWrite(void)
{
    nhd0420_clear(g_ui_lcd);
    DDR5_Delay_ms(20);

    UI_WriteLine20(0, "WRITE REGISTER");
    UI_WriteLine20(1, (g_write_index == 0U) ? "> WRITE I2C" : "  WRITE I2C");

    if (g_i3c_enabled)
        UI_WriteLine20(2, (g_write_index == 1U) ? "> WRITE I3C" : "  WRITE I3C");
    else
        UI_WriteLine20(2, "  WRITE I3C LOCKED");

    UI_WriteLine20(3, "OK select CAN back");
}

static void UI_ShowMessage(const char *l0,
                           const char *l1,
                           const char *l2,
                           const char *l3,
                           uint32_t delay_ms)
{
    nhd0420_clear(g_ui_lcd);
    DDR5_Delay_ms(20);

    UI_WriteLine20(0, l0);
    UI_WriteLine20(1, l1);
    UI_WriteLine20(2, l2);
    UI_WriteLine20(3, l3);

    DDR5_Delay_ms(delay_ms);
}
static void UI_ExecuteRead(void)
{
    g_read_value = 0x00U;

    if (g_read_bus_i3c)
    {
        if (g_i3c_enabled == 0U)
        {
            g_read_status = HAL_ERROR;

            UI_ShowMessage("I3C READ LOCKED",
                           "Send SETAASA first",
                           "",
                           "",
                           UI_MESSAGE_TIME_MS);

            g_ui_state = DDR5_UI_STATE_READ_MENU;
            UI_ShowRead();
            return;
        }

        g_read_status = I3C_LL_PrivateReadReg(g_read_addr,
                                              g_read_reg,
                                              &g_read_value,
                                              1);
    }
    else
    {
        g_read_status = I3C_LL_I2C_PrivateReadReg(g_read_addr,
                                                  g_read_reg,
                                                  &g_read_value,
                                                  1);
    }

    g_ui_state = DDR5_UI_STATE_READ_RESULT;
    UI_ShowReadResult();
}
static void UI_ExecuteCCC(void)
{
    HAL_StatusTypeDef st;

    if (g_ccc_index == 0U)
    {
        st = I3C_SETHID_Status();

        if (st == HAL_OK)
        {
            UI_ShowMessage("CCC SETHID OK",
                           "HID configured",
                           "Value 0x01",
                           "",
                           2500);
        }
        else
        {
            UI_ShowMessage("CCC SETHID FAIL",
                           "No Error_Handler",
                           "Menu still alive",
                           "",
                           2500);
        }
    }
    else if (g_ccc_index == 1U)
    {
        st = I3C_SETAASA_Status();

        if (st == HAL_OK)
        {
            g_i3c_enabled = 1U;

            UI_ShowMessage("CCC SETAASA OK",
                           "I3C enabled",
                           "",
                           "",
                           2500);
        }
        else
        {
            g_i3c_enabled = 0U;

            UI_ShowMessage("CCC SETAASA FAIL",
                           "I3C not enabled",
                           "Menu still alive",
                           "",
                           2500);
        }
    }
    else
    {
        st = I3C_RSTDAA_Status();

        if (st == HAL_OK)
        {
            g_i3c_enabled = 0U;

            UI_ShowMessage("CCC RSTDAA OK",
                           "I3C disabled",
                           "",
                           "",
                           2500);
        }
        else
        {
            UI_ShowMessage("CCC RSTDAA FAIL",
                           "Menu still alive",
                           "",
                           "",
                           2500);
        }
    }

    UI_ShowCCC();
}

void DDR5_UI_Menu_Init(nhd0420_t *lcd,
                       DDR5_DIMM_Table_t *dimm_table)
{
    g_ui_lcd = lcd;
    g_ui_dimm_table = dimm_table;

    g_ui_state = DDR5_UI_STATE_MAIN;
    g_main_index = 0U;
    g_ccc_index = 0U;
    g_read_index = 0U;
    g_write_index = 0U;
    g_i3c_enabled = 0U;
    g_hid = 0x01U;

    UI_ShowMainAuto();
}

void DDR5_UI_Menu_FSM_Update(void)
{
    UI_ButtonEvent_t ev;

    if ((g_ui_lcd == NULL) || (g_ui_dimm_table == NULL))
        return;

    ev = UI_GetButtonEvent();

    if (ev == UI_BTN_NONE)
        return;

    switch (g_ui_state)
    {
        case DDR5_UI_STATE_MAIN:
        {
            if (ev == UI_BTN_DOWN)
            {
                if (g_main_index < 3U)
                    g_main_index++;
                else
                    g_main_index = 0U;

                UI_ShowMainAuto();
            }
            else if (ev == UI_BTN_UP)
            {
                if (g_main_index > 0U)
                    g_main_index--;
                else
                    g_main_index = 3U;

                UI_ShowMainAuto();
            }
            else if (ev == UI_BTN_OK)
            {
                if (g_main_index == 0U)
                {
                    g_ui_state = DDR5_UI_STATE_DIMM_TEST;
                    DDR5_LCD_DIMM_Init(g_ui_lcd, g_ui_dimm_table);
                }
                else if (g_main_index == 1U)
                {
                    g_ui_state = DDR5_UI_STATE_CCC_MENU;
                    UI_ShowCCC();
                }
                else if (g_main_index == 2U)
                {
                    g_ui_state = DDR5_UI_STATE_READ_MENU;
                    UI_ShowRead();
                }
                else
                {
                    g_ui_state = DDR5_UI_STATE_WRITE_MENU;
                    UI_ShowWrite();
                }
            }
            break;
        }

        case DDR5_UI_STATE_DIMM_TEST:
        {
            if (ev == UI_BTN_CANCEL)
            {
                g_ui_state = DDR5_UI_STATE_MAIN;
                UI_ShowMainAuto();
            }
            else if (ev == UI_BTN_RIGHT)
            {
                DDR5_LCD_DIMM_NextPage();
            }
            else if (ev == UI_BTN_LEFT)
            {
                DDR5_LCD_DIMM_PreviousPage();
            }

            break;
        }

        case DDR5_UI_STATE_CCC_MENU:
        {
            if (ev == UI_BTN_DOWN)
            {
                if (g_ccc_index < 2U)
                    g_ccc_index++;
                else
                    g_ccc_index = 0U;

                UI_ShowCCC();
            }
            else if (ev == UI_BTN_UP)
            {
                if (g_ccc_index > 0U)
                    g_ccc_index--;
                else
                    g_ccc_index = 2U;

                UI_ShowCCC();
            }
            else if (ev == UI_BTN_OK)
            {
                UI_ExecuteCCC();
            }
            else if (ev == UI_BTN_CANCEL)
            {
                g_ui_state = DDR5_UI_STATE_MAIN;
                UI_ShowMainAuto();
            }

            break;
        }

        case DDR5_UI_STATE_READ_MENU:
        {
            if ((ev == UI_BTN_DOWN) || (ev == UI_BTN_UP))
            {
                g_read_index ^= 1U;
                UI_ShowRead();
            }
            else if (ev == UI_BTN_OK)
            {
                if ((g_read_index == 1U) && (g_i3c_enabled == 0U))
                {
                    UI_ShowMessage("I3C READ LOCKED",
                                   "Send SETAASA first",
                                   "",
                                   "",
                                   UI_MESSAGE_TIME_MS);
                    UI_ShowRead();
                }
                else
                {
                    g_read_bus_i3c = g_read_index;
                    g_ui_state = DDR5_UI_STATE_READ_EDIT_ADDR;
                    UI_ShowReadAddr();
                }
            }
            else if (ev == UI_BTN_CANCEL)
            {
                g_ui_state = DDR5_UI_STATE_MAIN;
                UI_ShowMainAuto();
            }

            break;
        }

        case DDR5_UI_STATE_WRITE_MENU:
        {
            if ((ev == UI_BTN_DOWN) || (ev == UI_BTN_UP))
            {
                g_write_index ^= 1U;
                UI_ShowWrite();
            }
            else if (ev == UI_BTN_OK)
            {
                if ((g_write_index == 1U) && (g_i3c_enabled == 0U))
                {
                    UI_ShowMessage("I3C WRITE LOCKED",
                                   "Send SETAASA first",
                                   "",
                                   "",
                                   1200);
                    UI_ShowWrite();
                }
                else
                {
                    UI_ShowMessage("WRITE MENU",
                                   "Next step:",
                                   "addr/reg/data",
                                   "",
                                   1200);
                    UI_ShowWrite();
                }
            }
            else if (ev == UI_BTN_CANCEL)
            {
                g_ui_state = DDR5_UI_STATE_MAIN;
                UI_ShowMainAuto();
            }

            break;
        }
        case DDR5_UI_STATE_READ_EDIT_ADDR:
        {
            if (ev == UI_BTN_UP)
            {
                g_read_addr++;
                UI_ShowReadAddr();
            }
            else if (ev == UI_BTN_DOWN)
            {
                g_read_addr--;
                UI_ShowReadAddr();
            }
            else if (ev == UI_BTN_RIGHT)
            {
                g_read_addr += 0x10U;
                UI_ShowReadAddr();
            }
            else if (ev == UI_BTN_LEFT)
            {
                g_read_addr -= 0x10U;
                UI_ShowReadAddr();
            }
            else if (ev == UI_BTN_OK)
            {
                g_ui_state = DDR5_UI_STATE_READ_EDIT_REG;
                UI_ShowReadReg();
            }
            else if (ev == UI_BTN_CANCEL)
            {
                g_ui_state = DDR5_UI_STATE_READ_MENU;
                UI_ShowRead();
            }

            break;
        }

        case DDR5_UI_STATE_READ_EDIT_REG:
        {
            if (ev == UI_BTN_UP)
            {
                g_read_reg++;
                UI_ShowReadReg();
            }
            else if (ev == UI_BTN_DOWN)
            {
                g_read_reg--;
                UI_ShowReadReg();
            }
            else if (ev == UI_BTN_RIGHT)
            {
                g_read_reg += 0x10U;
                UI_ShowReadReg();
            }
            else if (ev == UI_BTN_LEFT)
            {
                g_read_reg -= 0x10U;
                UI_ShowReadReg();
            }
            else if (ev == UI_BTN_OK)
            {
                UI_ExecuteRead();
            }
            else if (ev == UI_BTN_CANCEL)
            {
                g_ui_state = DDR5_UI_STATE_READ_EDIT_ADDR;
                UI_ShowReadAddr();
            }

            break;
        }

        case DDR5_UI_STATE_READ_RESULT:
        {
            if (ev == UI_BTN_OK)
            {
                UI_ExecuteRead();
            }
            else if (ev == UI_BTN_CANCEL)
            {
                g_ui_state = DDR5_UI_STATE_READ_MENU;
                UI_ShowRead();
            }

            break;
        }
        default:
        {
            g_ui_state = DDR5_UI_STATE_MAIN;
            UI_ShowMainAuto();
            break;
        }
    }
}
