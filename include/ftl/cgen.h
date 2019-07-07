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

#ifndef FTL_CGEN_H
#define FTL_CGEN_H

#include "ftl/common.h"
#include "ftl/bitops.h"
#include "ftl/error.h"

#include "ftl/reg.h"
#include "ftl/func.h"
#include "ftl/cbuf.h"
#include "ftl/fixup.h"
#include "ftl/label.h"
#include "ftl/value.h"
#include "ftl/alloc.h"
#include "ftl/emitter.h"

namespace ftl {

    class cgen
    {
    private:
        cbuf    m_buffer;
        emitter m_emitter;
        alloc   m_alloc;
        label   m_entry;
        label   m_exit;

        void gen_entry_exit();

        // disabled
        cgen();
        cgen(const cgen&);

    public:
        cgen(size_t size);
        virtual ~cgen();


        cbuf&    get_cbuffer()  { return m_buffer; }
        emitter& get_emitter()  { return m_emitter; }
        alloc&   get_alloc()    { return m_alloc; }

        label&   get_prologue() { return m_entry; }
        label&   get_epilogue() { return m_exit; }

        void reset();

        void set_base_ptr(void* ptr);
        void set_base_ptr_stack();
        void set_base_ptr_heap();
        void set_base_ptr_code();

        label gen_label();

        value gen_local_i8 (i8  val, reg r = NREGS);
        value gen_local_i16(i16 val, reg r = NREGS);
        value gen_local_i32(i32 val, reg r = NREGS);
        value gen_local_i64(i64 val, reg r = NREGS);

        value gen_global_i8 (void* addr);
        value gen_global_i16(void* addr);
        value gen_global_i32(void* addr);
        value gen_global_i64(void* addr);

        void sync_value(value& val);
        void free_value(value& val);

        func gen_function();
        void gen_ret();
        void gen_ret(i64 val);
        void gen_ret(value& val);

        void gen_jmp(label& l, bool far = false);
        void gen_jo(label& l, bool far = false);
        void gen_jno(label& l, bool far = false);
        void gen_jb(label& l, bool far = false);
        void gen_jae(label& l, bool far = false);
        void gen_jz(label& l, bool far = false);
        void gen_jnz(label& l, bool far = false);
        void gen_je(label& l, bool far = false);
        void gen_jne(label& l, bool far = false);
        void gen_jbe(label& l, bool far = false);
        void gen_ja(label& l, bool far = false);
        void gen_js(label& l, bool far = false);
        void gen_jns(label& l, bool far = false);
        void gen_jp(label& l, bool far = false);
        void gen_jnp(label& l, bool far = false);
        void gen_jl(label& l, bool far = false);
        void gen_jge(label& l, bool far = false);
        void gen_jle(label& l, bool far = false);
        void gen_jg(label& l, bool far = false);

        void gen_mov(value& dest, const value& src);
        void gen_add(value& dest, const value& src);
        void gen_or (value& dest, const value& src);
        void gen_adc(value& dest, const value& src);
        void gen_sbb(value& dest, const value& src);
        void gen_and(value& dest, const value& src);
        void gen_sub(value& dest, const value& src);
        void gen_xor(value& dest, const value& src);
        void gen_cmp(value& dest, const value& src);
        void gen_tst(value& dest, const value& src);

        void gen_mov(value& dest, i64 val);
        void gen_add(value& dest, i32 val);
        void gen_or (value& dest, i32 val);
        void gen_adc(value& dest, i32 val);
        void gen_sbb(value& dest, i32 val);
        void gen_and(value& dest, i32 val);
        void gen_sub(value& dest, i32 val);
        void gen_xor(value& dest, i32 val);
        void gen_cmp(value& dest, i32 val);
        void gen_tst(value& dest, i32 val);

        void gen_imul(value& dest, const value& src);
        void gen_idiv(value& dest, const value& src);
        void gen_imod(value& dest, const value& src);
        void gen_umul(value& dest, const value& src);
        void gen_udiv(value& dest, const value& src);
        void gen_umod(value& dest, const value& src);

        void gen_imul(value& dest, i64 val);
        void gen_idiv(value& dest, i64 val);
        void gen_imod(value& dest, i64 val);
        void gen_umul(value& dest, u64 val);
        void gen_udiv(value& dest, u64 val);
        void gen_umod(value& dest, u64 val);

        void gen_not(value& dest);
        void gen_neg(value& dest);

        void gen_shl(value& dest, u8 shift);
        void gen_shr(value& dest, u8 shift);
        void gen_sha(value& dest, u8 shift);
        void gen_rol(value& dest, u8 shift);
        void gen_ror(value& dest, u8 shift);

        typedef i64 (func1)(void* bptr);
        typedef i64 (func2)(void* bptr, i64 arg1);
        typedef i64 (func3)(void* bptr, i64 arg1, i64 arg2);
        typedef i64 (func4)(void* bptr, i64 arg1, i64 arg2, i64 arg3);

        value gen_call(func1* fn);
        value gen_call(func2* fn, value& arg1);
        value gen_call(func3* fn, value& arg1, value& arg2);
        value gen_call(func4* fn, value& arg1, value& arg2, value& arg3);
    };

    inline void cgen::set_base_ptr(void* ptr) {
        m_alloc.set_base_addr((u64)ptr);
    }

    inline label cgen::gen_label() {
        return label(m_buffer);
    }

    inline value cgen::gen_local_i8(i8 val, reg r) {
        return m_alloc.new_local(8, val, r);
    }

    inline value cgen::gen_local_i16(i16 val, reg r) {
        return m_alloc.new_local(16, val, r);
    }

    inline value cgen::gen_local_i32(i32 val, reg r) {
        return m_alloc.new_local(32, val, r);
    }

    inline value cgen::gen_local_i64(i64 val, reg r) {
        return m_alloc.new_local(64, val, r);
    }

    inline value cgen::gen_global_i8(void* addr) {
        return m_alloc.new_global(8, (u64)addr);
    }

    inline value cgen::gen_global_i16(void* addr) {
        return m_alloc.new_global(16, (u64)addr);
    }

    inline value cgen::gen_global_i32(void* addr) {
        return m_alloc.new_global(32, (u64)addr);
    }

    inline value cgen::gen_global_i64(void* addr) {
        return m_alloc.new_global(64, (u64)addr);
    }

    inline void cgen::sync_value(value& val) {
        m_alloc.store(val);
    }

    inline void cgen::free_value(value& val) {
        m_alloc.free_value(val);
    }

}

#endif
