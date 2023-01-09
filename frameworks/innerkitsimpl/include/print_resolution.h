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

#ifndef PRINT_RESOLUTION_H
#define PRINT_RESOLUTION_H

#include "napi/native_api.h"
#include "parcel.h"

namespace OHOS::Print {
class PrintResolution final : public Parcelable {
public:
    explicit PrintResolution();

    PrintResolution(const PrintResolution &right);

    PrintResolution &operator=(const PrintResolution &right);

    virtual ~PrintResolution();

    void Reset();

    [[nodiscard]] const std::string &GetId() const;

    [[nodiscard]] uint32_t GetHorizontalDpi() const;

    [[nodiscard]] uint32_t GetVerticalDpi() const;

    virtual bool Marshalling(Parcel &parcel) const override;

    static std::shared_ptr<PrintResolution> Unmarshalling(Parcel &parcel);

    napi_value ToJsObject(napi_env env) const;

    static std::shared_ptr<PrintResolution> BuildFromJs(napi_env env, napi_value jsValue);

    void Dump();

private:
    void SetId(const std::string &id);

    void SetHorizontalDpi(uint32_t horizontalDpi);

    void SetVerticalDpi(uint32_t verticalDpi);

    bool ReadFromParcel(Parcel &parcel);

    static bool ValidateProperty(napi_env env, napi_value object);

private:
    std::string id_;
    uint32_t horizontalDpi_;
    uint32_t verticalDpi_;
};
} // namespace OHOS::Print
#endif // PRINT_RESOLUTION_H