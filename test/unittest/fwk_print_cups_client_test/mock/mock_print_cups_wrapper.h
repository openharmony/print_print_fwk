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

#ifndef MOCK_PRINT_CUPS_WRAPPER_H
#define MOCK_PRINT_CUPS_WRAPPER_H

#include <gmock/gmock.h>
#include "print_cups_wrapper.h"

namespace OHOS {
namespace Print {
class MockPrintCupsWrapper final : public IPrintAbilityBase {
public:
    MOCK_METHOD3(GetNamedDest, cups_dest_t*(http_t *, const char *, const char *));
    MOCK_METHOD2(FreeDests, void(int, cups_dest_t *));
    MOCK_METHOD3(DoRequest, ipp_t *(http_t *, ipp_t *, const char *));
    MOCK_METHOD2(CopyDestInfo, cups_dinfo_t*(http_t *, cups_dest_t *));
    MOCK_METHOD1(FreeDestInfo, void(cups_dinfo_t *dinfo));
    MOCK_METHOD1(FreeRequest, void(ipp_t *response));
};
}  // namespace Print
}  // namespace OHOS
#endif  // MOCK_PRINT_CUPS_WRAPPER_H
