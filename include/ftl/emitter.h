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

#ifndef FTL_EMITTER_H
#define FTL_EMITTER_H

#include "ftl/common.h"
#include "ftl/error.h"
#include "ftl/bitops.h"

#include "ftl/reg.h"
#include "ftl/fixup.h"
#include "ftl/cbuf.h"

namespace ftl {

    class emitter
    {
    private:
        cbuf& m_buffer;

        inline void setup_fixup(fixup* fix, int size);

        size_t rex(bool is64, bool rexr, bool rexx, bool rexb);
        size_t modrm(int mod, int reg, int rm);
        size_t sib(int scale, int index, int base);

        size_t prefix(int bits, reg r, const rm& rm);
        size_t modrm(reg r, const rm& rm);

        size_t immop(int op, int bits, const rm& dest, i32 imm);
        size_t aluop(int op, int bits, const rm& dest, const rm& src);
        size_t shift(int op, int bits, const rm& dest, i8 imm);
        size_t branch(int op, i32 imm, fixup* fix);

        // disabled
        emitter();
        emitter(const emitter&);

    public:
        emitter(cbuf& buffer);
        virtual ~emitter();

        size_t ret();

        size_t push(reg src);
        size_t pop(reg dest);

        size_t movi(int bits, const rm& dest, i64 imm);
        size_t addi(int bits, const rm& dest, i32 imm);
        size_t ori (int bits, const rm& dest, i32 imm);
        size_t adci(int bits, const rm& dest, i32 imm);
        size_t sbbi(int bits, const rm& dest, i32 imm);
        size_t andi(int bits, const rm& dest, i32 imm);
        size_t subi(int bits, const rm& dest, i32 imm);
        size_t xori(int bits, const rm& dest, i32 imm);
        size_t cmpi(int bits, const rm& dest, i32 imm);
        size_t tsti(int bits, const rm& dest, i32 imm);

        size_t movr(int bits, const rm& dest, const rm& src);
        size_t addr(int bits, const rm& dest, const rm& src);
        size_t orr (int bits, const rm& dest, const rm& src);
        size_t adcr(int bits, const rm& dest, const rm& src);
        size_t sbbr(int bits, const rm& dest, const rm& src);
        size_t andr(int bits, const rm& dest, const rm& src);
        size_t subr(int bits, const rm& dest, const rm& src);
        size_t xorr(int bits, const rm& dest, const rm& src);
        size_t cmpr(int bits, const rm& dest, const rm& src);
        size_t tstr(int bits, const rm& dest, const rm& src);

        size_t notr(int bits, const rm& op);
        size_t negr(int bits, const rm& op);
        size_t mulr(int bits, const rm& op);
        size_t imul(int bits, const rm& op);
        size_t divr(int bits, const rm& op);
        size_t idiv(int bits, const rm& op);

        size_t imuli(int bits, reg dest, const rm& src, i32 imm);
        size_t imulr(int bits, reg dest, const rm& src);

        size_t roli(int bits, const rm& dest, i8 imm);
        size_t rori(int bits, const rm& dest, i8 imm);
        size_t rcli(int bits, const rm& dest, i8 imm);
        size_t rcri(int bits, const rm& dest, i8 imm);
        size_t shli(int bits, const rm& dest, i8 imm);
        size_t shri(int bits, const rm& dest, i8 imm);
        size_t sari(int bits, const rm& dest, i8 imm);

        size_t call(u8* fn, fixup* fix = NULL);
        size_t call(const rm& dest);

        size_t jmpi(i32 offset, fixup* fix = NULL);
        size_t jmpr(const rm& dest);

        size_t jo(i32 offset, fixup* fix = NULL);
        size_t jno(i32 offset, fixup* fix = NULL);
        size_t jb(i32 offset, fixup* fix = NULL);
        size_t jae(i32 offset, fixup* fix = NULL);
        size_t jz(i32 offset, fixup* fix = NULL);
        size_t jnz(i32 offset, fixup* fix = NULL);
        size_t je(i32 offset, fixup* fix = NULL);
        size_t jne(i32 offset, fixup* fix = NULL);
        size_t jbe(i32 offset, fixup* fix = NULL);
        size_t ja(i32 offset, fixup* fix = NULL);
        size_t js(i32 offset, fixup* fix = NULL);
        size_t jns(i32 offset, fixup* fix = NULL);
        size_t jp(i32 offset, fixup* fix = NULL);
        size_t jnp(i32 offset, fixup* fix = NULL);
        size_t jl(i32 offset, fixup* fix = NULL);
        size_t jge(i32 offset, fixup* fix = NULL);
        size_t jle(i32 offset, fixup* fix = NULL);
        size_t jg(i32 offset, fixup* fix = NULL);
    };

}

#endif
