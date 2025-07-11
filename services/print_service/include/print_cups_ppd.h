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

#ifndef PRINT_CUPS_PPD_H
#define PRINT_CUPS_PPD_H

#include <cups/ppd.h>
#include <cups/ppd-private.h>
#include "printer_capability.h"
#include "print_constant.h"

namespace OHOS::Print {
void ParsePrinterAttributesFromPPD(ppd_file_t *ppd, PrinterCapability &printerCaps);
int32_t QueryPrinterCapabilityFromPPDFile(PrinterCapability &printerCaps, const std::string &ppdFilePath);

const int ADVANCE_OPTION_MAXLENGTH = 100;
}
#endif // PRINT_CUPS_PPD_H