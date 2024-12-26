/*
* Copyright (c) 2025 Huawei Device Co., Ltd.
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
#ifndef SANE_SA_DEATH_RECIPIENT_H
#define SANE_SA_DEATH_RECIPIENT_H

#include "iremote_object.h"
#include "refbase.h"

namespace OHOS::Scan {
class SaneSaDeathRecipient : public IRemoteObject::DeathRecipient {
public:
    explicit SaneSaDeathRecipient();
    ~SaneSaDeathRecipient() = default;
    void OnRemoteDied(const wptr<IRemoteObject> &object) override;
};
} // namespace OHOS::Scan
#endif // SANE_SA_DEATH_RECIPIENT_H