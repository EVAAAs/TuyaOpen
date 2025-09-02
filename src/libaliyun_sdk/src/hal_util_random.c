/*
 * Copyright 2025 Alibaba Group Holding Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http: *www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "c_utils.h"
#include "tal_system.h"
#include <stdlib.h>
#include <time.h>

// Static variable to track initialization state and seed
static uint8_t s_random_initialized = 0;
static uint32_t s_random_seed = 0;

/**
 * 初始化随机数生成器
 *
 * @param seed 用于初始化随机数生成器的种子值
 *
 * @return 返回初始化结果，0表示成功，非0表示失败
 *
 * 此函数通过对随机数生成器进行初始化，以确保后续生成的随机数序列具有良好的随机性
 * 种子值的选择对生成的随机数序列有重要影响，相同的种子值会生成相同的随机数序列
 */
int32_t util_random_init(uint32_t seed)
{
    // Use the provided seed to initialize the C library PRNG
    srandom(seed);
    s_random_seed = seed;
    s_random_initialized = 1;

    UTIL_LOG_D("Random number generator initialized (seed: %u)", seed);
    return UTIL_SUCCESS;
}

/**
 * 生成一个随机数
 *
 * @return 返回生成的随机数
 *
 * 在调用此函数之前，应确保随机数生成器已经通过util_random_init函数成功初始化
 * 此函数生成的随机数是基于初始化时提供的种子值产生的
 */
uint32_t util_random(void)
{
    if (!s_random_initialized) {
        // Auto-initialize with a time-based seed if not already done
        uint32_t auto_seed = (uint32_t)time(NULL);
        util_random_init(auto_seed);
        UTIL_LOG_W("Random generator auto-initialized with seed: %u", auto_seed);
    }

    // Use the standard C library random function to ensure deterministic output for the same seed
    uint32_t random_value = (uint32_t)random();

    return random_value;
}
