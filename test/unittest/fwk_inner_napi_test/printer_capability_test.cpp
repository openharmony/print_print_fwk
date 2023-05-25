/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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
#include "napi/native_api.h"
#include "printer_capability.h"
#include "print_margin.h"
#include "print_resolution.h"

using namespace testing::ext;

namespace OHOS {
namespace Print {
class PrinterCapabilityTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void PrinterCapabilityTest::SetUpTestCase(void) {}

void PrinterCapabilityTest::TearDownTestCase(void) {}

void PrinterCapabilityTest::SetUp(void) {}

void PrinterCapabilityTest::TearDown(void) {}

/**
 * @tc.name: PrinterCapabilityTest_001
 * @tc.desc: Verify the colormode function.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PrinterCapabilityTest, PrinterCapabilityTest_001, TestSize.Level1)
{
    OHOS::Print::PrinterCapability capability;
    OHOS::Print::PrinterCapability(capability_);
    OHOS::Print::PrintMargin margin, getMargin;
    std::vector<OHOS::Print::PrintResolution> resolutionList;
    napi_env env = nullptr;
    napi_value val = nullptr;
    std::vector<PrintPageSize> pagesize, getPagesize;
    capability.SetMinMargin(margin);
    capability.SetPageSize(pagesize);
    capability.SetResolution(resolutionList);
    capability.GetMinMargin(getMargin);
    capability.GetPageSize(getPagesize);
    capability.ToJsObject(env);
    capability.Dump();
    capability.Reset();
    OHOS::Print::PrinterCapability capability_val = capability;
    OHOS::Print::PrinterCapability::BuildFromJs(env, val);
    OHOS::Print::PrinterCapability::ValidateProperty(env, val);
    EXPECT_EQ(0, capability.GetColorMode());
}

/**
 * @tc.name: PrinterCapabilityTest_002
 * @tc.desc: Verify the duplexmode function.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PrinterCapabilityTest, PrinterCapabilityTest_002, TestSize.Level1)
{
    OHOS::Print::PrinterCapability capability;
    capability.Reset();
    EXPECT_EQ(0, capability.GetDuplexMode());
}

/**
 * @tc.name: PrinterCapabilityTest_003
 * @tc.desc: Verify the colormode function.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PrinterCapabilityTest, PrinterCapabilityTest_003, TestSize.Level1)
{
    OHOS::Print::PrinterCapability capability;
    OHOS::Print::PrintMargin margin, getMargin;
    std::vector<OHOS::Print::PrintResolution> resolutionList;
    std::vector<PrintPageSize> pagesize, getPagesize;
    capability.SetMinMargin(margin);
    capability.SetPageSize(pagesize);
    capability.SetResolution(resolutionList);
    capability.GetMinMargin(getMargin);
    capability.GetPageSize(getPagesize);
    Parcel parcel;
    capability.Marshalling(parcel);
    capability.ReadFromParcel(parcel);
    capability.Unmarshalling(parcel);
}
} // namespace Print
} // namespace OHOS