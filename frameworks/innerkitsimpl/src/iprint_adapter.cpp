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

#include "iprint_adapter.h"

namespace OHOS::Print {
void PrintDocumentAdapter::onStartLayoutWrite(const std::string &jobId, const PrintAttributes &oldAttrs,
    const PrintAttributes &newAttrs, uint32_t fd,
    std::function<void(std::string, uint32_t)> writeResultCallback)
{
}

void PrintDocumentAdapter::onJobStateChanged(const std::string &jobId, uint32_t state)
{
}
}