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

#ifndef FTL_FUNC_H
#define FTL_FUNC_H

#include "ftl/common.h"
#include "ftl/bitops.h"
#include "ftl/error.h"

#include "ftl/reg.h"
#include "ftl/call.h"
#include "ftl/cbuf.h"
#include "ftl/fixup.h"
#include "ftl/label.h"
#include "ftl/value.h"
#include "ftl/alloc.h"
#include "ftl/emitter.h"

namespace ftl {

    class func
    {
    private:
        string  m_name;

        cbuf*   m_bufptr;
        cbuf&   m_buffer;

        emitter m_emitter;
        alloc   m_alloc;

        u8*     m_head;
        u8*     m_code;
        u8*     m_last;

        label   m_entry;
        label   m_exit;

        void gen_prologue_epilogue();

    public:
        const char* name() const { return m_name.c_str(); }
        u8* entry()        const { return m_code; }
        u8* final()        const { return m_last; }

        size_t size() const;
        u8* finish();

        bool is_finished() const { return m_last != nullptr; }

        cbuf&    get_cbuffer()   { return m_buffer; }
        emitter& get_emitter()   { return m_emitter; }
        alloc&   get_alloc()     { return m_alloc; }

        label&   get_prologue()  { return m_entry; }
        label&   get_epilogue()  { return m_exit; }

        func(const string& name, size_t bufsz = 4 * KiB);
        func(const string& name, cbuf& buffer, void* dataptr = nullptr);
        func(func&& other);
        ~func();

        func() = delete;
        func(const func&) = delete;

        i64 exec();
        i64 exec(void* data);

        i64 operator () ()           { return exec(); }
        i64 operator () (void* data) { return exec(data); }

        void set_data_ptr(void* ptr);
        void set_data_ptr_stack();
        void set_data_ptr_heap();
        void set_data_ptr_code();

        label gen_label(const string& name);

        value gen_local_val(const string& name, int bits, reg r = NREGS);
        value gen_local_i8 (const string& name, reg r = NREGS);
        value gen_local_i16(const string& name, reg r = NREGS);
        value gen_local_i32(const string& name, reg r = NREGS);
        value gen_local_i64(const string& name, reg r = NREGS);

        value gen_local_val(const string& nm, int bits, i64 v, reg r = NREGS);
        value gen_local_i8 (const string& nm, i8  val, reg r = NREGS);
        value gen_local_i16(const string& nm, i16 val, reg r = NREGS);
        value gen_local_i32(const string& nm, i32 val, reg r = NREGS);
        value gen_local_i64(const string& nm, i64 val, reg r = NREGS);

        value gen_global_val(const string& name, int bits, void* addr);
        value gen_global_i8 (const string& name, void* addr);
        value gen_global_i16(const string& name, void* addr);
        value gen_global_i32(const string& name, void* addr);
        value gen_global_i64(const string& name, void* addr);

        value gen_scratch_val(const string& name, int bits, reg r = NREGS);
        value gen_scratch_i8 (const string& name, reg r = NREGS);
        value gen_scratch_i16(const string& name, reg r = NREGS);
        value gen_scratch_i32(const string& name, reg r = NREGS);
        value gen_scratch_i64(const string& name, reg r = NREGS);

        value gen_scratch_val(const string& nm, int bits, i64 v, reg r = NREGS);
        value gen_scratch_i8 (const string& nm, i8  val, reg r = NREGS);
        value gen_scratch_i16(const string& nm, i16 val, reg r = NREGS);
        value gen_scratch_i32(const string& nm, i32 val, reg r = NREGS);
        value gen_scratch_i64(const string& nm, i64 val, reg r = NREGS);

        scalar gen_local_fp (const string& name, int bits, xmm r = NXMM);
        scalar gen_local_f32(const string& name, xmm r = NXMM);
        scalar gen_local_f64(const string& name, xmm r = NXMM);

        scalar gen_local_fp (const string& nm, int bits, f64 f, xmm r = NXMM);
        scalar gen_local_f32(const string& nm, f32 val, xmm r = NXMM);
        scalar gen_local_f64(const string& nm, f64 val, xmm r = NXMM);

        scalar gen_global_fp (const string& name, int bits, void* addr);
        scalar gen_global_f32(const string& name, void* addr);
        scalar gen_global_f64(const string& name, void* addr);

        scalar gen_scratch_fp (const string& name, int bits, xmm r = NXMM);
        scalar gen_scratch_f32(const string& name, xmm r = NXMM);
        scalar gen_scratch_f64(const string& name, xmm r = NXMM);

        scalar gen_scratch_fp (const string& nm, int bits, f64 f, xmm r = NXMM);
        scalar gen_scratch_f32(const string& nm, f32 val, xmm r = NXMM);
        scalar gen_scratch_f64(const string& nm, f64 val, xmm r = NXMM);

        void free_value(value& val);

        func gen_function(const string& name);
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

        void gen_seto(value& dest);
        void gen_setno(value& dest);
        void gen_setb(value& dest);
        void gen_setae(value& dest);
        void gen_setz(value& dest);
        void gen_setnz(value& dest);
        void gen_sete(value& dest);
        void gen_setne(value& dest);
        void gen_setbe(value& dest);
        void gen_seta(value& dest);
        void gen_sets(value& dest);
        void gen_setns(value& dest);
        void gen_setp(value& dest);
        void gen_setnp(value& dest);
        void gen_setl(value& dest);
        void gen_setge(value& dest);
        void gen_setle(value& dest);
        void gen_setg(value& dest);

        void gen_cmovo (value& dest, const value& src);
        void gen_cmovno(value& dest, const value& src);
        void gen_cmovb (value& dest, const value& src);
        void gen_cmovae(value& dest, const value& src);
        void gen_cmovz (value& dest, const value& src);
        void gen_cmovnz(value& dest, const value& src);
        void gen_cmove (value& dest, const value& src);
        void gen_cmovne(value& dest, const value& src);
        void gen_cmovbe(value& dest, const value& src);
        void gen_cmova (value& dest, const value& src);
        void gen_cmovs (value& dest, const value& src);
        void gen_cmovns(value& dest, const value& src);
        void gen_cmovp (value& dest, const value& src);
        void gen_cmovnp(value& dest, const value& src);
        void gen_cmovl (value& dest, const value& src);
        void gen_cmovge(value& dest, const value& src);
        void gen_cmovle(value& dest, const value& src);
        void gen_cmovg (value& dest, const value& src);

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
        void gen_xchg(value& dest, value& src);

        void gen_add(value& dest, const value& src1, const value& src2);
        void gen_or (value& dest, const value& src1, const value& src2);
        void gen_adc(value& dest, const value& src1, const value& src2);
        void gen_sbb(value& dest, const value& src1, const value& src2);
        void gen_and(value& dest, const value& src1, const value& src2);
        void gen_sub(value& dest, const value& src1, const value& src2);
        void gen_xor(value& dest, const value& src1, const value& src2);

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

        void gen_lea(value& dest,       value& src, i32 val);
        void gen_add(value& dest, const value& src, i32 val);
        void gen_or (value& dest, const value& src, i32 val);
        void gen_adc(value& dest, const value& src, i32 val);
        void gen_sbb(value& dest, const value& src, i32 val);
        void gen_and(value& dest, const value& src, i32 val);
        void gen_sub(value& dest, const value& src, i32 val);
        void gen_xor(value& dest, const value& src, i32 val);

        void gen_imul(value& hi, value& lo, const value& src);
        void gen_umul(value& hi, value& lo, const value& src);
        void gen_imul(value& dest, const value& src);
        void gen_umul(value& dest, const value& src);
        void gen_idiv(value& dest, const value& src);
        void gen_udiv(value& dest, const value& src);
        void gen_imod(value& dest, const value& src);
        void gen_umod(value& dest, const value& src);

        void gen_imul(value& dest, i64 val);
        void gen_idiv(value& dest, i64 val);
        void gen_imod(value& dest, i64 val);
        void gen_umul(value& dest, u64 val);
        void gen_udiv(value& dest, u64 val);
        void gen_umod(value& dest, u64 val);

        void gen_inc(value& dest);
        void gen_dec(value& dest);
        void gen_not(value& dest);
        void gen_neg(value& dest);

        void gen_shl(value& dest, value& src);
        void gen_shr(value& dest, value& src);
        void gen_sha(value& dest, value& src);
        void gen_rol(value& dest, value& src);
        void gen_ror(value& dest, value& src);

        void gen_shl(value& dest, u8 shift);
        void gen_shr(value& dest, u8 shift);
        void gen_sha(value& dest, u8 shift);
        void gen_rol(value& dest, u8 shift);
        void gen_ror(value& dest, u8 shift);

        void gen_bt (value& dest, value& src);
        void gen_bts(value& dest, value& src);
        void gen_btr(value& dest, value& src);
        void gen_btc(value& dest, value& src);

        void gen_bt (value& dest, u8 idx);
        void gen_bts(value& dest, u8 idx);
        void gen_btr(value& dest, u8 idx);
        void gen_btc(value& dest, u8 idx);

        void gen_zxt(value& dest, value& src);
        void gen_zxt(value& dest, value& src, int dbits, int sbits);
        void gen_sxt(value& dest, value& src);
        void gen_sxt(value& dest, value& src, int dbits, int sbits);

        void gen_cmpxchg(value& dest, value& src, value& cmpv);
        void gen_fence(bool sync_loads = true, bool sync_stores = true);

        void gen_mov(scalar& dest, const value& src);
        void gen_mov(value& dest, scalar& src);

        void gen_mov(scalar& dest, const scalar& src);
        void gen_add(scalar& dest, const scalar& src);
        void gen_sub(scalar& dest, const scalar& src);
        void gen_mul(scalar& dest, const scalar& src);
        void gen_div(scalar& dest, const scalar& src);
        void gen_min(scalar& dest, const scalar& src);
        void gen_max(scalar& dest, const scalar& src);
        void gen_sqrt(scalar& dest, const scalar& src);
        void gen_pxor(scalar& dest, const scalar& src);
        void gen_cmp(scalar& op1, const scalar& op2, bool signal_qnan = false);
        void gen_cvt(scalar& dest, const value& src);
        void gen_cvt(value& dest, const scalar& src);

        template <typename FUNC>
        value gen_call(FUNC* fn);

        template <typename FUNC, typename T1>
        value gen_call(FUNC* fn, const T1& arg1);

        template <typename FUNC, typename T1, typename T2>
        value gen_call(FUNC* fn, const T1& arg1, const T2& arg2);

        template <typename FUNC, typename T1, typename T2, typename T3>
        value gen_call(FUNC* fn, const T1& arg1, const T2& arg2,
                       const T3& arg3);

        template <typename FUNC, typename T1, typename T2, typename T3,
                  typename T4>
        value gen_call(FUNC* fn, const T1& arg1, const T2& arg2,
                       const T3& arg3, const T4& arg4);

        template <typename FUNC, typename T1, typename T2, typename T3,
                  typename T4, typename T5>
        value gen_call(FUNC* fn, const T1& arg1, const T2& arg2,
                       const T3& arg3, const T4& arg4, const T5& arg5);
    };

    inline size_t func::size() const {
        FTL_ERROR_ON(!is_finished(), "function '%s' not finished", name());
        return m_last - m_code;
    }

    inline u8* func::finish() {
        return m_last = m_buffer.get_code_ptr();
    }

    inline label func::gen_label(const string& name) {
        return label(name, m_buffer, m_alloc);
    }

    inline value func::gen_local_val(const string& name, int bits, reg r) {
        return m_alloc.new_local_noinit(name, bits, r);
    }

    inline value func::gen_local_i8 (const string& name, reg r) {
        return gen_local_val(name, 8, r);
    }

    inline value func::gen_local_i16(const string& name, reg r) {
        return gen_local_val(name, 16, r);
    }

    inline value func::gen_local_i32(const string& name, reg r) {
        return gen_local_val(name, 32, r);
    }

    inline value func::gen_local_i64(const string& name, reg r) {
        return gen_local_val(name, 64, r);
    }

    inline value func::gen_local_val(const string& nm, int w, i64 v, reg r) {
        FTL_ERROR_ON(encode_size(v) > w, "initialization value too large");
        return m_alloc.new_local(nm, w, v, r);
    }

    inline value func::gen_local_i8(const string& name, i8 val, reg r) {
        return gen_local_val(name, 8, val, r);
    }

    inline value func::gen_local_i16(const string& name, i16 val, reg r) {
        return gen_local_val(name, 16, val, r);
    }

    inline value func::gen_local_i32(const string& name, i32 val, reg r) {
        return gen_local_val(name, 32, val, r);
    }

    inline value func::gen_local_i64(const string& name, i64 val, reg r) {
        return gen_local_val(name, 64, val, r);
    }

    inline value func::gen_global_val(const string& nm, int bits, void* addr) {
        return m_alloc.new_global(nm, bits, (u64)addr);
    }

    inline value func::gen_global_i8(const string& name, void* addr) {
        return gen_global_val(name, 8, addr);
    }

    inline value func::gen_global_i16(const string& name, void* addr) {
        return gen_global_val(name, 16, addr);
    }

    inline value func::gen_global_i32(const string& name, void* addr) {
        return gen_global_val(name, 32, addr);
    }

    inline value func::gen_global_i64(const string& name, void* addr) {
        return gen_global_val(name, 64, addr);
    }

    inline value func::gen_scratch_val(const string& name, int bits, reg r) {
        return m_alloc.new_scratch_noinit(name, bits, r);
    }

    inline value func::gen_scratch_i8 (const string& name, reg r) {
        return gen_scratch_val(name, 8, r);
    }

    inline value func::gen_scratch_i16(const string& name, reg r) {
        return gen_scratch_val(name, 16, r);
    }

    inline value func::gen_scratch_i32(const string& name, reg r) {
        return gen_scratch_val(name, 32, r);
    }

    inline value func::gen_scratch_i64(const string& name, reg r) {
        return gen_scratch_val(name, 64, r);
    }

    inline value func::gen_scratch_val(const string& nm, int w, i64 v, reg r) {
        FTL_ERROR_ON(encode_size(v) > w, "initialization value too large");
        return m_alloc.new_scratch(nm, w, v, r);
    }

    inline value func::gen_scratch_i8(const string& name, i8 val, reg r) {
        return gen_scratch_val(name, 8, val, r);
    }

    inline value func::gen_scratch_i16(const string& name, i16 val, reg r) {
        return gen_scratch_val(name, 16, val, r);
    }

    inline value func::gen_scratch_i32(const string& name, i32 val, reg r) {
        return gen_scratch_val(name, 32, val, r);
    }

    inline value func::gen_scratch_i64(const string& name, i64 val, reg r) {
        return gen_scratch_val(name, 64, val, r);
    }

    inline scalar func::gen_local_fp (const string& name, int bits, xmm r) {
        return m_alloc.new_local_scalar_noinit(name, bits, r);
    }

    inline scalar func::gen_local_f32(const string& name, xmm r) {
        return gen_local_fp(name, 32, r);
    }

    inline scalar func::gen_local_f64(const string& name, xmm r) {
        return gen_local_fp(name, 64, r);
    }

    inline scalar func::gen_local_fp(const string& n, int bits, f64 f, xmm r) {
        return m_alloc.new_local_scalar(n, bits, f, r);
    }

    inline scalar func::gen_local_f32(const string& nm, f32 f, xmm r) {
        return gen_local_fp(nm, 32, f, r);
    }

    inline scalar func::gen_local_f64(const string& nm, f64 f, xmm r) {
        return gen_local_fp(nm, 64, f, r);
    }

    inline scalar func::gen_global_fp(const string& nm, int bits, void* addr) {
        return m_alloc.new_global_scalar(nm, bits, (u64)addr);
    }

    inline scalar func::gen_global_f32(const string& name, void* addr) {
        return gen_global_fp(name, 32, addr);
    }

    inline scalar func::gen_global_f64(const string& name, void* addr) {
        return gen_global_fp(name, 64, addr);
    }

    inline scalar func::gen_scratch_fp (const string& name, int bits, xmm r) {
        return m_alloc.new_scratch_scalar_noinit(name, bits, r);
    }

    inline scalar func::gen_scratch_f32(const string& name, xmm r) {
        return gen_scratch_fp(name, 32, r);
    }

    inline scalar func::gen_scratch_f64(const string& name, xmm r) {
        return gen_scratch_fp(name, 64, r);
    }

    inline scalar func::gen_scratch_fp(const string& n, int w, f64 f, xmm r) {
        return m_alloc.new_scratch_scalar(n, w, f, r);
    }

    inline scalar func::gen_scratch_f32(const string& name, f32 f, xmm r) {
        return gen_scratch_fp(name, 32, f, r);
    }

    inline scalar func::gen_scratch_f64(const string& name, f64 f, xmm r) {
        return gen_scratch_fp(name, 64, f, r);
    }

    inline void func::free_value(value& val) {
        m_alloc.free_value(val);
    }

    template <typename FUNC>
    inline value func::gen_call(FUNC* fn) {
        m_alloc.flush_volatile_regs();
        m_alloc.store_all_regs();
        m_emitter.movr(64, argreg(0), BASE_POINTER);

        if (can_call_directly(m_buffer.get_code_ptr(), fn)) {
            m_emitter.call((u8*)fn);
            value ret = gen_scratch_i64("retval", RAX);
            m_alloc.mark_dirty(RAX);
            return ret;
        } else {
            value ret = gen_scratch_i64("retval", (i64)fn, RAX);
            m_emitter.call(RAX);
            m_alloc.mark_dirty(RAX);
            return ret;
        }
    }

    template <typename FUNC, typename T1>
    inline value func::gen_call(FUNC* fn, const T1& arg1) {
        arg<T1>::fetch(m_alloc, arg1);
        return gen_call(fn);
    }

    template <typename FUNC, typename T1, typename T2>
    inline value func::gen_call(FUNC* fn, const T1& arg1, const T2& arg2) {
        arg<T2>::template fetch<T1>(m_alloc, arg2);
        return gen_call(fn, arg1);
    }

    template <typename FUNC, typename T1, typename T2, typename T3>
    inline value func::gen_call(FUNC* fn, const T1& arg1, const T2& arg2,
                                const T3& arg3) {
        arg<T3>::template fetch<T1,T2>(m_alloc, arg3);
        return gen_call(fn, arg1, arg2);
    }

    template <typename FUNC, typename T1, typename T2, typename T3,
              typename T4>
    inline value func::gen_call(FUNC* fn, const T1& arg1, const T2& arg2,
                                const T3& arg3, const T4& arg4) {
        arg<T4>::template fetch<T1,T2,T3>(m_alloc, arg4);
        return gen_call(fn, arg1, arg2, arg3);
    }

    template <typename FUNC, typename T1, typename T2, typename T3,
              typename T4, typename T5>
    inline value func::gen_call(FUNC* fn, const T1& arg1, const T2& arg2,
                                const T3& arg3, const T4& arg4,
                                const T5& arg5) {
        arg<T5>::template fetch<T1,T2,T3,T4>(m_alloc, arg5);
        return gen_call(fn, arg1, arg2, arg3, arg4);
    }

    static inline i64 invoke(const cbuf& buffer, void* code, void* data) {
        typedef i64 func_t (void* code, void* data);
        func_t* fn = (func_t*)buffer.get_code_entry();
        return fn(code, data);
    }

}

#endif
