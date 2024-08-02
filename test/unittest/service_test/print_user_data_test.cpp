/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
#include <vector>
#include <string>
#include <map>
#include "printer_info.h"
#include "iprint_callback.h"
#define private public
#include "print_user_data.h"
#undef private
#include "print_constant.h"
#include "print_log.h"
#include "mock_print_callback_stub.h"

using namespace testing::ext;

namespace OHOS {
namespace Print {

class PrintUserDataTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void PrintUserDataTest::SetUpTestCase(void) {}

void PrintUserDataTest::TearDownTestCase(void) {}

void PrintUserDataTest::SetUp(void)
{
    static int32_t testNo = 0;
    PRINT_HILOGI("PrintUserDataTest_%{public}d", ++testNo);
}

void PrintUserDataTest::TearDown(void) {}

/**
 * @tc.name: PrintServiceStubTest_0001
 * @tc.desc: Verify the capability function.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(PrintUserDataTest, PrintUserDataTest_0001, TestSize.Level1)
{
    std::string type = "111";
    sptr<IPrintCallback> listener = nullptr;
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    userData->RegisterPrinterCallback(type, listener);
    EXPECT_EQ(userData->registeredListeners_.size(), 1);
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0002, TestSize.Level1)
{
    std::string type = "111";
    sptr<IPrintCallback> listener = nullptr;
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    userData->RegisterPrinterCallback(type, listener);
    userData->UnregisterPrinterCallback(type);
    EXPECT_EQ(userData->registeredListeners_.size(), 0);
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0003, TestSize.Level1)
{
    std::string type = "111";
    std::string type2 = "222";
    sptr<IPrintCallback> listener = nullptr;
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    userData->RegisterPrinterCallback(type, listener);
    userData->UnregisterPrinterCallback(type2);
    EXPECT_EQ(userData->registeredListeners_.size(), 1);
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0004, TestSize.Level1)
{
    std::string type = "111";
    sptr<IPrintCallback> listener = nullptr;
    PrinterInfo info;
    int event = 0;
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    userData->RegisterPrinterCallback(type, listener);
    userData->SendPrinterEvent(type, event, info);
    EXPECT_EQ(userData->registeredListeners_.size(), 1);
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0005, TestSize.Level1)
{
    std::string type = "111";
    std::string type2 = "222";
    sptr<IPrintCallback> listener = nullptr;
    PrinterInfo info;
    int event = 0;
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    userData->RegisterPrinterCallback(type, listener);
    userData->SendPrinterEvent(type2, event, info);
    EXPECT_EQ(userData->registeredListeners_.size(), 1);
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0006, TestSize.Level1)
{
    std::string type = "111";
    std::string type2 = "222";
    sptr<IPrintCallback> listener = nullptr;
    sptr<IPrintCallback> newListener = nullptr;
    PrinterInfo info;
    int event = 0;
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    userData->RegisterPrinterCallback(type, listener);
    userData->RegisterPrinterCallback(type2, newListener);
    userData->SendPrinterEvent(type, event, info);
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0007, TestSize.Level1)
{
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    std::string jobId = "1";
    PrintJob findPrintJob;
    EXPECT_EQ(userData->QueryPrintJobById(jobId, findPrintJob), E_PRINT_INVALID_PRINTJOB);
    std::shared_ptr<PrintJob> printJob = std::make_shared<PrintJob>();
    userData->AddToPrintJobList(jobId, printJob);
    EXPECT_EQ(userData->QueryPrintJobById(jobId, findPrintJob), E_PRINT_NONE);
    jobId = "2";
    EXPECT_EQ(userData->QueryPrintJobById(jobId, findPrintJob), E_PRINT_INVALID_PRINTJOB);
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0008, TestSize.Level1)
{
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    std::string jobOrderId = "0";
    std::string jobId = "1";
    std::shared_ptr<PrintJob> printJob = std::make_shared<PrintJob>();
    userData->AddToPrintJobList(jobId, printJob);
    std::string newJobId = "2";
    userData->UpdateQueuedJobList(newJobId, printJob, jobOrderId);
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0009, TestSize.Level1)
{
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    std::string jobOrderId = "0";
    std::string jobId = "1";
    std::shared_ptr<PrintJob> printJob = std::make_shared<PrintJob>();
    userData->AddToPrintJobList(jobId, printJob);
    userData->UpdateQueuedJobList(jobId, printJob, jobOrderId);
    std::vector<PrintJob> printJobs;
    EXPECT_EQ(userData->QueryAllPrintJob(printJobs), E_PRINT_NONE);
    printJob = nullptr;
    userData->UpdateQueuedJobList(jobId, printJob, jobOrderId);
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0010, TestSize.Level1)
{
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    std::string type = "111";
    sptr<IPrintCallback> listener = new (std::nothrow) DummyPrintCallbackStub();
    PrinterInfo info;
    int event = 0;
    userData->RegisterPrinterCallback(type, listener);
    userData->SendPrinterEvent(type, event, info);
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0011, TestSize.Level1)
{
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    std::string jobOrderId = "0";
    std::string jobId = "1";
    std::shared_ptr<PrintJob> printJob = std::make_shared<PrintJob>();
    userData->AddToPrintJobList(jobId, printJob);
    userData->UpdateQueuedJobList(jobId, printJob, jobOrderId);
    userData->AddToPrintJobList(jobId, printJob);
    userData->UpdateQueuedJobList(jobId, printJob, jobOrderId);
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0012, TestSize.Level1)
{
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    std::string jobOrderId = "0";
    std::string jobId = "1";
    std::shared_ptr<PrintJob> printJob = std::make_shared<PrintJob>();
    userData->jobOrderList_.insert(std::make_pair(jobOrderId, jobId));
    std::vector<PrintJob> printJobs;
    userData->QueryAllPrintJob(printJobs);
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0013, TestSize.Level1)
{
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    int32_t userId = 101;
    userData->SetUserId(userId);
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0014, TestSize.Level1)
{
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    std::string printerId = "com.ohos.spooler:p2p://DIRECT-HUAWEI_PixLab_V1-0105";
    userData->SetLastUsedPrinter(printerId);
    std::string printerId2 = "";
    userData->SetLastUsedPrinter(printerId2);
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0015, TestSize.Level1)
{
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    EXPECT_EQ(userData->GetDefaultPrinter(), "");
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0016, TestSize.Level1)
{
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    userData->ParseUserData();
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0017, TestSize.Level1)
{
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    std::string fileData = "";
    userData->GetFileData(fileData);
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0018, TestSize.Level1)
{
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    userData->SetUserDataToFile();
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0019, TestSize.Level1)
{
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    EXPECT_EQ(userData->GetLastUsedPrinter(), "");
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0020, TestSize.Level1)
{
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    std::string printerId = "com.ohos.spooler:p2p://DIRECT-HUAWEI_PixLab_V1-0105";
    userData->SetDefaultPrinter(printerId, 0);
    std::string printerId2 = "";
    userData->SetDefaultPrinter(printerId2, 0);
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0021, TestSize.Level1)
{
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    nlohmann::json jsonObject;
    userData->ParseUserDataFromJson(jsonObject);
    nlohmann::json userDataList;
    jsonObject["print_user_data"] = userDataList;
    userData->ParseUserDataFromJson(jsonObject);

    nlohmann::json jsonObject2;
    nlohmann::json userDataList2;
    userData->userId_ = 100;
    nlohmann::json userData2 = nlohmann::json::array();
    userDataList2["100"] = userData2;
    jsonObject2["print_user_data"] = userDataList2;
    userData->ParseUserDataFromJson(jsonObject2);

    nlohmann::json jsonObject3;
    nlohmann::json userDataList3;
    nlohmann::json userData3 = nlohmann::json::object();
    userDataList3["100"] = userData3;
    jsonObject3["print_user_data"] = userDataList3;
    userData->ParseUserDataFromJson(jsonObject3);
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0022, TestSize.Level1)
{
    auto printUserData = std::make_shared<OHOS::Print::PrintUserData>();
    nlohmann::json jsonObject;
    nlohmann::json userDataList;
    printUserData->userId_ = 100;
    nlohmann::json userData = nlohmann::json::object();
    userData["defaultPrinter"] = 123;
    userDataList["100"] = userData;
    jsonObject["print_user_data"] = userDataList;
    printUserData->ParseUserDataFromJson(jsonObject);

    nlohmann::json jsonObject2;
    nlohmann::json userDataList2;
    nlohmann::json userData2 = nlohmann::json::object();
    userData2["defaultPrinter"] = "123";
    userDataList2["100"] = userData2;
    jsonObject2["print_user_data"] = userDataList2;
    printUserData->ParseUserDataFromJson(jsonObject2);

    nlohmann::json jsonObject3;
    nlohmann::json userDataList3;
    nlohmann::json userData3 = nlohmann::json::object();
    userData3["defaultPrinter"] = "123";
    userData3["lastUsedPrinter"] = 123;
    userDataList3["100"] = userData3;
    jsonObject3["print_user_data"] = userDataList3;
    printUserData->ParseUserDataFromJson(jsonObject3);

    nlohmann::json jsonObject4;
    nlohmann::json userDataList4;
    nlohmann::json userData4 = nlohmann::json::object();
    userData4["defaultPrinter"] = "123";
    userData4["lastUsedPrinter"] = "123";
    userDataList4["100"] = userData4;
    jsonObject4["print_user_data"] = userDataList4;
    printUserData->ParseUserDataFromJson(jsonObject4);
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0023, TestSize.Level1)
{
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    nlohmann::json jsonObject;
    std::string fileData0 = "test";
    userData->CheckFileData(fileData0, jsonObject);

    nlohmann::json fileJson;
    fileJson["key"] = "value";
    std::string fileData = fileJson.dump();
    userData->CheckFileData(fileData, jsonObject);

    nlohmann::json fileJson2;
    fileJson2["version"] = 123;
    std::string fileData2 = fileJson2.dump();
    userData->CheckFileData(fileData2, jsonObject);

    nlohmann::json fileJson3;
    fileJson3["version"] = "123";
    std::string fileData3 = fileJson3.dump();
    userData->CheckFileData(fileData3, jsonObject);

    nlohmann::json fileJson4;
    fileJson4["version"] = "v1";
    std::string fileData4 = fileJson4.dump();
    userData->CheckFileData(fileData4, jsonObject);

    nlohmann::json fileJson5;
    fileJson5["version"] = "v1";
    fileJson5["print_user_data"] = "100";
    std::string fileData5 = fileJson5.dump();
    userData->CheckFileData(fileData5, jsonObject);
}

HWTEST_F(PrintUserDataTest, PrintUserDataTest_0030, TestSize.Level1)
{
    std::string printerId1 = "com.ohos.spooler:p2p://DIRECT_PixLab_V1-0105";
    std::string printerId2 = "com.ohos.spooler:p2p://DIRECT_PixLab_V1-0106";
    auto userData = std::make_shared<OHOS::Print::PrintUserData>();
    EXPECT_NE(userData, nullptr);
    userData->SetLastUsedPrinter(printerId1);
    EXPECT_STREQ(userData->GetLastUsedPrinter().c_str(), printerId1.c_str());
    userData->SetDefaultPrinter(printerId2, DEFAULT_PRINTER_TYPE_LAST_USED_PRINTER);
    EXPECT_EQ(userData->CheckIfUseLastUsedPrinterForDefault(), true);
    userData->SetLastUsedPrinter(printerId2);
    userData->SetDefaultPrinter(printerId1, DELETE_DEFAULT_PRINTER);
    userData->DeletePrinter(printerId1);
    userData->DeletePrinter(printerId2);
    userData->SetDefaultPrinter(printerId2, DELETE_LAST_USED_PRINTER);
}
}
}