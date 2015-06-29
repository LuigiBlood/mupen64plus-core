/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*   Mupen64plus - pi_controller.c                                         *
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

#include "api/callbacks.h"
#include "api/m64p_types.h"
#include "main/main.h"
#include "memory/memory.h"
#include "r4300/r4300_core.h"
#include "ri/ri_controller.h"


void connect_dd(struct dd_controller* dd,
                struct r4300_core* r4300,
                struct ri_controller* ri)
{
	dd->r4300 = r4300;
	dd->ri = ri;
}

void init_dd(struct dd_controller* dd)
{
	memset(dd->regs, 0, ASIC_REGS_COUNT*sizeof(uint32_t));
}

int read_dd_regs(void* opaque, uint32_t address, uint32_t* value)
{
	//F-Zero X hack, unchanged
	*value = (address == 0xa5000508)
		? 0xffffffff
		: 0x00000000;

	return 0;
}

int write_dd_regs(void* opaque, uint32_t address, uint32_t value, uint32_t mask)
{
	return 0;
}
