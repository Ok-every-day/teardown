/*
 * Copyright 2021 Gianluca Pacchiella <gp@ktln2.org>
 *
 * This file is part of open-adec.
 *
 * open-adec is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * open-adec is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with open-adec.  If not, see <http://www.gnu.org/licenses/>.
 */
// https://interrupt.memfault.com/blog/asserts-in-embedded-systems
#include "assert.h"
#include "log.h"

void _assert(const char* filename, u32 linenum, const char* expr) {
    log("ASSERT failed at ");log(filename);log(":");/*log(linenum);*/log(":");log(expr);log("\n");

    __asm("break");
}
