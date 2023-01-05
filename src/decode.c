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

#include "disasm.h"
#include "decode.h"


/*
    *    src/decode.c
    *    Date: 01/05/23
    *    Author: @xmmword
*/


/* Opcode table containing the mnemonic and group information. */
const armcat_opcode_table_t opcode_table[ARMCAT_OPCODE_TABLE_SIZE] = {
  {"adc", 0x0a, DATA_PROCESSING},
  {"adc", 0x2a, DATA_PROCESSING},
  {"and", 0x00, DATA_PROCESSING},
  {"and", 0x20, DATA_PROCESSING},
  {"add", 0x28, DATA_PROCESSING},
  {"add", 0x08, DATA_PROCESSING},
  {"bic", 0x3c, DATA_PROCESSING},
  {"bic", 0x1c, DATA_PROCESSING},
  {"mov", 0x3a, DATA_PROCESSING},
  {"mov", 0x1a, DATA_PROCESSING},
  {"mvn", 0x3e, DATA_PROCESSING},
  {"mvn", 0x1e, DATA_PROCESSING},
  {"orr", 0x38, DATA_PROCESSING},
  {"orr", 0x18, DATA_PROCESSING},
  {"sub", 0x24, DATA_PROCESSING},
  {"sub", 0x04, DATA_PROCESSING},
  {"cmp", 0x35, DATA_PROCESSING},
  {"cmp", 0x15, DATA_PROCESSING},
  {"cmn", 0x37, DATA_PROCESSING},
  {"cmn", 0x17, DATA_PROCESSING},
  {"rsb", 0x26, DATA_PROCESSING},
  {"rsb", 0x06, DATA_PROCESSING},
  {"eor", 0x22, DATA_PROCESSING},
  {"eor", 0x02, DATA_PROCESSING},
  {"teq", 0x33, DATA_PROCESSING},
  {"teq", 0x13, DATA_PROCESSING},
  {"tst", 0x31, DATA_PROCESSING},
  {"tst", 0x11, DATA_PROCESSING},
  {"rsc", 0x2e, DATA_PROCESSING},
  {"rsc", 0x0e, DATA_PROCESSING},
  {"sbc", 0x2c, DATA_PROCESSING},
  {"sbc", 0x0c, DATA_PROCESSING},
  {"ldr", 0x59, LOAD_STORE},
  {"ldr", 0x79, LOAD_STORE},
  {"ldrt", 0x4b, LOAD_STORE},
  {"ldrb", 0x5d, LOAD_STORE},
  {"ldrb", 0x7d, LOAD_STORE},
  {"ldrbt", 0x4e, LOAD_STORE},
  {"str", 0x52, LOAD_STORE},
  {"str", 0x58, LOAD_STORE},
  {"str", 0x78, LOAD_STORE},
  {"strt", 0x4a, LOAD_STORE},
  {"strb", 0x5c, LOAD_STORE},
  {"strb", 0x7c, LOAD_STORE},
  {"strbt", 0x4f, LOAD_STORE},
  {"b", 0xa4, BRANCHING},
  {"b", 0xa0, BRANCHING},
  {"bl", 0xb0, BRANCHING},
  {"bx", 0x12, BRANCHING},
  {"svc", 0xf0, MISCELLANEOUS},
  {"clz", 0x16, MISCELLANEOUS},
  {"nop", 0x32, MISCELLANEOUS},
  {"rfe", 0x89, MISCELLANEOUS},
  {"rfedb", 0x91, MISCELLANEOUS},
  {"cps", 0x10, MISCELLANEOUS},
  {"pli", 0x4d, MISCELLANEOUS}
};

/**
 * @brief Decodes MUL/MLA instructions.
 * @param instr The instruction.
 * @returns A structure containing the decoded instruction attributes.
 */

armcat_mul_instr_t *decode_mul_instr(const uint32_t instr) {
  return &(armcat_mul_instr_t) {
    .src     = ARMCAT_MUL_SRCREG_DECODE(instr),
    .dst     = ARMCAT_MUL_DSTREG_DECODE(instr),
    .code    = ARMCAT_CONDITION_CODE_DECODE(instr),
    .type    = ARMCAT_MUL_BIT_DECODE(instr),
    .operand = ARMCAT_MUL_OPERAND_DECODE(instr)
  };
}

/**
 * @brief Decodes data-processing instructions.
 * @param instr The instruction.
 * @returns A structure containing the decoded instruction attributes.
 */

armcat_data_instr_t *decode_data_instr(const uint32_t instr) {
  return &(armcat_data_instr_t) {
    .src     = ARMCAT_SRCREG_DECODE(instr),
    .dst     = ARMCAT_DSTREG_DECODE(instr),
    .rot     = ARMCAT_DATAINSTR_ROT_DECODE(instr),
    .code    = ARMCAT_CONDITION_CODE_DECODE(instr),
    .type    = ARMCAT_DATAINSTR_IMM_OPERAND_DECODE(instr),
    .operand = ARMCAT_OPERAND_DECODE(instr)
  };
}

/**
 * @brief Decodes miscellaneous instructions.
 * @param instr The instruction.
 * @returns A structure containing the decoded instruction attributes.
 */

armcat_misc_instr_t *decode_misc_instr(const uint32_t instr) {
  return &(armcat_misc_instr_t) {
    .src      = ARMCAT_SRCREG_DECODE(instr),
    .dst      = ARMCAT_DSTREG_DECODE(instr),
    .code     = ARMCAT_CONDITION_CODE_DECODE(instr),
    .type     = ARMCAT_DATAINSTR_IMM_OPERAND_DECODE(instr),
    .opcode   = ARMCAT_PARSE_BITS(instr, 21, 23),
    .optype   = ARMCAT_PARSE_BITS(instr, 4, 7),
    .operand  = ARMCAT_OPERAND_DECODE(instr),
    .moperand = ARMCAT_MISC_REGISTER_DECODE(instr)
  };
}

/**
 * @brief Decodes branching instructions.
 * @param instr The instruction.
 * @returns A structure containing the decoded instruction attributes.
 */

armcat_branch_instr_t *decode_branch_instr(const uint32_t instr) {
  return &(armcat_branch_instr_t) {
    .src     = ARMCAT_SRCREG_DECODE(instr),
    .dst     = ARMCAT_DSTREG_DECODE(instr),
    .code    = ARMCAT_CONDITION_CODE_DECODE(instr),
    .type    = ARMCAT_DATAINSTR_IMM_OPERAND_DECODE(instr),
    .opcode  = ARMCAT_BRANCHING_OPCODE_DECODE(instr),
    .operand = ARMCAT_MISC_REGISTER_DECODE(instr)
  };
}

/**
 * @brief Decodes load/store instructions.
 * @param instr The instruction.
 * @returns A structure containing the decoded instruction attributes.
 */

armcat_ldrstr_instr_t *decode_ldrstr_instr(const uint32_t instr) {
  return &(armcat_ldrstr_instr_t) {
    .src       = ARMCAT_SRCREG_DECODE(instr),
    .dst       = ARMCAT_DSTREG_DECODE(instr),
    .code      = ARMCAT_CONDITION_CODE_DECODE(instr),
    .type      = ARMCAT_LDRSTR_BIT_DECODE(instr),
    .branch    = ARMCAT_LDRSTR_BRANCH_DECODE(instr),
    .offset    = ARMCAT_LDRSTR_OFFSET_DECODE(instr),
    .updown    = ARMCAT_LDRSTR_UPDOWN_BIT_DECODE(instr),
    .operand   = ARMCAT_OPERAND_DECODE(instr),
    .immediate = ARMCAT_LDRSTR_IMMEDIATE_DECODE(instr),
  };
}

/**
 * @brief Decodes the opcode of the instruction.
 * @param instr The instruction.
 * @returns An entry in the opcode table containing information related to the decoded opcode.
 */

const armcat_opcode_table_t *decode_opcode(const uint32_t instr) {
  const armcat_opcode_t opcode = ARMCAT_INSTR_ENCODING_TYPE_DECODE(instr);

  for (int i = 0; i < (sizeof(opcode_table) / sizeof(*opcode_table)); ++i)
    if (opcode == opcode_table[i].opcode)
      return &opcode_table[i];

  return NULL;
}