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

#ifndef PRINT_TASK_H
#define PRINT_TASK_H

#include <map>
#include <mutex>
#include <string>
#include <vector>

#include "ui_content.h"
#include "modal_ui_extension_config.h"
#include "want.h"
#include "napi_base_context.h"
#include "ipc_skeleton.h"
#include "bundle_mgr_client.h"
#include "iremote_object.h"
#include "napi/native_api.h"
#include "napi/native_common.h"

#include "iprint_callback.h"
#include "print_async_call.h"
#include "print_modal_ui_callback.h"
#include "print_context.h"
#include "print_utils.h"

namespace OHOS::Print {
class PrintTask {
public:
    explicit PrintTask(const std::vector<std::string> &fileList, const sptr<IRemoteObject> &callerToken_);
    PrintTask(const std::string &printJobName_, const sptr<IPrintCallback> &printAdapterCallback_,
        const std::shared_ptr<PrintAttributes> &printAttributes_, const sptr<IRemoteObject> &callerToken_);
    ~PrintTask();

    uint32_t Start(napi_env env, napi_callback_info info);
    uint32_t StartPrintAdapter(napi_env env, napi_callback_info info);
    void Stop();
    const std::string &GetId() const;
    static napi_value On(napi_env env, napi_callback_info info);
    static napi_value Off(napi_env env, napi_callback_info info);

    bool IsSupportType(const std::string &type) const;

private:
    uint32_t CallSpooler(napi_env env, napi_callback_info info, const std::shared_ptr<AdapterParam> &adapterParam,
        bool isPrintByAdapter);
    bool ParseAbilityContextReq(napi_env env, const napi_value &obj,
        std::shared_ptr<OHOS::AbilityRuntime::AbilityContext> &abilityContext,
        std::shared_ptr<OHOS::AbilityRuntime::UIExtensionContext> &uiExtensionContext);
    uint32_t StartUIExtensionAbility(
        std::shared_ptr<BaseContext> asyncContext, const std::shared_ptr<AdapterParam> &adapterParam);
    uint32_t StartUIExtensionAbility(OHOS::AAFwk::Want &want, std::shared_ptr<BaseContext> asyncContext);
    OHOS::Ace::UIContent* GetUIContent(const BaseContext *asyncContext);
    void CreateDefaultAdapterParam(const std::shared_ptr<AdapterParam> &adapterParam);
    bool CheckPermission(const std::string &name);

private:
    struct TaskEventContext : public PrintAsyncCall::Context {
        std::string type = "";
        std::string taskId = "";
        sptr<IPrintCallback> callback = nullptr;
        bool result = false;
        napi_status status = napi_generic_failure;
        TaskEventContext() : Context(nullptr, nullptr) {};
        TaskEventContext(InputAction input, OutputAction output) : Context(std::move(input), std::move(output)) {};
        virtual ~TaskEventContext() {};
    };

    std::string taskId_;
    std::vector<std::string> fileList_;
    std::vector<uint32_t> fdList_;
    std::map<std::string, bool> supportEvents_;
    uint32_t pathType_ = 0;
    sptr<IRemoteObject> callerToken_ = nullptr;
    std::string printJobName_;
    sptr<IPrintCallback> printAdapterCallback_ = nullptr;
    std::shared_ptr<PrintAttributes> printAttributes_ = nullptr;
    enum FilePathType {
        FD_UNDEFINED,
        FD_PATH,
        FILE_PATH,
        FILE_PATH_ABSOLUTED
    };
};
}  // namespace OHOS::Print
#endif  // PRINT_TASK_H
