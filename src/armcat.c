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

#include "armcat.h"
#include "decode.h"
#include "disasm.h"


/*
    *    src/armcat.c
    *    Date: 01/05/23
    *    Author: @xmmword
*/


/**
 * @brief Deallocates the memory that was allocated for the disassembly object.
 * @param disassembly The disassembly object.
 */

void armcat_free(armcat_disasm_t *disassembly) {
  free(disassembly->instructions);
  free(disassembly);
}

/**
 * @brief Disassembles a given buffer.
 * @param buffer The buffer.
 * @param nbytes The size.
 * @returns A struct containing the disassembly data.
 */

armcat_disasm_t *armcat_disasm(const void *buffer, const size_t nbytes) {
  armcat_disasm_t *disassembly = calloc(1, sizeof(armcat_disasm_t));
  if (!disassembly)
    return NULL;

  if (!(disassembly->instructions = calloc((disassembly->ninstr = (nbytes / ARMCAT_INSTR_SIZEMAX)), 
    sizeof(armcat_instr_t))))
  {
    free(disassembly);

    return NULL;
  }

  for (size_t i = 0, pc = 0; i < disassembly->ninstr; ++i, pc += ARMCAT_INSTR_SIZEMAX) {
    disasm_instr(&disassembly->instructions[i], *(uint32_t *)(buffer + pc));

    #ifdef ARMCAT_DEBUG
      printf("[debug]: status: %d\n", disasm_instr(&disassembly->instructions[i], *(uint32_t *)(buffer + pc)));
    #endif
  }

  return disassembly;
}