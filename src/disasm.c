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

#include "instr.h"
#include "disasm.h"


/*
    *    src/disasm.c
    *    Date: 01/05/23
    *    Author: @xmmword
*/


/* ARM condition codes! */
const char *condition_codes[ARMCAT_CONDITION_CODES_AMOUNTMAX] = {
  "eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc", "hi", "ls", "ge", "lt", "gt", "le", "", ""
};

/* ARM registers! */
const char *registers[ARMCAT_REGISTERS_AMOUNTMAX] = {
  "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "sb", "sl", "fp", "ip", "sp", "lr", "pc", "r16"
};

/**
 * @brief Disassembles and formats MUL/MLA instructions.
 * @param instr A structure containing the decoded instruction attributes.
 * @returns ARMLIB_DISASM_SUCCESS if the instruction could be disassembled, ARMLIB_DISASM_FAILURE if otherwise.
 */

static armcat_status_t disasm_format_mul_instr(armcat_instr_t *instr) {
  const armcat_mul_instr_t *decoded = decode_mul_instr(instr->instr);

  switch (decoded->type) {
    case ARMCAT_MULINSTR_BIT_TYPE_MUL:
      snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "mul%s\tr%d, r%d, r%d", 
        condition_codes[decoded->code], decoded->dst, decoded->src, decoded->operand);
      return ARMCAT_STATUS_SUCCESS;
    case ARMCAT_MULINSTR_BIT_TYPE_MLA:
      snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "mla%s\tr%d, r%d, r%d", 
        condition_codes[decoded->code], decoded->dst, decoded->src, decoded->operand);
      return ARMCAT_STATUS_SUCCESS;
  }

  return ARMCAT_STATUS_FAILURE;
}

/**
 * @brief Disassembles and formats data-processing instructions.
 * @param instr A structure containing the decoded instruction attributes.
 * @returns ARMLIB_DISASM_SUCCESS if the instruction could be disassembled, ARMLIB_DISASM_FAILURE if otherwise.
 */

static armcat_status_t disasm_format_data_instr(armcat_instr_t *instr,
  const armcat_opcode_table_t *info)
{
  const armcat_data_instr_t *decoded = decode_data_instr(instr->instr);

  switch (decoded->type) {
    case ARMCAT_DATAINSTR_BIT_TYPE0:
      snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "%s%s\t%s, %s, %s",
        info->mnemonic, condition_codes[decoded->code], registers[decoded->dst], registers[decoded->src], registers[decoded->operand]);
      return ARMCAT_STATUS_SUCCESS;
    case ARMCAT_DATAINSTR_BIT_TYPE1:
      if ((info->opcode == ARMCAT_INSTR_CMP1 || info->opcode == ARMCAT_INSTR_CMP2) || (info->opcode == ARMCAT_INSTR_TST1 || info->opcode == ARMCAT_INSTR_TST2) 
        || (info->opcode == ARMCAT_INSTR_CMN1 || info->opcode == ARMCAT_INSTR_CMN2) || (info->opcode == ARMCAT_INSTR_TEQ1 || info->opcode == ARMCAT_INSTR_TEQ2))
      {
        snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "%s%s\t%s, %s",
          info->mnemonic, condition_codes[decoded->code], registers[decoded->src], registers[decoded->operand]);
        return ARMCAT_STATUS_SUCCESS;
      }

      snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "%s%s\t%s, %s",
        info->mnemonic, condition_codes[decoded->code], registers[decoded->dst], registers[decoded->operand]);
      return ARMCAT_STATUS_SUCCESS;
    case ARMCAT_DATAINSTR_BIT_TYPE2:
      snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "%s%s\t%s, %s, #0x%x", info->mnemonic, 
        condition_codes[decoded->code], registers[decoded->dst], registers[decoded->src], decoded->operand);
      return ARMCAT_STATUS_SUCCESS;
    case ARMCAT_DATAINSTR_BIT_TYPE3:
      if (!decoded->rot) {
        if ((info->opcode == ARMCAT_INSTR_CMP1 || info->opcode == ARMCAT_INSTR_CMP2) || (info->opcode == ARMCAT_INSTR_TST1 || info->opcode == ARMCAT_INSTR_TST2) 
          || (info->opcode == ARMCAT_INSTR_CMN1 || info->opcode == ARMCAT_INSTR_CMN2) || (info->opcode == ARMCAT_INSTR_TEQ1 || info->opcode == ARMCAT_INSTR_TEQ2))
        {
          snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "%s%s\t%s, #0x%x", info->mnemonic,
            condition_codes[decoded->code], registers[decoded->src], decoded->operand);
          return ARMCAT_STATUS_SUCCESS;
        }

        snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "%s%s\t%s, #0x%x", info->mnemonic,
          condition_codes[decoded->code], registers[decoded->dst], decoded->operand);
        return ARMCAT_STATUS_SUCCESS;
      }

      snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "%s\tr%d, #0x%x", info->mnemonic,
        decoded->dst, ARMCAT_OPERAND_ROTATE(decoded->operand, decoded->rot));
      return ARMCAT_STATUS_SUCCESS;
  }

  return ARMCAT_STATUS_FAILURE;
}

/**
 * @brief Disassembles and formats branching instructions.
 * @param instr A structure containing the decoded instruction attributes.
 * @returns ARMLIB_DISASM_SUCCESS if the instruction could be disassembled, ARMLIB_DISASM_FAILURE if otherwise.
 */

static armcat_status_t disasm_format_branch_instr(armcat_instr_t *instr,
  const armcat_opcode_table_t *info)
{
  const armcat_branch_instr_t *decoded = decode_branch_instr(instr->instr);

  switch (decoded->opcode) {
    case ARMCAT_BRANCH_OPCODE_TYPE_BX_REGIMM:
      snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "bx%s\tr%d", 
        condition_codes[decoded->code], decoded->operand);
      return ARMCAT_STATUS_SUCCESS;
    case ARMCAT_BRANCH_OPCODE_TYPE_BLX_REGIMM:
      snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "blx%s\tr%d", 
        condition_codes[decoded->code], decoded->operand);
      return ARMCAT_STATUS_SUCCESS;
  }

  switch (decoded->type) {
    case ARMCAT_BRANCH_BIT_TYPE_B:
      if (info->opcode == 0xa4 && decoded->code == ARMCAT_CONDITION_CODE_AL)
        snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "b\t#0x%x",
          ((ARMCAT_OPERAND_EXTEND(instr->instr, 24) << 2) + 8));
      else
        snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "b%s\t#0x%x",
          condition_codes[decoded->code], ((ARMCAT_OPERAND_EXTEND(instr->instr, 24) << 2) + 12));

      if (decoded->code == ARMCAT_CONDITION_CODE_UNCONDITIONAL)
        snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "blx%s\t#0x%x", 
          condition_codes[decoded->code], ((ARMCAT_OPERAND_EXTEND(instr->instr, 24) << 2) + 16));
      
      return ARMCAT_STATUS_SUCCESS;
    case ARMCAT_BRANCH_BIT_TYPE_BL:
      if (decoded->code == ARMCAT_CONDITION_CODE_AL)
        snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "bl\t#0x%x", 
          ((ARMCAT_OPERAND_EXTEND(instr->instr, 24) << 2) + 16));
      else
        snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "bl%s\t#0x%x", condition_codes[decoded->code],
          ((ARMCAT_OPERAND_EXTEND(instr->instr, 24) << 2) + 8));
      return ARMCAT_STATUS_SUCCESS;
  }

  return ARMCAT_STATUS_FAILURE;
}

/**
 * @brief Disassembles and formats load/store instructions.
 * @param instr A structure containing the decoded instruction attributes.
 * @returns ARMLIB_DISASM_SUCCESS if the instruction could be disassembled, ARMLIB_DISASM_FAILURE if otherwise.
 */

static armcat_status_t disasm_format_ldrstr_instr(armcat_instr_t *instr,
  const armcat_opcode_table_t *info)
{
  const armcat_ldrstr_instr_t *decoded = decode_ldrstr_instr(instr->instr);

  switch (decoded->immediate) {
    case ARMCAT_INSTR_LDRSTR_IMM:
      if (!decoded->offset && decoded->updown == ARMCAT_INSTR_UD_SET) {
        if (decoded->branch == ARMCAT_INSTR_BRANCH_SET) {
          snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "%sb\t%s, [%s]", info->mnemonic, 
            registers[decoded->dst], registers[decoded->src]);
          return ARMCAT_STATUS_SUCCESS;
        }

        snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "%s\t%s, [%s]", info->mnemonic, 
          registers[decoded->dst], registers[decoded->src]);
        return ARMCAT_STATUS_SUCCESS;
      }

      snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "%s\t%s, [%s, #0x%x]", info->mnemonic,
        registers[decoded->dst], registers[decoded->src], decoded->operand);
      return ARMCAT_STATUS_SUCCESS;
    case ARMCAT_INSTR_LDRSTR_REGIMM:
      snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "%s\t%s, [%s, %s]", info->mnemonic,
        registers[decoded->dst], registers[decoded->src], registers[decoded->operand]);
      return ARMCAT_STATUS_SUCCESS;
  }

  return ARMCAT_STATUS_FAILURE;
}

/**
 * @brief Disassembles and formats miscellaneous instructions.
 * @param instr A structure containing the decoded instruction attributes.
 * @returns ARMLIB_DISASM_SUCCESS if the instruction could be disassembled, ARMLIB_DISASM_FAILURE if otherwise.
 */

static armcat_status_t disasm_misc_instr(armcat_instr_t *instr) {
  const armcat_opcode_table_t *info = decode_opcode(instr->instr);
  if (!info)
    return ARMCAT_STATUS_FAILURE;

  const armcat_misc_instr_t *decoded = decode_misc_instr(instr->instr);

  switch (decoded->optype) {
    case ARMCAT_INSTR_MISC_GROUP1:
      if (decoded->opcode == ARMCAT_INSTR_HVC) {
        snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "hvc%s\t#0x%x",
          condition_codes[decoded->code], decoded->moperand);
        return ARMCAT_STATUS_SUCCESS;
      }
      break;
    case ARMCAT_INSTR_MISC_GROUP2:
      if (decoded->opcode == ARMCAT_INSTR_BXJ) {
        snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "bxj%s\t%s",
          condition_codes[decoded->code], registers[decoded->moperand]);
        return ARMCAT_STATUS_SUCCESS;
      }
      break;
    case ARMCAT_INSTR_MISC_GROUP3:
      if (decoded->opcode == ARMCAT_INSTR_CLZ) {
        snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "%s%s\t%s, %s",
          info->mnemonic, condition_codes[decoded->code], registers[decoded->dst], registers[decoded->moperand]);
        return ARMCAT_STATUS_SUCCESS;
      }
      break;
    case ARMCAT_INSTR_MISC_GROUP4:
      if (decoded->opcode == ARMCAT_INSTR_BKPT) {
        snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "bkpt%s\t#0x%x", 
          condition_codes[decoded->code], decoded->moperand);
        return ARMCAT_STATUS_SUCCESS;
      }
      break;
  }

  switch (info->opcode) {
    case ARMCAT_INSTR_SVC:
      snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "%s%s\t#0x%x", info->mnemonic, 
        condition_codes[decoded->code], decoded->operand);
      return ARMCAT_STATUS_SUCCESS;
    case ARMCAT_INSTR_NOP:
      snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "%s%s", info->mnemonic, condition_codes[decoded->code]);
      return ARMCAT_STATUS_SUCCESS;
    case ARMCAT_INSTR_RFE:
      snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "%s\t%s", info->mnemonic, registers[decoded->src]);
      return ARMCAT_STATUS_SUCCESS;
    case ARMCAT_INSTR_RFEDB:
      snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "%s\t%s", info->mnemonic, registers[decoded->src]);
      return ARMCAT_STATUS_SUCCESS;
    case ARMCAT_INSTR_CPS:
      snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "%s\t#0x%x", info->mnemonic, decoded->operand);
      return ARMCAT_STATUS_SUCCESS;
    case ARMCAT_INSTR_PLI:
      snprintf(instr->disasm_instr, ARMCAT_DISASM_INSTR_SIZEMAX, "%s\t[%s, #0x%x]", info->mnemonic,
        registers[decoded->src], decoded->operand);
      return ARMCAT_STATUS_SUCCESS;
  }

  switch (decoded->type) {
    case ARMCAT_INSTR_TYPE_MUL:
      return disasm_format_mul_instr(instr);
  }

  return ARMCAT_STATUS_FAILURE;
}

/**
 * @brief Disassembles an instruction.
 * @param instr A structure containing the decoded instruction attributes.
 * @param data The encoded instruction.
 * @returns ARMLIB_DISASM_SUCCESS if the instruction could be disassembled, ARMLIB_DISASM_FAILURE if otherwise.
 */

armcat_status_t disasm_instr(armcat_instr_t *instr, const uint32_t data) {
  instr->instr = data;

  if (disasm_misc_instr(instr) == ARMCAT_STATUS_SUCCESS)
    return ARMCAT_STATUS_SUCCESS;

  const armcat_opcode_table_t *info = decode_opcode(instr->instr);
  if (!info)
    return ARMCAT_STATUS_FAILURE;

  switch (info->group) {
    case LOAD_STORE:
      return disasm_format_ldrstr_instr(instr, info);
    case BRANCHING:
      return disasm_format_branch_instr(instr, info);
    case DATA_PROCESSING:
      return disasm_format_data_instr(instr, info);
  }

  return ARMCAT_STATUS_FAILURE;
}