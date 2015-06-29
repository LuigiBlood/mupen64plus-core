/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*   Mupen64plus - si_controller.h                                         *
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

#ifndef DD_CONTROLLER_H
#define DD_CONTROLLER_H

struct r4300_core;
struct ri_controller;

#include <stdint.h>

enum dd_registers
{
    ASIC_DATA,
    ASIC_MISC_REG,
    ASIC_CMD_STATUS,
    ASIC_CUR_TK,
    ASIC_BM_STATUS_CTL,
    ASIC_ERR_SECTOR,
    ASIC_SEQ_STATUS_CTL,
    ASIC_CUR_SECTOR,
    ASIC_HARD_RESET,
    ASIC_C1_S0,
    ASIC_HOST_SECBYTE,
    ASIC_C1_S2,
    ASIC_SEC_BYTE,
    ASIC_C1_S4,
    ASIC_C1_S6,
    ASIC_CUR_ADDR,
    ASIC_ID_REG,
    ASIC_TEST_REG,
    ASIC_TEST_PIN_SEL,
    ASIC_REGS_COUNT
};

struct dd_controller
{
    uint32_t regs[ASIC_REGS_COUNT];  
    
	struct r4300_core* r4300;
    struct ri_controller* ri;
};

static uint32_t dd_reg(uint32_t address)
{
    return (address & 0xffff) >> 2;
}

void connect_dd(struct dd_controller* dd,
                struct r4300_core* r4300,
                struct ri_controller* ri);

void init_dd(struct dd_controller* dd);

int read_dd_regs(void* opaque, uint32_t address, uint32_t* value);
int write_dd_regs(void* opaque, uint32_t address, uint32_t value, uint32_t mask);

#endif