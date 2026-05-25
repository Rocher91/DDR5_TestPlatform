#include "DDR5_LCD_DIMM.h"
#include "DDR5_board.h"
#include "DDR5_Time.h"

#include <stdio.h>
#include <string.h>

#define DDR5_LCD_DIMM_COUNT  8U

static nhd0420_t *g_lcd = NULL;
static DDR5_DIMM_Table_t *g_table = NULL;
static uint8_t g_lcd_page = 0U;

static const char *DDR5_ACK_String(uint8_t ack)
{
    return (ack != 0U) ? "ACK" : "NACK";
}

static void DDR5_LCD_WriteLine20(nhd0420_t *lcd,
                                 uint8_t row,
                                 const char *text)
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

    nhd0420_write_line_20(lcd, row, line);
}

void DDR5_LCD_DIMM_Init(nhd0420_t *lcd,
                        DDR5_DIMM_Table_t *table)
{
    g_lcd = lcd;
    g_table = table;
    g_lcd_page = 0U;

    nhd0420_clear(g_lcd);
    DDR5_Delay_ms(50);

    DDR5_LCD_WriteLine20(g_lcd, 0, "DDR5 Platform Test  ");
    HAL_Delay(20);
    DDR5_LCD_WriteLine20(g_lcd, 1, "DIMM Scanner        ");
    DDR5_LCD_WriteLine20(g_lcd, 2, "Xavier Rocher      ");
    DDR5_LCD_WriteLine20(g_lcd, 3, "                    ");

    DDR5_Delay_ms(2500);
    nhd0420_clear(g_lcd);

    DDR5_LCD_DIMM_ShowPage(g_lcd_page);
}

void DDR5_LCD_DIMM_ShowPage(uint8_t page)
{
    char line[32];

    if ((g_lcd == NULL) || (g_table == NULL))
        return;

    if (page >= DDR5_LCD_DIMM_COUNT)
        page = 0U;

    DDR5_DIMM_t *dimm = &g_table->DIMM[page];

    nhd0420_clear(g_lcd);
    DDR5_Delay_ms(20);

    snprintf(line,
             sizeof(line),
             "DIMM%u %s 0x%02X",
             (unsigned int)page,
             (dimm->DIMM_Status != 0U) ? "OK" : "FAIL",
             dimm->SPD_Address);

    DDR5_LCD_WriteLine20(g_lcd, 0, line);

    snprintf(line,
             sizeof(line),
             "SPD %-4s 0x%02X",
             DDR5_ACK_String(dimm->SPD_Ack),
             dimm->SPD_Address);

    DDR5_LCD_WriteLine20(g_lcd, 1, line);

    snprintf(line,
             sizeof(line),
             "T0 %-4s T1 %-4s",
             DDR5_ACK_String(dimm->TS0_Ack),
             DDR5_ACK_String(dimm->TS1_Ack));

    DDR5_LCD_WriteLine20(g_lcd, 2, line);

    snprintf(line,
             sizeof(line),
             "PM %-4s RC %-4s",
             DDR5_ACK_String(dimm->PMIC_Ack),
             DDR5_ACK_String(dimm->RCD_Ack));

    DDR5_LCD_WriteLine20(g_lcd, 3, line);
}

void DDR5_LCD_DIMM_FSM_Update(void)
{
    if ((g_lcd == NULL) || (g_table == NULL))
        return;

    /*
     * BUTTON RIGHT
     */

    if (DDR5_RIGHTButtonPressed())
    {
        DDR5_Delay_ms(150);

        if (DDR5_RIGHTButtonPressed())
        {
            if (g_lcd_page < (DDR5_LCD_DIMM_COUNT - 1U))
                g_lcd_page++;
            else
                g_lcd_page = 0U;

            DDR5_LCD_DIMM_ShowPage(g_lcd_page);

            while (DDR5_RIGHTButtonPressed())
            {
            }
        }
    }

    /*
     * BUTTON LEFT
     */

    if (DDR5_LEFTButtonPressed())
    {
        DDR5_Delay_ms(150);

        if (DDR5_LEFTButtonPressed())
        {
            if (g_lcd_page > 0U)
                g_lcd_page--;
            else
                g_lcd_page = DDR5_LCD_DIMM_COUNT - 1U;

            DDR5_LCD_DIMM_ShowPage(g_lcd_page);

            while (DDR5_LEFTButtonPressed())
            {
            }
        }
    }
}

void DDR5_LCD_DIMM_NextPage(void)
{
    if ((g_lcd == NULL) || (g_table == NULL))
        return;

    if (g_lcd_page < (DDR5_LCD_DIMM_COUNT - 1U))
        g_lcd_page++;
    else
        g_lcd_page = 0U;

    DDR5_LCD_DIMM_ShowPage(g_lcd_page);
}

void DDR5_LCD_DIMM_PreviousPage(void)
{
    if ((g_lcd == NULL) || (g_table == NULL))
        return;

    if (g_lcd_page > 0U)
        g_lcd_page--;
    else
        g_lcd_page = DDR5_LCD_DIMM_COUNT - 1U;

    DDR5_LCD_DIMM_ShowPage(g_lcd_page);
}
