/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <gtest/gtest.h>

namespace OHOS {
namespace Scan {
class ScanModuleTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void ScanModuleTest::SetUpTestCase(void) {}

void ScanModuleTest::TearDownTestCase(void) {}

void ScanModuleTest::SetUp(void) {}

void ScanModuleTest::TearDown(void) {}

/**
 * @tc.name: ScanModuleTest_0001
 * @tc.desc: Verify the capability function.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ScanModuleTest, ScanModuleTest_0001, TestSize.Level1) { }
} // namespace Scan
} // namespace OHOS