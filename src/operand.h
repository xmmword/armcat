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

#ifndef __OPERAND_H
#define __OPERAND_H

#include <stdio.h>
#include <stdint.h>


/*
    *    src/operand.h
    *    Date: 01/05/23
    *    Author: @xmmword
*/


/**
 * @brief Rotates an immediate instruction operand.
 * @param operand The immediate operand.
 * @param rotate Rotate value.
 * @returns Rotated immediate operand.
 */

static inline __always_inline int operand_rotate(const int operand, const int rotate) {
  return (
    (operand & ((1 << (rotate * 2)) - 1)) << (32 - (rotate * 2))) | (operand >> (rotate * 2)
  );
}

/**
 * @brief Sign extends an operand.
 * @param data The data.
 * @param offset Bit offset.
 * @returns Sign extended operand.
 */

static inline __always_inline uint32_t operand_extend(const uint32_t data, 
  const int offset)
{
  return (
    (data & ((1U << offset) - 1)) ^ (1U << (offset - 1))) - (1U << (offset - 1)
  );
}

#endif