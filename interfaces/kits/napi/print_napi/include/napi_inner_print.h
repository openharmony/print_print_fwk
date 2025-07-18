/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#ifndef NAPI_INNER_PRINT_H
#define NAPI_INNER_PRINT_H

#include <string>
#include <vector>

#include "iprint_callback.h"
#include "napi/native_api.h"
#include "print_async_call.h"
#include "print_extension_info.h"
#include "print_job.h"
#include "print_task.h"
#include "printer_info.h"
#include "print_utils.h"
#include "print_constant.h"
#include "printer_preferences.h"

namespace OHOS::Print {
class NapiInnerPrint {
public:
    static napi_value QueryExtensionInfo(napi_env env, napi_callback_info info);
    static napi_value StartDiscovery(napi_env env, napi_callback_info info);
    static napi_value StopDiscovery(napi_env env, napi_callback_info info);
    static napi_value ConnectPrinter(napi_env env, napi_callback_info info);
    static napi_value DisconnectPrinter(napi_env env, napi_callback_info info);
    static napi_value StartPrintJob(napi_env env, napi_callback_info info);
    static napi_value CancelPrintJob(napi_env env, napi_callback_info info);
    static napi_value RestartPrintJob(napi_env env, napi_callback_info info);
    static napi_value RequestPreview(napi_env env, napi_callback_info info);
    static napi_value QueryCapability(napi_env env, napi_callback_info info);
    static napi_value QueryAllPrintJob(napi_env env, napi_callback_info info);
    static napi_value QueryAllHistoryPrintJob(napi_env env, napi_callback_info info);
    static napi_value QueryPrintJobById(napi_env env, napi_callback_info info);
    static napi_value On(napi_env env, napi_callback_info info);
    static napi_value Off(napi_env env, napi_callback_info info);
    static napi_value StartGetPrintFile(napi_env env, napi_callback_info info);
    static napi_value NotifyPrintService(napi_env env, napi_callback_info info);
    static napi_value QueryAddedPrinter(napi_env env, napi_callback_info info);
    static napi_value QueryPrinterInfoByPrinterId(napi_env env, napi_callback_info info);
    static napi_value NotifyPrintServiceEvent(napi_env env, napi_callback_info info);
    static napi_value SetPrinterPreference(napi_env env, napi_callback_info info);
    static napi_value SetDefaultPrinter(napi_env env, napi_callback_info info);
    static napi_value GetAddedPrinterInfoById(napi_env env, napi_callback_info info);

private:
    static bool IsSupportType(const std::string& type);
    static bool IsValidApplicationEvent(uint32_t event);
    static bool IsValidDefaultPrinterType(uint32_t type);
    static void NapiThrowError(napi_env env, int32_t errCode);
    static bool CheckCallerIsSystemApp();

private:
    struct InnerPrintContext : public PrintAsyncCall::Context {
        std::vector<PrintExtensionInfo> allExtensionInfos;
        std::vector<PrintJob> allPrintJobs;
        std::vector<std::string> allPrinters;
        std::string previewResult = "";
        std::string type = "";
        sptr<IPrintCallback> callback = nullptr;
        std::string fileUri = "";
        uint32_t fileOffset = 0;
        uint32_t fileMaxRead = -1;
        std::vector<uint8_t> fileRead;
        PrinterCapability printerCapability;
        bool result = false;
        std::string stateType_ = "";
        PrintJob printJob;
        PrinterInfo printerInfo;
        std::string jobId = "";
        std::string printerId = "";
        std::vector<std::string> extensionList;
        uint32_t applicationEvent = -1;
        PrinterPreferences printerPreference;
        uint32_t defaultPrinterType = -1;

        InnerPrintContext() : Context(nullptr, nullptr) {};
        InnerPrintContext(InputAction input, OutputAction output) : Context(std::move(input), std::move(output)) {};
        virtual ~InnerPrintContext() {};
    };
};
}  // namespace OHOS::Print
#endif  // NAPI_INNER_PRINT_H
