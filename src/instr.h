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

#ifndef __INSTR_H
#define __INSTR_H

#include <stdint.h>

#define ARMCAT_DISASM_INSTR_SIZEMAX 32 /* Maximum size of the disassembly buffer. */

#define ARMCAT_INSTR_TYPE_MUL 0x00000000 /* MUL/MLA instruction type. */

/* MUL/MLA instruction bit types! */
#define ARMCAT_MULINSTR_BIT_TYPE_MUL 0
#define ARMCAT_MULINSTR_BIT_TYPE_MLA 1

/* Macros representing condition codes! */
#define ARMCAT_CONDITION_CODE_AL            14
#define ARMCAT_CONDITION_CODE_UNCONDITIONAL 15

/* Branch instruction attribute information! */
#define ARMCAT_BRANCH_BIT_TYPE_B             10
#define ARMCAT_BRANCH_BIT_TYPE_BL            11
#define ARMCAT_BRANCH_OPCODE_TYPE_BX_REGIMM  1
#define ARMCAT_BRANCH_OPCODE_TYPE_BLX_REGIMM 3

/* Data-processing instruction bit types! */
#define ARMCAT_DATAINSTR_BIT_TYPE0 0
#define ARMCAT_DATAINSTR_BIT_TYPE1 1
#define ARMCAT_DATAINSTR_BIT_TYPE2 2
#define ARMCAT_DATAINSTR_BIT_TYPE3 3

/* Miscellaneous instruction group types! */
#define ARMCAT_INSTR_MISC_GROUP1 0b000
#define ARMCAT_INSTR_MISC_GROUP2 0b010
#define ARMCAT_INSTR_MISC_GROUP3 0b001
#define ARMCAT_INSTR_MISC_GROUP4 0b111

/* Miscellaneous instruction attribute information! */
#define ARMCAT_INSTR_UD_SET        1
#define ARMCAT_INSTR_BRANCH_SET    1
#define ARMCAT_INSTR_LDRSTR_IMM    0x00000000
#define ARMCAT_INSTR_LDRSTR_REGIMM 0x00000001

/* Macros representing opcodes! */
#define ARMCAT_INSTR_HVC   0b10
#define ARMCAT_INSTR_BXJ   0b01
#define ARMCAT_INSTR_CLZ   0b11
#define ARMCAT_INSTR_SVC   0xf0
#define ARMCAT_INSTR_NOP   0x32
#define ARMCAT_INSTR_RFE   0x89
#define ARMCAT_INSTR_CPS   0x10
#define ARMCAT_INSTR_PLI   0x4d
#define ARMCAT_INSTR_CMP1  0x15
#define ARMCAT_INSTR_CMP2  0x35
#define ARMCAT_INSTR_TST1  0x11
#define ARMCAT_INSTR_TST2  0x31
#define ARMCAT_INSTR_CMN1  0x17
#define ARMCAT_INSTR_CMN2  0x37
#define ARMCAT_INSTR_TEQ1  0x13
#define ARMCAT_INSTR_TEQ2  0x33
#define ARMCAT_INSTR_BKPT  0b01
#define ARMCAT_INSTR_RFEDB 0x91


/*
    *    src/instr.h
    *    Date: 01/05/23
    *    Author: @xmmword
*/


/* Structure containing the attributes of a MUL/MLA instruction. */
typedef struct _armcat_mul_instr {
  uint32_t src; /* The source register. */
  uint32_t dst; /* The destination register. */
  uint32_t code; /* The condition code. */
  uint32_t type; /* The instruction type. */
  uint32_t operand; /* The instruction operand. */
} armcat_mul_instr_t;

/* Structure containing the attributes of a miscellaneous instruction. */
typedef struct _armcat_misc_instr {
  uint32_t src; /* The source register. */
  uint32_t dst; /* The destination register. */
  uint32_t code; /* The condition code. */
  uint32_t type; /* The instruction type. */
  uint32_t opcode; /* The opcode. */
  uint32_t optype; /* The opcode type. */
  uint32_t operand; /* The instruction operand. */
  uint32_t moperand; /* The instruction operand AND (&) 0xf. */
} armcat_misc_instr_t;

/* Structure containing the attributes of a branch instruction. */
typedef struct _armcat_branch_instr {
  uint32_t src; /* The source register. */
  uint32_t dst; /* The destination register. */
  uint32_t code; /* The condition code. */
  uint32_t type; /* The instruction type. */
  uint32_t opcode; /* The opcode. */
  uint32_t operand; /* The instruction operand. */
} armcat_branch_instr_t;

/* Structure containing the attributes of a data-processing instruction. */
typedef struct _armcat_data_instr {
  uint32_t src; /* The source register. */
  uint32_t dst; /* The destination register. */
  uint32_t rot; /* The rotation bit. */
  uint32_t type; /* The instruction type. */
  uint32_t code; /* The condition code. */
  uint32_t opcode; /* The opcode. */
  uint32_t operand; /* The instruction operand. */
} armcat_data_instr_t;

/* Structure containing the attributes of a load/store instruction. */
typedef struct _armcat_ldrstr_instr {
  uint32_t src; /* The source register. */
  uint32_t dst; /* The destination register. */
  uint32_t code; /* The condition code. */
  uint32_t type; /* The instruction type. */
  uint32_t branch; /* The branch bit. */
  uint32_t offset; /* The immediate offset. (if any) */
  uint32_t updown; /* The updown bit. */
  uint32_t operand; /* The instruction operand. */
  uint32_t immediate; /* The immediate data. */
} armcat_ldrstr_instr_t;

/* Structure containing the disassembly data of an encoded instruction. */
typedef struct _armcat_instr {
  uint32_t instr; /* The encoded instruction. */
  char disasm_instr[ARMCAT_DISASM_INSTR_SIZEMAX]; /* The decoded and disassembled instruction. */
} armcat_instr_t;

#endif