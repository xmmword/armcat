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

#ifndef __DECODE_H
#define __DECODE_H

#include <stdio.h>
#include <stdint.h>

#include "instr.h"

#define ARMCAT_OPCODE_TABLE_SIZE 56 /* Size of opcode table! */

#define ARMCAT_REGISTERS_AMOUNTMAX       17 /* Maximum amount of registers! */
#define ARMCAT_CONDITION_CODES_AMOUNTMAX 16 /* Maximum amount of condition codes! */

/* Macro that parses bits from <start> to <end>! */
#define ARMCAT_PARSE_BITS(instr, offset, end) (((uint32_t)(instr) << (31u - (end))) >> ((offset) + 31u - (end)))

/* Macros for decoding generic instruction attributes. */
#define ARMCAT_OPERAND_DECODE(encoded)             (encoded & 0x000000FF)
#define ARMCAT_DSTREG_DECODE(encoded)              ((encoded & 0x0000F000) >> 12)
#define ARMCAT_SRCREG_DECODE(encoded)              ((encoded & 0x000F0000) >> 16)
#define ARMCAT_INSTR_ENCODING_TYPE_DECODE(encoded) ((encoded >> 20) & 0xFF)
#define ARMCAT_CONDITION_CODE_DECODE(encoded)      ((encoded & 0xF0000000) >> 28)

/* Macros for decoding data-processing instruction attributes. */
#define ARMCAT_DATAINSTR_ROT_DECODE(encoded)         ((encoded & 0x00000F00) >> 8)
#define ARMCAT_DATAINSTR_IMM_OPERAND_DECODE(encoded) ((encoded & 0x0F000000) >> 24)

/* Macro for decoding the register type for miscellaneous instructions. */
#define ARMCAT_MISC_REGISTER_DECODE(encoded)      (encoded & 0xf)

/* Macro for decoding the opcode type for branching instructions. */
#define ARMCAT_BRANCHING_OPCODE_DECODE(encoded)   ARMCAT_PARSE_BITS(encoded, 4, 7)

/* Macros for decoding MUL/MLA instruction attributes. */
#define ARMCAT_MUL_BIT_DECODE(encoded)     ((encoded & 0x00200000) >> 21)
#define ARMCAT_MUL_DSTREG_DECODE(encoded)  ((encoded & 0x000F0000) >> 16)
#define ARMCAT_MUL_SRCREG_DECODE(encoded)  (encoded & 0x0000000F)
#define ARMCAT_MUL_OPERAND_DECODE(encoded) ((encoded & 0x00000F00) >> 8)

/* Macros for decoding load/store instruction attributes. */
#define ARMCAT_LDRSTR_BIT_DECODE(encoded)        ((encoded & 0x00100000) >> 20)
#define ARMCAT_LDRSTR_BRANCH_DECODE(encoded)     ((encoded & 0x00400000) >> 22)
#define ARMCAT_LDRSTR_OFFSET_DECODE(encoded)     ((encoded << 20) >> 4)
#define ARMCAT_LDRSTR_IMMEDIATE_DECODE(encoded)  ((encoded & 0x02000000) >> 25)
#define ARMCAT_LDRSTR_UPDOWN_BIT_DECODE(encoded) ((encoded & 0x00800000) >> 23)


/*
    *    src/decode.h
    *    Date: 01/05/23
    *    Author: @xmmword
*/


typedef uint32_t armcat_opcode_t; /* Type-definition for uint32_t. */

/* Enumerations representing the immediate types. */
typedef enum _armcat_instr_imm {
  NONE,
  IMMEDIATE,
  REGISTER_IMMEDIATE
} armcat_instr_imm_t;

/* Enumerations representing the different instruction groups. */
typedef enum _armcat_instr_group {
  BRANCHING,
  LOAD_STORE,
  MISCELLANEOUS,
  DATA_PROCESSING
} armcat_instr_group_t;

/* The opcode table, each table entry will contain data related to a opcode. */
typedef struct _armcat_opcode_table {
  const char* restrict mnemonic; /* The mnemonic. */
  const armcat_opcode_t opcode; /* The opcode. */
  const armcat_instr_group_t group; /* The instruction group. */
} armcat_opcode_table_t;

const armcat_opcode_table_t *decode_opcode(const uint32_t instr);

armcat_mul_instr_t *decode_mul_instr(const uint32_t instr);
armcat_data_instr_t *decode_data_instr(const uint32_t instr);

armcat_misc_instr_t *decode_misc_instr(const uint32_t instr);

armcat_ldrstr_instr_t *decode_ldrstr_instr(const uint32_t instr);
armcat_branch_instr_t *decode_branch_instr(const uint32_t instr);

#endif