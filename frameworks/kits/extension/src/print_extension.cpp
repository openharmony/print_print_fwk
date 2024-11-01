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

#include "print_extension.h"

#include "ability_loader.h"
#include "connection_manager.h"
#include "js_print_extension.h"
#include "print_extension_context.h"
#include "print_log.h"
#include "runtime.h"

namespace OHOS {
namespace AbilityRuntime {
using namespace OHOS::AppExecFwk;
PrintExtension *PrintExtension::Create(const std::unique_ptr<Runtime> &runtime)
{
    PRINT_HILOGD("jws PrintExtension::Create runtime");
    if (!runtime) {
        return new PrintExtension();
    }
    PRINT_HILOGD("jws PrintExtension::Create runtime");
    switch (runtime->GetLanguage()) {
        case Runtime::Language::JS:
            return JsPrintExtension::Create(runtime);

        default:
            return new PrintExtension();
    }
}

void PrintExtension::Init(const std::shared_ptr<AbilityLocalRecord> &record,
    const std::shared_ptr<OHOSApplication> &application, std::shared_ptr<AbilityHandler> &handler,
    const sptr<IRemoteObject> &token)
{
    PRINT_HILOGD("jws PrintExtension begin init context");
    ExtensionBase<PrintExtensionContext>::Init(record, application, handler, token);
    PRINT_HILOGD("PrintExtension begin init context");
}

std::shared_ptr<PrintExtensionContext> PrintExtension::CreateAndInitContext(
    const std::shared_ptr<AbilityLocalRecord> &record, const std::shared_ptr<OHOSApplication> &application,
    std::shared_ptr<AbilityHandler> &handler, const sptr<IRemoteObject> &token)
{
    PRINT_HILOGD("jws PrintExtension begin init context");
    std::shared_ptr<PrintExtensionContext> context =
        ExtensionBase<PrintExtensionContext>::CreateAndInitContext(record, application, handler, token);
    if (context == nullptr) {
        PRINT_HILOGE("PrintExtension::CreateAndInitContext context is nullptr");
        return context;
    }
    return context;
}

void PrintExtension::OnStop()
{
    Extension::OnStop();
    GetContext()->TerminateAbility();
}
} // namespace AbilityRuntime
} // namespace OHOS