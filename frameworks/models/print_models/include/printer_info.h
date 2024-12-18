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

#ifndef PRINTER_INFO_H
#define PRINTER_INFO_H

#include "parcel.h"
#include "printer_capability.h"

namespace OHOS::Print {
class PrinterInfo final : public Parcelable {
public:
    explicit PrinterInfo();

    PrinterInfo(const PrinterInfo &right);

    PrinterInfo &operator=(const PrinterInfo &right);

    virtual ~PrinterInfo();

    void SetPrinterId(const std::string &printerId);

    void SetPrinterName(std::string printerName);

    void SetPrinterIcon(uint32_t printIcon);

    void SetPrinterState(uint32_t printerState);

    void SetDescription(std::string description);

    void SetCapability(const PrinterCapability &capability);

    void SetUri(const std::string &uri);

    void SetPrinterMake(const std::string &printerMake);

    void SetOption(const std::string &option);

    void SetIsDefaultPrinter(bool isDefaultPrinter);

    void SetIsLastUsedPrinter(bool isLastUsedPrinter);

    void SetPrinterStatus(uint32_t printerStatus);

    [[nodiscard]] const std::string &GetPrinterId() const;

    [[nodiscard]] const std::string &GetPrinterName() const;
    
    [[nodiscard]] bool HasPrinterIcon() const;

    [[nodiscard]] uint32_t GetPrinterIcon() const;

    [[nodiscard]] uint32_t GetPrinterState() const;

    [[nodiscard]] bool HasDescription() const;

    [[nodiscard]] const std::string &GetDescription() const;

    [[nodiscard]] bool HasCapability() const;

    void GetCapability(PrinterCapability &cap) const;

    [[nodiscard]] bool HasUri() const;

    [[nodiscard]] std::string GetUri() const;

    [[nodiscard]] bool HasPrinterMake() const;

    [[nodiscard]] std::string GetPrinterMake() const;

    [[nodiscard]] bool HasOption() const;

    [[nodiscard]] std::string GetOption() const;

    [[nodiscard]] bool HasIsDefaultPrinter() const;

    [[nodiscard]] bool GetIsDefaultPrinter() const;

    [[nodiscard]] bool HasIsLastUsedPrinter() const;

    [[nodiscard]] bool GetIsLastUsedPrinter() const;

    [[nodiscard]] bool HasPrinterStatus() const;

    [[nodiscard]] uint32_t GetPrinterStatus() const;

    virtual bool Marshalling(Parcel &parcel) const override;

    static std::shared_ptr<PrinterInfo> Unmarshalling(Parcel &parcel);

    void Dump() const;

private:
    bool ReadFromParcel(Parcel &parcel);

    void ReadInnerPropertyFromParcel(PrinterInfo& right, Parcel& parcel);

    bool ValidateAll();

private:
    std::string printerId_;

    std::string printerName_;

    uint32_t printerState_;

    bool hasPrinterIcon_;

    uint32_t printerIcon_;

    bool hasDescription_;

    std::string description_;

    bool hasPrinterStatus_;

    uint32_t printerStatus_;

    bool hasCapability_;

    PrinterCapability capability_;

    bool hasUri_;

    std::string uri_;

    bool hasPrinterMake_;

    std::string printerMake_;

    bool hasOption_;

    std::string option_;

    bool hasIsDefaultPrinter_; // Deprecated, to be removed in a future version.

    bool isDefaultPrinter_; // Deprecated, to be removed in a future version.

    bool hasIsLastUsedPrinter_; // Deprecated, to be removed in a future version.

    bool isLastUsedPrinter_; // Deprecated, to be removed in a future version.
};
}  // namespace OHOS::Print
#endif  // PRINTER_INFO_H
