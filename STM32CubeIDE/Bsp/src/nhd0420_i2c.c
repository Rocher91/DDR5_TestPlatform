#include "nhd0420_i2c.h"

/* Return codes */
#define NHD0420_OK            0
#define NHD0420_ERR_PARAM    -1
#define NHD0420_ERR_I2C      -2

#define NHD0420_I2C_TIMEOUT_MS  1000U

int nhd0420_i2c_write_raw(I2C_HandleTypeDef *hi2c,
                          uint8_t dev_addr_7bit,
                          const uint8_t *data,
                          uint16_t size)
{
    if ((hi2c == NULL) || (data == NULL) || (size == 0U))
        return NHD0420_ERR_PARAM;

    if (HAL_I2C_Master_Transmit(hi2c,
                                (uint16_t)(dev_addr_7bit << 1),
                                (uint8_t *)data,
                                size,
                                NHD0420_I2C_TIMEOUT_MS) != HAL_OK)
    {
        return NHD0420_ERR_I2C;
    }

    return NHD0420_OK;
}

static int nhd0420_send_cmd0(nhd0420_t *lcd, uint8_t cmd)
{
    uint8_t buf[2] = { NHD0420_PREFIX, cmd };
    return nhd0420_i2c_write_raw(lcd->hi2c, lcd->addr_7bit, buf, 2);
}

static int nhd0420_send_cmd1(nhd0420_t *lcd, uint8_t cmd, uint8_t p1)
{
    uint8_t buf[3] = { NHD0420_PREFIX, cmd, p1 };
    return nhd0420_i2c_write_raw(lcd->hi2c, lcd->addr_7bit, buf, 3);
}

static uint8_t nhd0420_rowcol_to_pos(uint8_t row, uint8_t col)
{
    static const uint8_t row_base[4] = { 0x00U, 0x40U, 0x14U, 0x54U };
    //static const uint8_t row_base[4] = { 0U, 20U, 40U, 60U };
    return (uint8_t)(row_base[row] + col);
}

int nhd0420_init(nhd0420_t *lcd,
                 I2C_HandleTypeDef *hi2c,
                 uint8_t addr_7bit,
                 void (*delay_ms_fn)(uint32_t ms))
{
    if ((lcd == NULL) || (hi2c == NULL) || (delay_ms_fn == NULL))
        return NHD0420_ERR_PARAM;

    lcd->hi2c = hi2c;
    lcd->addr_7bit = addr_7bit;
    lcd->delay_ms = delay_ms_fn;

    lcd->delay_ms(150U);

    return nhd0420_display_on(lcd);
}

int nhd0420_display_on(nhd0420_t *lcd)
{
    return nhd0420_send_cmd0(lcd, NHD0420_CMD_DISPLAY_ON);
}

int nhd0420_display_off(nhd0420_t *lcd)
{
    return nhd0420_send_cmd0(lcd, NHD0420_CMD_DISPLAY_OFF);
}

int nhd0420_clear(nhd0420_t *lcd)
{
    int rc = nhd0420_send_cmd0(lcd, NHD0420_CMD_CLEAR);
    if ((rc == NHD0420_OK) && (lcd->delay_ms != NULL))
        lcd->delay_ms(10U);
    return rc;
}

int nhd0420_home(nhd0420_t *lcd)
{
    int rc = nhd0420_send_cmd0(lcd, NHD0420_CMD_HOME);
    if ((rc == NHD0420_OK) && (lcd->delay_ms != NULL))
        lcd->delay_ms(2U);
    return rc;
}

int nhd0420_set_cursor_pos(nhd0420_t *lcd, uint8_t pos)
{
    return nhd0420_send_cmd1(lcd, NHD0420_CMD_SET_CURSOR, pos);
}

int nhd0420_set_cursor_rc(nhd0420_t *lcd, uint8_t row, uint8_t col)
{
    int rc;

    if ((row >= 4U) || (col >= 20U))
        return NHD0420_ERR_PARAM;

    rc = nhd0420_set_cursor_pos(lcd, nhd0420_rowcol_to_pos(row, col));

    if ((rc == NHD0420_OK) && (lcd->delay_ms != NULL))
        lcd->delay_ms(2U);

    return rc;
}

int nhd0420_write_char(nhd0420_t *lcd, char c)
{
    uint8_t buf[1];

    if (lcd == NULL)
        return NHD0420_ERR_PARAM;

    buf[0] = (uint8_t)c;

    if (HAL_I2C_Master_Transmit(lcd->hi2c,
                                (uint16_t)(lcd->addr_7bit << 1),
                                buf,
                                1,
                                1000) != HAL_OK)
    {
        return NHD0420_ERR_I2C;
    }

    if (lcd->delay_ms != NULL)
        lcd->delay_ms(2);

    return NHD0420_OK;
}
int nhd0420_write_data(nhd0420_t *lcd, const uint8_t *data, size_t len)
{
    if ((lcd == NULL) || (data == NULL) || (len == 0U))
        return NHD0420_ERR_PARAM;

    for (size_t i = 0; i < len; i++)
    {
        int rc = nhd0420_i2c_write_raw(lcd->hi2c,
                                       lcd->addr_7bit,
                                       &data[i],
                                       1);
        if (rc != NHD0420_OK)
            return rc;
    }

    return NHD0420_OK;
}

int nhd0420_write_string(nhd0420_t *lcd, const char *s)
{
    if ((lcd == NULL) || (s == NULL))
        return NHD0420_ERR_PARAM;

    while (*s != '\0')
    {
        int rc = nhd0420_write_char(lcd, *s++);
        if (rc != NHD0420_OK)
            return rc;
    }

    return NHD0420_OK;
}

int nhd0420_write_line(nhd0420_t *lcd, uint8_t row, const char *s)
{
    int rc;

    rc = nhd0420_set_cursor_rc(lcd, row, 0);
    if (rc != NHD0420_OK)
        return rc;

    for (uint8_t col = 0; col < 20U; col++)
    {
        char ch = ' ';

        if ((s != NULL) && (s[col] != '\0'))
            ch = s[col];

        rc = nhd0420_write_char(lcd, ch);

        if (rc != NHD0420_OK)
            return rc;
    }

    return NHD0420_OK;
}

int nhd0420_write_line_20(nhd0420_t *lcd, uint8_t row, const char *text)
{
    if ((lcd == NULL) || (row >= 4U))
        return NHD0420_ERR_PARAM;

    return nhd0420_write_line(lcd, row, text);
}

int nhd0420_set_contrast(nhd0420_t *lcd, uint8_t contrast_1_to_50)
{
    if ((contrast_1_to_50 < 1U) || (contrast_1_to_50 > 50U))
        return NHD0420_ERR_PARAM;

    return nhd0420_send_cmd1(lcd, NHD0420_CMD_CONTRAST, contrast_1_to_50);
}

int nhd0420_set_backlight(nhd0420_t *lcd, uint8_t brightness_1_to_8)
{
    if ((brightness_1_to_8 < 1U) || (brightness_1_to_8 > 8U))
        return NHD0420_ERR_PARAM;

    return nhd0420_send_cmd1(lcd, NHD0420_CMD_BRIGHTNESS, brightness_1_to_8);
}

int nhd0420_underline_on(nhd0420_t *lcd)
{
    return nhd0420_send_cmd0(lcd, NHD0420_CMD_UNDERLINE_ON);
}

int nhd0420_underline_off(nhd0420_t *lcd)
{
    return nhd0420_send_cmd0(lcd, NHD0420_CMD_UNDERLINE_OFF);
}

int nhd0420_blink_on(nhd0420_t *lcd)
{
    return nhd0420_send_cmd0(lcd, NHD0420_CMD_BLINK_ON);
}

int nhd0420_blink_off(nhd0420_t *lcd)
{
    return nhd0420_send_cmd0(lcd, NHD0420_CMD_BLINK_OFF);
}

int nhd0420_shift_left(nhd0420_t *lcd)
{
    return nhd0420_send_cmd0(lcd, NHD0420_CMD_SHIFT_LEFT);
}

int nhd0420_shift_right(nhd0420_t *lcd)
{
    return nhd0420_send_cmd0(lcd, NHD0420_CMD_SHIFT_RIGHT);
}

int nhd0420_load_custom_char(nhd0420_t *lcd,
                             uint8_t addr_0_to_7,
                             const uint8_t bitmap[8])
{
    uint8_t buf[11];

    if ((lcd == NULL) || (bitmap == NULL) || (addr_0_to_7 > 7U))
        return NHD0420_ERR_PARAM;

    buf[0] = NHD0420_PREFIX;
    buf[1] = NHD0420_CMD_CUSTOM_CHAR;
    buf[2] = addr_0_to_7;

    for (uint8_t i = 0; i < 8U; i++)
        buf[3 + i] = bitmap[i];

    return nhd0420_i2c_write_raw(lcd->hi2c, lcd->addr_7bit, buf, 11);
}
