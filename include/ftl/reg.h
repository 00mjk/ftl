/******************************************************************************
 *                                                                            *
 * Copyright 2019 Jan Henrik Weinstock                                        *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License");            *
 * you may not use this file except in compliance with the License.           *
 * You may obtain a copy of the License at                                    *
 *                                                                            *
 *     http://www.apache.org/licenses/LICENSE-2.0                             *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 *                                                                            *
 ******************************************************************************/

#ifndef FTL_REG_H
#define FTL_REG_H

#include "ftl/common.h"
#include "ftl/bitops.h"
#include "ftl/error.h"

namespace ftl {

    enum reg {
        RAX   = 0,  // caller-saved, return value
        RCX   = 1,  // caller-saved, arg3
        RDX   = 2,  // caller-saved, arg2
        RBX   = 3,
        RSP   = 4,
        RBP   = 5,
        RSI   = 6,  // arg1
        RDI   = 7,  // arg0
        R8    = 8,  // caller-saved, arg4
        R9    = 9,  // caller-saved, arg5
        R10   = 10, // caller-saved
        R11   = 11, // caller-saved
        R12   = 12,
        R13   = 13,
        R14   = 14,
        R15   = 15,
        NREGS = 16,
    };

    const char* reg_name(reg r);

    struct rm {
        const bool is_mem;
        const reg  r;
        const i32  offset;

        bool is_reg() const { return !is_mem; }

        rm(reg _r): is_mem(false), r(_r), offset(0) {}
        rm(reg base, i32 off): is_mem(true), r(base), offset(off) {}

    private:
        rm(); // disabled
    };

    static inline rm regop(reg r) {
        return rm(r);
    }

    static inline rm memop(reg base, i32 offset) {
        return rm(base, offset);
    }

    static inline reg argreg(unsigned int argno) {
        reg sysv[] = { RDI, RSI, RDX, RCX, R8, R9 };
        FTL_ERROR_ON(argno > ARRAY_SIZE(sysv), "argno out of bounds");
        return sysv[argno];
    }

}

std::ostream& operator << (std::ostream& os, const ftl::reg& r);
std::ostream& operator << (std::ostream& os, const ftl::rm& rm);

#endif
