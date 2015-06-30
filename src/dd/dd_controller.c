/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*   Mupen64plus - dd_controller.c                                         *
*   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
*   Copyright (C) 2014 Bobby Smiles                                       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "dd_controller.h"

#include <string.h>
#include <time.h>

#include "api/callbacks.h"
#include "api/config.h"
#include "api/m64p_types.h"
#include "main/main.h"
#include "memory/memory.h"
#include "r4300/r4300_core.h"
#include "si/pif.h"
#include "si/si_controller.h"

static unsigned char byte2bcd(int n)
{
    n %= 100;
    return ((n / 10) << 4) | (n % 10);
}

void connect_dd(struct dd_controller* dd,
                struct r4300_core* r4300)
{
    dd->r4300 = r4300;
}

void init_dd(struct dd_controller* dd)
{
    memset(dd->regs, 0, ASIC_REGS_COUNT*sizeof(uint32_t));
    memset(dd->c2_buf, 0, 0x400);
    memset(dd->sec_buf, 0, 0x100);
    memset(dd->mseq_buf, 0, 0x40);
}

int read_dd_regs(void* opaque, uint32_t address, uint32_t* value)
{
    struct dd_controller* dd = (struct dd_controller*)opaque;
    uint32_t reg = dd_reg(address);

    *value = 0x00000000;

    switch (reg)
    {
        case ASIC_CMD_STATUS:
            *value = (!ConfigGetParamBool(g_CoreConfig, "64DD")) ? 0xffffffff : dd->regs[ASIC_CMD_STATUS];
            break;

        default:
            if (reg < ASIC_REGS_COUNT)
                *value = dd->regs[reg];
    }

    return 0;
}

int write_dd_regs(void* opaque, uint32_t address, uint32_t value, uint32_t mask)
{
    struct dd_controller* dd = (struct dd_controller*)opaque;
    uint32_t reg = dd_reg(address);

    value &= 0xffff0000;

    switch (reg)
    {
        case ASIC_DATA:
            dd->regs[ASIC_DATA] = value;
            break;

        case ASIC_BM_STATUS_CTL:
            if (value == 0x01000000)
            {
                dd->regs[ASIC_CMD_STATUS] &= ~0x02000000;
            }
            break;

        case ASIC_CMD_STATUS:
            //ASIC Commands
            timeinfo = af_rtc_get_time(&g_si.pif.af_rtc);
            uint8_t year, month, hour, day, min, sec;

            switch (value >> 16)
            {
                case 0x12:
                    //Get Year/Month

                    //Put time in DATA as BCD
                    year = (uint8_t)byte2bcd(timeinfo->tm_year);
                    month = (uint8_t)byte2bcd((timeinfo->tm_mon + 1));

                    dd->regs[ASIC_DATA] = (year << 24) | (month << 16);
                    break;

                case 0x13:
                    //Get Day/Hour

                    //Put time in DATA as BCD
                    hour = (uint8_t)byte2bcd(timeinfo->tm_hour);
                    day = (uint8_t)byte2bcd(timeinfo->tm_mday);

                    dd->regs[ASIC_DATA] = (day << 24) | (hour << 16);
                    break;
                case 0x14:
                    //Get Min/Sec

                    //Put time in DATA as BCD
                    min = (uint8_t)byte2bcd(timeinfo->tm_min);
                    sec = (uint8_t)byte2bcd(timeinfo->tm_sec);

                    dd->regs[ASIC_DATA] = (min << 24) | (sec << 16);
                    break;
            }

            dd->regs[ASIC_CMD_STATUS] |= 0x02000000;
            update_count();
            add_interupt_event(CART_INT, 100);
            break;
    }

    return 0;
}

int dd_end_of_dma_event(struct dd_controller* dd)
{
    //Insert clear CART INT here or something
    if (!(dd->regs[ASIC_CMD_STATUS] & 0x02000000))
        return 1;

    return 0;
}