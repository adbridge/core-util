/*
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed-drivers/mbed.h"
#include "mbed-drivers/test_env.h"
#include "core-util/FunctionPointer.h"
#include "int_types.hpp"
#include "lifetime.hpp"
#include "side_effects.hpp"

namespace {
volatile int ebp_flag = 0;
volatile int ecp_flag = 0;
}

class VTest {
public:
    void print() {
        ecp_flag = 1;
        printf("Class Print\r\n");
    }

};

void bareprint() {
    ebp_flag = 1;
    printf("Bare Print\r\n");
}

void runTest(void) {
    MBED_HOSTTEST_TIMEOUT(10);
    MBED_HOSTTEST_SELECT(default_auto);
    MBED_HOSTTEST_DESCRIPTION(FunctionPointer test);
    MBED_HOSTTEST_START("FPOINTER_1");

    VTest test;
    printf("Testing mbed FunctionPointer...\r\n");

    mbed::util::FunctionPointer ebp(bareprint);
    mbed::util::FunctionPointer ecp(&test, &VTest::print);

    size_t ebsize = sizeof(ebp);
    size_t ecsize = sizeof(ecp);
    printf("sizeof(bp) = %d\r\n", ebsize);
    printf("sizeof(cp) = %d\r\n", ecsize);

    bool result = true;
    // Test checks
    {
        ebp.call();
        result = result && ebp_flag;
        printf("ebp_flag = %d\r\n", ebp_flag);

        ecp.call();
        result = result && ecp_flag;
        printf("ecp_flag = %d\r\n", ecp_flag);
    }
    bool iResult = testInts();
    result = result && iResult;
    bool lifeResult = checkLifetime();
    result = result && lifeResult;
    bool sideEffectResult = checkSideEffects();
    result = result && lifeResult;

    printf("Test Complete\r\n");
    MBED_HOSTTEST_RESULT(result);
}

void app_start(int, char*[]) {
    minar::Scheduler::postCallback(&runTest);
}
