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

#include "print_pdf_render.h"
#include "napi/native_common.h"
#include "napi_print_utils.h"
#include "print_callback.h"
#include "print_log.h"
#include "print_manager_client.h"
#include "print_utils.h"

namespace OHOS::Print {
static constexpr const char *PAGESIZE_WIDTH = "width";
static constexpr const char *PAGESIZE_HEIGHT = "height";

PrintPdfRender::PrintPdfRender(const std::string file)
{
    file_ = file;
}

PrintPdfRender::~PrintPdfRender() {}

int32_t PrintPdfRender::Init(const std::string sandBoxPath)
{
    int32_t ret = PrintManagerClient::GetInstance()->PdfRenderInit(file_, sandBoxPath, basePngName, pageCount);
    if (ret != E_PRINT_NONE) {
        PRINT_HILOGE("Failed to init PdfRender");
        return ret;
    }
    return ret;
}

napi_value PrintPdfRender::Destroy(napi_env env, napi_callback_info info)
{
    PRINT_HILOGD("PdfRender::Destroy Enter ---->");
    size_t argc = NapiPrintUtils::MAX_ARGC;
    napi_value argv[NapiPrintUtils::MAX_ARGC] = { nullptr };
    napi_value thisVal = nullptr;
    void *data = nullptr;
    PRINT_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVal, &data));
    PRINT_ASSERT(env, argc == NapiPrintUtils::ARGC_ZERO, "PdfRender::Destroy need 0 parameter!");
    PrintPdfRender *render = nullptr;
    PRINT_CALL(env, napi_unwrap(env, thisVal, reinterpret_cast<void **>(&render)));
    if (render == nullptr) {
        PRINT_HILOGE("This render is null");
        return nullptr;
    }
    int32_t ret = PrintManagerClient::GetInstance()->PdfRenderDestroy(render->basePngName, render->pageCount);
    if (ret != E_PRINT_NONE) {
        PRINT_HILOGE("Failed to PdfRenderDestroy");
        return nullptr;
    }
    return nullptr;
}

napi_value PrintPdfRender::GetPageCount(napi_env env, napi_callback_info info)
{
    PRINT_HILOGD("PdfRender::GetPageCount Enter ---->");
    size_t argc = NapiPrintUtils::MAX_ARGC;
    napi_value argv[NapiPrintUtils::MAX_ARGC] = { nullptr };
    napi_value thisVal = nullptr;
    void *data = nullptr;
    PRINT_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVal, &data));
    PRINT_ASSERT(env, argc == NapiPrintUtils::ARGC_ZERO, "PdfRender::GetPageCount need 0 parameter!");
    PrintPdfRender *render = nullptr;
    PRINT_CALL(env, napi_unwrap(env, thisVal, reinterpret_cast<void **>(&render)));
    napi_value result = nullptr;
    if (render == nullptr) {
        PRINT_HILOGE("This render is null");
        return result;
    }
    result = NapiPrintUtils::CreateUint32(env, render->pageCount);
    return result;
}

napi_value PrintPdfRender::GetPageSize(napi_env env, napi_callback_info info)
{
    PRINT_HILOGD("PdfRender::GetPageCount Enter ---->");
    size_t argc = NapiPrintUtils::MAX_ARGC;
    napi_value argv[NapiPrintUtils::MAX_ARGC] = { nullptr };
    napi_value thisVal = nullptr;
    void *data = nullptr;
    PRINT_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVal, &data));
    PRINT_ASSERT(env, argc == NapiPrintUtils::ARGC_ONE, "PdfRender::GetPageSize need 1 parameter!");
    napi_valuetype valuetype;
    PRINT_CALL(env, napi_typeof(env, argv[NapiPrintUtils::ARGC_ZERO], &valuetype));
    PRINT_ASSERT(env, valuetype == napi_number, "index not a number");
    uint32_t pageIndex = NapiPrintUtils::GetUint32FromValue(env, argv[NapiPrintUtils::INDEX_ZERO]);
    PrintPdfRender *render = nullptr;
    napi_value napi_pageSize = nullptr;
    PRINT_CALL(env, napi_unwrap(env, thisVal, reinterpret_cast<void **>(&render)));
    if (pageIndex < 0 || pageIndex >= render->pageCount) {
        PRINT_HILOGE("This pageIndex can't be used");
        return napi_pageSize;
    }
    uint32_t pageSizeWidth = 0;
    uint32_t pageSizeHeight = 0;
    int32_t ret = PrintManagerClient::GetInstance()->GetPdfPageSize(render->file_, pageIndex,
                                                                    pageSizeWidth, pageSizeHeight);
    if (ret != E_PRINT_NONE) {
        PRINT_HILOGE("Failed to GetPageSize");
        return napi_pageSize;
    }
    PRINT_CALL(env, napi_create_object(env, &napi_pageSize));
    NapiPrintUtils::SetUint32Property(env, napi_pageSize, PAGESIZE_WIDTH, pageSizeWidth);
    NapiPrintUtils::SetUint32Property(env, napi_pageSize, PAGESIZE_HEIGHT, pageSizeHeight);
    return napi_pageSize;
}

napi_value PrintPdfRender::RenderPageToPng(napi_env env, napi_callback_info info)
{
    PRINT_HILOGD("PdfRender::RenderPageToPng Enter ---->");
    auto context = std::make_shared<PrintPdfRenderEventContext>();
    auto input = [context](napi_env env, size_t argc, napi_value *argv, napi_value self) -> napi_status {
        // Check PdfRender::RenderPageToPng parameters except callback
        PRINT_ASSERT_BASE(env, argc == NapiPrintUtils::ARGC_ONE, "need 1 parameter!", napi_invalid_arg);
        napi_valuetype valuetype;
        PRINT_CALL_BASE(env, napi_typeof(env, argv[NapiPrintUtils::INDEX_ZERO], &valuetype), napi_invalid_arg);
        PRINT_ASSERT_BASE(env, valuetype == napi_number, "type is not a number", napi_number_expected);
        // Get pageIndex and PdfRender
        context->pageIndex = NapiPrintUtils::GetUint32FromValue(env, argv[NapiPrintUtils::INDEX_ZERO]);
        PrintPdfRender *render = nullptr;
        PRINT_CALL_BASE(env, napi_unwrap(env, self, reinterpret_cast<void **>(&render)), napi_invalid_arg);
        if (render == nullptr) {
            PRINT_HILOGE("This render is null");
            context->SetErrorIndex(E_PRINT_INVALID_PARAMETER);
            return napi_invalid_arg;
        }
        if (context->pageIndex < 0 || context->pageIndex >= render->pageCount) {
            PRINT_HILOGE("This pageIndex can't be used");
            context->SetErrorIndex(E_PRINT_INVALID_PARAMETER);
            return napi_invalid_arg;
        }
        context->file_ = render->file_;
        context->basePngName = render->basePngName;
        return napi_ok;
    };
    auto output = [context](napi_env env, napi_value *result) -> napi_status {
        *result = NapiPrintUtils::CreateStringUtf8(env, context->imagePath);
        return napi_ok;
    };
    auto exec = [context](PrintAsyncCall::Context *ctx) {
        int32_t ret = PrintManagerClient::GetInstance()->RenderPdfToPng(context->file_,
                                                                        context->basePngName,
                                                                        context->pageIndex,
                                                                        context->imagePath);
        if (ret != E_PRINT_NONE) {
            PRINT_HILOGE("Failed to RenderPdfToPng");
            context->SetErrorIndex(ret);
        }
    };
    context->SetAction(std::move(input), std::move(output));
    PrintAsyncCall asyncCall(env, info, std::dynamic_pointer_cast<PrintAsyncCall::Context>(context));
    return asyncCall.Call(env, exec);
}
} // namespace OHOS::Print
