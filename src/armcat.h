/*
 * Copyright (C) 2023 xmmword
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __ARMCAT_H
#define __ARMCAT_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "instr.h"

#define ARMCAT_INSTR_SIZEMAX 4 /* Maximum size of an ARM instruction. */

/* ARMCAT disassembler API statuses. */
#define ARMCAT_STATUS_SUCCESS  1
#define ARMCAT_STATUS_FAILURE -1


/*
    *    src/armcat.h
    *    Date: 01/05/23
    *    Author: @xmmword
*/


typedef int armcat_status_t; /* Type-definition for int. */

typedef struct _armcat_disasm {
  size_t ninstr; /* The amount of instructions. */
  armcat_instr_t *instructions; /* A dynamically-allocated array of structs containing the disassembly data. */
} armcat_disasm_t;

void armcat_free(armcat_disasm_t *disassembly);
armcat_disasm_t *armcat_disasm(const void *buffer, const size_t nbytes);

#endif