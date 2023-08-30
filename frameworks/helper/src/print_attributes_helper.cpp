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

#include "napi/native_api.h"
#include "print_attributes_helper.h"

namespace OHOS::Print {
static constexpr const char *PARAM_JOB_COPYNUMBER = "copyNumber";
static constexpr const char *PARAM_JOB_PAGERANGE = "pageRange";
static constexpr const char *PARAM_JOB_ISSEQUENTIAL = "isSequential";
static constexpr const char *PARAM_JOB_PAGESIZE = "pageSize";
static constexpr const char *PARAM_JOB_ISLANDSCAPE = "isLandscape";
static constexpr const char *PARAM_JOB_COLORMODE = "colorMode";
static constexpr const char *PARAM_JOB_DUPLEXMODE = "duplexMode";
static constexpr const char *PARAM_JOB_MARGIN = "margin";
static constexpr const char *PARAM_JOB_OPTION = "option";

napi_value PrintAttributesHelper::MakeJsObject(napi_env env, const PrintAttributes &attributes)
{
    napi_value jsObj = nullptr;
    PRINT_CALL(env, napi_create_object(env, &jsObj));
    NapiPrintUtils::SetUint32Property(env, jsObj, PARAM_JOB_COPYNUMBER, attributes.GetCopyNumber());

    if (!CreatePageRange(env, jsObj, attributes)) {
        PRINT_HILOGE("Failed to create page range property of print job");
        return nullptr;
    }

    NapiPrintUtils::SetUint32Property(env, jsObj, PARAM_JOB_ISSEQUENTIAL, attributes.GetIsSequential());

    if (!CreatePageSize(env, jsObj, attributes)) {
        PRINT_HILOGE("Failed to create page size property of print job");
        return nullptr;
    }

    NapiPrintUtils::SetUint32Property(env, jsObj, PARAM_JOB_ISLANDSCAPE, attributes.GetIsLandscape());
    NapiPrintUtils::SetUint32Property(env, jsObj, PARAM_JOB_COLORMODE, attributes.GetColorMode());
    NapiPrintUtils::SetUint32Property(env, jsObj, PARAM_JOB_DUPLEXMODE, attributes.GetDuplexMode());

    if (!CreateMargin(env, jsObj, attributes)) {
        PRINT_HILOGE("Failed to create margin property of print job");
        return nullptr;
    }

    if (attributes.HasOption()) {
        NapiPrintUtils::SetStringPropertyUtf8(env, jsObj, PARAM_JOB_OPTION, attributes.GetOption());
    }
    return jsObj;
}

std::shared_ptr<PrintAttributes> PrintAttributesHelper::BuildFromJs(napi_env env, napi_value jsValue)
{
    auto nativeObj = std::make_shared<PrintAttributes>();
    if (nativeObj == nullptr) {
        PRINT_HILOGE("Failed to create print job object");
        return nullptr;
    }

    if (!ValidateProperty(env, jsValue)) {
        PRINT_HILOGE("Invalid property of print job");
        return nullptr;
    }

    uint32_t copyNumber = NapiPrintUtils::GetUint32Property(env, jsValue, PARAM_JOB_COPYNUMBER);
    bool isSequential = NapiPrintUtils::GetBooleanProperty(env, jsValue, PARAM_JOB_ISSEQUENTIAL);
    bool isLandscape = NapiPrintUtils::GetBooleanProperty(env, jsValue, PARAM_JOB_ISLANDSCAPE);
    uint32_t colorMode = NapiPrintUtils::GetUint32Property(env, jsValue, PARAM_JOB_COLORMODE);
    uint32_t duplexMode = NapiPrintUtils::GetUint32Property(env, jsValue, PARAM_JOB_DUPLEXMODE);
    nativeObj->SetCopyNumber(copyNumber);
    nativeObj->SetIsSequential(isSequential);
    nativeObj->SetIsLandscape(isLandscape);
    nativeObj->SetColorMode(colorMode);
    nativeObj->SetDuplexMode(duplexMode);

    BuildJsWorkerIsLegal(env, jsValue, nativeObj);
    nativeObj->Dump();
    return nativeObj;
}

std::shared_ptr<PrintAttributes> PrintAttributesHelper::BuildJsWorkerIsLegal(napi_env env, napi_value jsValue,
    std::shared_ptr<PrintAttributes> &nativeObj)
{
    napi_value jsPageRange = NapiPrintUtils::GetNamedProperty(env, jsValue, PARAM_JOB_PAGERANGE);
    auto pageRangePtr = PrintRangeHelper::BuildFromJs(env, jsPageRange);
    if (pageRangePtr == nullptr) {
        PRINT_HILOGE("Failed to build print job object from js");
        return nullptr;
    }
    nativeObj->SetPageRange(*pageRangePtr);

    napi_value jsPageSize = NapiPrintUtils::GetNamedProperty(env, jsValue, PARAM_JOB_PAGESIZE);
    auto pageSizePtr = PrintPageSizeHelper::BuildFromJs(env, jsPageSize);
    if (pageSizePtr == nullptr) {
        PRINT_HILOGE("Failed to build print job object from js");
        return nullptr;
    }
    nativeObj->SetPageSize(*pageSizePtr);

    napi_value jsMargin = NapiPrintUtils::GetNamedProperty(env, jsValue, PARAM_JOB_MARGIN);
    if (jsMargin != nullptr) {
        auto marginPtr = PrintMarginHelper::BuildFromJs(env, jsMargin);
        if (marginPtr == nullptr) {
            PRINT_HILOGE("Failed to build print job object from js");
            return nullptr;
        }
        nativeObj->SetMargin(*marginPtr);
    }

    napi_value jsOption = NapiPrintUtils::GetNamedProperty(env, jsValue, PARAM_JOB_OPTION);
    if (jsOption != nullptr) {
        nativeObj->SetOption(NapiPrintUtils::GetStringPropertyUtf8(env, jsValue, PARAM_JOB_OPTION));
    }
    return nativeObj;
}

bool PrintAttributesHelper::CreatePageRange(napi_env env, napi_value &jsPrintAttributes,
    const PrintAttributes &attributes)
{
    PrintRange range;
    attributes.GetPageRange(range);
    napi_value jsPageRange = PrintRangeHelper::MakeJsObject(env, range);
    PRINT_CALL_BASE(env, napi_set_named_property(env, jsPrintAttributes, PARAM_JOB_PAGERANGE, jsPageRange), false);
    return true;
}

bool PrintAttributesHelper::CreatePageSize(napi_env env, napi_value &jsPrintAttributes,
    const PrintAttributes &attributes)
{
    PrintPageSize pageSize;
    attributes.GetPageSize(pageSize);
    napi_value jsPageSize = PrintPageSizeHelper::MakeJsObject(env, pageSize);
    PRINT_CALL_BASE(env, napi_set_named_property(env, jsPrintAttributes, PARAM_JOB_PAGESIZE, jsPageSize), false);
    return true;
}

bool PrintAttributesHelper::CreateMargin(napi_env env, napi_value &jsPrintAttributes,
    const PrintAttributes &attributes)
{
    PrintMargin margin;
    attributes.GetMargin(margin);
    napi_value jsMargin = PrintMarginHelper::MakeJsObject(env, margin);
    PRINT_CALL_BASE(env, napi_set_named_property(env, jsPrintAttributes, PARAM_JOB_MARGIN, jsMargin), false);
    return true;
}

bool PrintAttributesHelper::ValidateProperty(napi_env env, napi_value object)
{
    std::map<std::string, PrintParamStatus> propertyList = {
        {PARAM_JOB_COPYNUMBER, PRINT_PARAM_NOT_SET},
        {PARAM_JOB_PAGERANGE, PRINT_PARAM_NOT_SET},
        {PARAM_JOB_ISSEQUENTIAL, PRINT_PARAM_NOT_SET},
        {PARAM_JOB_PAGESIZE, PRINT_PARAM_NOT_SET},
        {PARAM_JOB_ISLANDSCAPE, PRINT_PARAM_NOT_SET},
        {PARAM_JOB_COLORMODE, PRINT_PARAM_NOT_SET},
        {PARAM_JOB_DUPLEXMODE, PRINT_PARAM_NOT_SET},
        {PARAM_JOB_MARGIN, PRINT_PARAM_OPT},
        {PARAM_JOB_OPTION, PRINT_PARAM_OPT},
    };

    auto names = NapiPrintUtils::GetPropertyNames(env, object);
    for (auto name : names) {
        if (propertyList.find(name) == propertyList.end()) {
            PRINT_HILOGE("Invalid property: %{public}s", name.c_str());
            return false;
        }
        propertyList[name] = PRINT_PARAM_SET;
    }

    for (auto propertypItem : propertyList) {
        if (propertypItem.second == PRINT_PARAM_NOT_SET) {
            PRINT_HILOGE("Missing Property: %{public}s", propertypItem.first.c_str());
            return false;
        }
    }
    return true;
}
}
