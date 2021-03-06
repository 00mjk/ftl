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

#include <gtest/gtest.h>

#include "ftl.h"

using namespace ftl;

TEST(bitops, fits) {
    size_t val0 = 0;
    EXPECT_TRUE(fits_i8(val0));

    size_t val1 = 127;
    EXPECT_TRUE(fits_i8(val1));

    size_t val2 = 128;
    EXPECT_FALSE(fits_i8(val2));

    size_t val3 = -128;
    EXPECT_TRUE(fits_i8(val3));

    size_t val4 = -128;
    EXPECT_TRUE(fits_i8(val4));
}

TEST(bitops, encode_size) {
    EXPECT_EQ(encode_size(127),  8);
    EXPECT_EQ(encode_size(127u), 8);
    EXPECT_EQ(encode_size(128), 16);
    EXPECT_EQ(encode_size(128u), 8);

    EXPECT_EQ(encode_size<i32>(127),  8);
    EXPECT_EQ(encode_size<i64>(127),  8);
    EXPECT_EQ(encode_size<u32>(127),  8);
    EXPECT_EQ(encode_size<u64>(127),  8);

    EXPECT_EQ(encode_size<i32>(127u), 8);
    EXPECT_EQ(encode_size<i64>(127u), 8);
    EXPECT_EQ(encode_size<u32>(127u), 8);
    EXPECT_EQ(encode_size<u64>(127u), 8);

    EXPECT_EQ(encode_size<i32>(128), 16);
    EXPECT_EQ(encode_size<u32>(128),  8);
    EXPECT_EQ(encode_size<i64>(128), 16);
    EXPECT_EQ(encode_size<u64>(128),  8);

    EXPECT_EQ(encode_size<i32>(128u), 16);
    EXPECT_EQ(encode_size<u32>(128u),  8);
    EXPECT_EQ(encode_size<i64>(128u), 16);
    EXPECT_EQ(encode_size<u64>(128u),  8);

    EXPECT_EQ(encode_size<i64>(0x80000000), 64);
    EXPECT_EQ(encode_size<u64>(0x80000000), 32);
}

TEST(bitops, pow2) {
    EXPECT_FALSE(is_pow2(0));
    EXPECT_FALSE(is_pow2(255));
    EXPECT_FALSE(is_pow2(1020));
    EXPECT_FALSE(is_pow2(779632));

    EXPECT_TRUE(is_pow2(1));
    EXPECT_TRUE(is_pow2(2));
    EXPECT_TRUE(is_pow2(32));
    EXPECT_TRUE(is_pow2(4096));
    EXPECT_TRUE(is_pow2(65536));
    EXPECT_TRUE(is_pow2(2097152));
    EXPECT_TRUE(is_pow2(274877906944));
}

TEST(bitops, log2i) {
    EXPECT_EQ(log2i(2), 1);
    EXPECT_EQ(log2i(4), 2);
    EXPECT_EQ(log2i(8), 3);
    EXPECT_EQ(log2i(16), 4);
    EXPECT_EQ(log2i(32), 5);
    EXPECT_EQ(log2i(64), 6);
    EXPECT_EQ(log2i(128), 7);
    EXPECT_EQ(log2i(256), 8);
    EXPECT_EQ(log2i(512), 9);
    EXPECT_EQ(log2i(1024), 10);
    EXPECT_EQ(log2i(2048), 11);
    EXPECT_EQ(log2i(4096), 12);
    EXPECT_EQ(log2i(8192), 13);
    EXPECT_EQ(log2i(16384), 14);
    EXPECT_EQ(log2i(32768), 15);
}
