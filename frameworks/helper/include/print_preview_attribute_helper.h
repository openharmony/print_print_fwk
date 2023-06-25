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
#ifndef PRINT_PREVIEW_ATTRIBUTE_HELPER_H
#define PRINT_PREVIEW_ATTRIBUTE_HELPER_H

#include "napi/native_api.h"
#include "print_preview_attribute.h"

namespace OHOS::Print {
class PrintPreviewAttributeHelper {
public:
    static napi_value MakeJsObject(napi_env env, const PrintPreviewAttribute &preview);
    static std::shared_ptr<PrintPreviewAttribute> BuildFromJs(napi_env env, napi_value jsValue);

private:
    static bool ValidateProperty(napi_env env, napi_value object);
};
}  // namespace OHOS::Print
#endif  // PRINT_PREVIEW_ATTRIBUTE_HELPER_H
