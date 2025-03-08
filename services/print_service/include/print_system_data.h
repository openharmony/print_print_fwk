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

#ifndef PRINT_SYSTEM_DATA_H
#define PRINT_SYSTEM_DATA_H

#include <string>
#include <map>
#include <vector>
#include <json/json.h>
#include "printer_info.h"
#include "printer_capability.h"
#include "print_constant.h"
#include "print_utils.h"
#include "print_log.h"
#include "print_map_safe.h"
#include "printer_preferences.h"

namespace OHOS {
namespace Print {

struct CupsPrinterInfo {
    std::string name;
    std::string uri;
    std::string maker;
    PrinterCapability printerCapability;
    PrinterStatus printerStatus = PRINTER_STATUS_UNAVAILABLE;
    std::string alias;
    PrinterPreferences printPreferences;
};

class PrintSystemData {
public:
    bool Init();
    void InsertCupsPrinter(const std::string &printerId, const CupsPrinterInfo &printerInfo);
    void SavePrinterFile(const std::string &printerId);
    std::string QueryPrinterIdByStandardizeName(const std::string &printerName);
    bool QueryCupsPrinterInfoByPrinterId(const std::string &printerId, CupsPrinterInfo &cupsPrinter);
    void InsertPrinterInfo(const std::string &printerId, const PrinterInfo &printerInfo);
    std::shared_ptr<PrinterInfo> QueryPrinterInfoByPrinterId(const std::string &printerId);
    bool IsPrinterAdded(const std::string &printerId);
    bool GetPrinterCapabilityFromSystemData(
        CupsPrinterInfo &cupsPrinter, std::string printerId, PrinterCapability &printerCapability);
    void DeleteCupsPrinter(const std::string &printerId, const std::string &printerName);
    void GetAddedPrinterListFromSystemData(std::vector<std::string> &printerNameList);
    void UpdatePrinterStatus(const std::string &printerId, PrinterStatus printerStatus);
    bool UpdatePrinterAlias(const std::string& printerId, const std::string& printerAlias);
    void UpdatePrinterUri(const std::shared_ptr<PrinterInfo> &printerInfo);
    void UpdatePrinterPreferences(const std::string &printerId, PrinterPreferences &preferences);
    void QueryPrinterInfoById(const std::string &printerId, PrinterInfo &printerInfo);
    bool CheckPrinterBusy(const std::string &printerId);
    bool GetAllPrintUser(std::vector<int32_t> &allPrintUserList);

    std::vector<std::string> QueryAddedPrinterIdList();
    std::shared_ptr<PrinterInfo> QueryDiscoveredPrinterInfoById(const std::string &printerId);
    std::shared_ptr<PrinterInfo> QueryDiscoveredPrinterInfoByName(const std::string &printerName);
    void AddPrinterToDiscovery(std::shared_ptr<PrinterInfo> printerInfo);
    void RemovePrinterFromDiscovery(const std::string &printerId);
    std::map<std::string, std::shared_ptr<PrinterInfo>> GetDiscoveredPrinterInfo();
    size_t GetDiscoveredPrinterCount();
    void ClearDiscoveredPrinterList();

    void AddIpPrinterToList(std::shared_ptr<PrinterInfo> printerInfo);
    void RemoveIpPrinterFromList(const std::string &printerId);
    std::shared_ptr<PrinterInfo> QueryIpPrinterInfoById(const std::string &printerId);
    int32_t BuildPrinterPreference(const PrinterCapability &cap, PrinterPreferences &printPreferences);
    void BuildEprintPreference(const PrinterCapability &cap, PrinterPreferences &printPreferences);
    void ConvertJsonToPrinterPreferences(Json::Value &preferencesJson, PrinterPreferences &preferences);

private:
    bool ParsePrinterListJsonV1(Json::Value& jsonObject);
    bool GetJsonObjectFromFile(Json::Value &jsonObject, const std::string &fileName);
    void ConvertPrinterCapabilityToJson(PrinterCapability &printerCapability, Json::Value &capsJson);
    void ConvertPrintMarginToJson(PrinterCapability &printerCapability, Json::Value &capsJson);
    void ConvertPageSizeToJson(PrinterCapability &printerCapability, Json::Value &capsJson);
    void ConvertPrintResolutionToJson(PrinterCapability &printerCapability, Json::Value &capsJson);
    void ConvertSupportedColorModeToJson(PrinterCapability &printerCapability, Json::Value &capsJson);
    void ConvertSupportedDuplexModeToJson(PrinterCapability &printerCapability, Json::Value &capsJson);
    void ConvertSupportedMediaTypeToJson(PrinterCapability &printerCapability, Json::Value &capsJson);
    void ConvertSupportedQualityToJson(PrinterCapability &printerCapability, Json::Value &capsJson);
    bool ConvertJsonToPrinterCapability(Json::Value &capsJson, PrinterCapability &printerCapability);
    bool ConvertJsonToPrintMargin(Json::Value &capsJson, PrinterCapability &printerCapability);
    bool ConvertJsonToPageSize(Json::Value &capsJson, PrinterCapability &printerCapability);
    bool ConvertJsonToPrintResolution(Json::Value &capsJson, PrinterCapability &printerCapability);
    bool ConvertJsonToSupportedColorMode(Json::Value &capsJson, PrinterCapability &printerCapability);
    bool ConvertJsonToSupportedDuplexMode(Json::Value &capsJson, PrinterCapability &printerCapability);
    bool ConvertJsonToSupportedMediaType(Json::Value &capsJson, PrinterCapability &printerCapability);
    bool ConvertJsonToSupportedQuality(Json::Value &capsJson, PrinterCapability &printerCapability);
    bool ConvertJsonToSupportedOrientation(Json::Value &capsJson, PrinterCapability &printerCapability);
    bool GetPrinterCapabilityFromFile(std::string printerId, PrinterCapability &printerCapability);
    bool CheckPrinterInfoJson(Json::Value &object, std::string &printerId);
    bool GetPrinterCapabilityFromJson(
        std::string printerId, Json::Value &jsonObject, PrinterCapability &printerCapability);
    bool ParseUserListJsonV1(
        Json::Value &jsonObject, std::vector<int32_t> &allPrintUserList);
    bool ConvertJsonToCupsPrinterInfo(Json::Value &object);
    void ConvertInnerJsonToCupsPrinterInfo(Json::Value &object, CupsPrinterInfo &info);

    bool ParsePreviousPreferencesSetting(Json::Value &settingJson, PrinterPreferences &preferences);
    bool ParsePrinterPreferencesJson(Json::Value &jsonObject);
    bool ReadJsonFile(const std::filesystem::path &path);
    std::string ParseDefaultPageSizeId(const PrinterCapability &cap, Json::Value &capOpt);
    int32_t ParseDefaultOrientation(const PrinterCapability &cap, Json::Value &capOpt);
    int32_t ParseDefaultDuplexMode(const PrinterCapability &cap, Json::Value &capOpt);
    int32_t ParseDefaultPrintQuality(const PrinterCapability &cap, Json::Value &capOpt);
    std::string ParseDefaultMediaType(const PrinterCapability &cap, Json::Value &capOpt);
    void DeleteFile(const std::filesystem::path &path);

    template<typename T>
    bool ProcessJsonToCapabilityList(Json::Value &capsJson,
                                     const std::string &key,
                                     PrinterCapability &printerCapability,
                                     void (PrinterCapability::*setter)(const std::vector <T> &),
                                     std::function<bool(const Json::Value &, T &)> converter)
    {
        if (!PrintJsonUtil::IsMember(capsJson, key) || !capsJson[key].isArray()) {
            PRINT_HILOGW("Cannot find %{public}s or it's not an array", key.c_str());
            return true;
        }
        PRINT_HILOGD("find Capability %{public}s success", key.c_str());
        std::vector<T> resultList;
        for (const auto &item: capsJson[key]) {
            if (!PrintUtils::CheckJsonType<T>(item)) {
                PRINT_HILOGE("%{public}s item has incorrect type", key.c_str());
                return false;
            }
            T object;
            bool ret = converter(item, object);
            if (!ret) {
                PRINT_HILOGE("Invalid format,key is %{public}s", key.c_str());
                return false;
            }
            resultList.push_back(object);
        }
        if (!resultList.empty()) {
            (printerCapability.*setter)(resultList);
        }
        PRINT_HILOGD("processCapabilityList success, %{public}s", key.c_str());
        return true;
    }

private:
    PrintMapSafe<CupsPrinterInfo> addedPrinterMap_;
    std::map<std::string, std::shared_ptr<PrinterInfo>> addedPrinterInfoList_;
    std::map<std::string, std::shared_ptr<PrinterInfo>> discoveredPrinterInfoList_;
    std::map<std::string, std::shared_ptr<PrinterInfo>> connectingIpPrinterInfoList_;
    std::mutex discoveredListMutex;
    std::mutex connectingIpPrinterListMutex;
};

}  // namespace Print
}  // namespace OHOS
#endif  // PRINT_SYSTEM_DATA_H
