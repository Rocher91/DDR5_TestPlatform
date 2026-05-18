/*
 * DDR5_Time.c
 *
 *  Created on: 30 Apr 2026
 *      Author: bcn-lab
 */

#include "DDR5_Time.h"

/* Tick global */

static volatile uint32_t g_ms_ticks = 0;

/* =========================================================

 * Tick increment (llamado desde SysTick_Handler)

 * ========================================================= */

void DDR5_Time_IncTick(void)

{

    g_ms_ticks++;

}

/* =========================================================

 * Get current tick

 * ========================================================= */

uint32_t DDR5_GetTick(void)

{

    return g_ms_ticks;

}

/* =========================================================

 * Delay en ms (blocking)

 * ========================================================= */

void DDR5_Delay_ms(uint32_t ms)

{

    uint32_t start = DDR5_GetTick();

    while ((DDR5_GetTick() - start) < ms)

    {

        /* busy wait */

    }

}

/* =========================================================

 * Timeout start

 * ========================================================= */

void DDR5_Timeout_Start(DDR5_Timeout_t *t, uint32_t timeout_ms)

{

    if (t == 0)

        return;

    t->start   = DDR5_GetTick();

    t->timeout = timeout_ms;

}

/* =========================================================

 * Timeout expired?

 * ========================================================= */

uint8_t DDR5_Timeout_Expired(const DDR5_Timeout_t *t)

{

    if (t == 0)

        return 1;

    if ((DDR5_GetTick() - t->start) >= t->timeout)

        return 1;

    return 0;

}

