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

#ifndef __DISASM_H
#define __DISASM_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "armcat.h"
#include "decode.h"
#include "operand.h"

/* Macros for sign-extending and rotating immediate instruction operands! */
#define ARMCAT_OPERAND_EXTEND(instr, offset) operand_extend(instr, offset)
#define ARMCAT_OPERAND_ROTATE(operand, rotate) operand_rotate(operand, rotate)


/*
    *    src/disasm.h
    *    Date: 01/05/23
    *    Author: @xmmword
*/


static armcat_status_t disasm_format_mul_instr(armcat_instr_t *instr);
static armcat_status_t disasm_format_misc_instr(armcat_instr_t *instr);

static armcat_status_t disasm_format_data_instr(armcat_instr_t *instr, 
  const armcat_opcode_table_t *info);

static armcat_status_t disasm_format_branch_instr(armcat_instr_t *instr,
  const armcat_opcode_table_t *info);

static armcat_status_t disasm_format_ldrstr_instr(armcat_instr_t *instr,
  const armcat_opcode_table_t *info);

armcat_status_t disasm_instr(armcat_instr_t *instr, const uint32_t data);

#endif