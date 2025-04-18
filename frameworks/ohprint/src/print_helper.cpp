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

#include <vector>

#include "print_helper.h"
#include "print_constant.h"
#include "print_log.h"
#include "print_util.h"
#include "ability_manager_client.h"
#include "print_converter.h"
#include "print_manager_client.h"
#include "print_util.h"
#include "print_json_util.h"
#include <json/json.h>

namespace OHOS::Print {

const std::string DUPLEX_STRING = "duplex";
const std::string ORIENTATION_STRING = "orientation";
const std::string PAGESIZEID_STRING = "pagesizeId";
const std::string QUALITY_STRING = "quality";
const std::string DEFAULT_QUALITY_PREFERENCE = "4";
const int NUMBER_BASE = 10;
const uint32_t COPIES_NUMBER_DEFAULT = 99;

char *CopyString(const std::string &source)
{
    auto len = source.length();
    char *dest = new (std::nothrow) char[len + 1];
    if (dest == nullptr) {
        PRINT_HILOGW("allocate failed");
        return nullptr;
    }
    if (strcpy_s(dest, len + 1, source.c_str()) != 0) {
        PRINT_HILOGW("CopyString strcpy_s failed");
    }
    dest[len] = '\0';
    return dest;
}

void AddJsonFieldStringToJsonObject(const Json::Value &cupsOpt, const std::string &keyword,
    Json::Value &advancedCapJson)
{
    PRINT_HILOGD("AddJsonFieldStringToJsonObject %{public}s", keyword.c_str());
    if (!PrintJsonUtil::IsMember(cupsOpt, keyword)) {
        PRINT_HILOGW("missing keyword");
        return;
    }
    if (!cupsOpt[keyword].isString()) {
        PRINT_HILOGW("not string type");
        return;
    }
    std::string optionString = cupsOpt[keyword].asString();
    PRINT_HILOGD("AddJsonFieldStringToJsonObject %{public}s", optionString.c_str());
    advancedCapJson[keyword] = optionString;
}

void ParseJsonFieldAsArrayOpt(const Json::Value &cupsOpt, const std::string &key,
    Print_PrinterInfo &nativePrinterInfo, void (*arrayOpteration)(const Json::Value &, Print_PrinterInfo &))
{
    PRINT_HILOGD("ParseJsonFieldAsArrayOpt: %{public}s", key.c_str());
    if (arrayOpteration == nullptr) {
        PRINT_HILOGW("arrayOpteration is null");
        return;
    }
    if (!PrintJsonUtil::IsMember(cupsOpt, key)) {
        PRINT_HILOGW("key missing");
        return;
    }
    if (!cupsOpt[key].isString()) {
        PRINT_HILOGW("not string");
        return;
    }
    std::string arrayJson = cupsOpt[key].asString();
    Json::Value arrayOpt;
    if (!PrintJsonUtil::Parse(arrayJson, arrayOpt)) {
        PRINT_HILOGW("accept fail");
        return;
    }
    
    if (!arrayOpt.isArray()) {
        PRINT_HILOGW("not array");
        return;
    }
    arrayOpteration(arrayOpt, nativePrinterInfo);
}

bool ParseJsonFieldAsInt(const Json::Value &cupsOpt, const std::string &key, int &value)
{
    if (!PrintJsonUtil::IsMember(cupsOpt, key)) {
        PRINT_HILOGW("key missing");
        return false;
    }
    if (cupsOpt[key].isInt()) {
        value = cupsOpt[key].asInt();
        return true;
    }
    if (!cupsOpt[key].isString()) {
        PRINT_HILOGW("incorrect type");
        return false;
    }
    std::string jsonString = cupsOpt[key].asString();
    if (!jsonString.empty() && ConvertStringToInt(jsonString.c_str(), value)) {
        return true;
    }
    return false;
}

void ConvertJsonArrayToIntList(const Json::Value &jsonArray, const std::string &key, std::vector<uint32_t> &list)
{
    PRINT_HILOGD("ConvertJsonArrayToIntList %{public}s, %{public}u", key.c_str(), jsonArray.size());
    for (auto &item : jsonArray) {
        int value = 0;
        if (!ParseJsonFieldAsInt(item, key, value)) {
            continue;
        }
        list.push_back(static_cast<uint32_t>(value));
    }
}

void ReleaseCapabilityPageSizes(Print_PrinterCapability &capability)
{
    if (capability.supportedPageSizes != nullptr) {
        for (uint32_t i = 0; i < capability.supportedPageSizesCount; i++) {
            SAFE_DELETE_ARRAY(capability.supportedPageSizes[i].id);
            SAFE_DELETE_ARRAY(capability.supportedPageSizes[i].name);
        }
        SAFE_DELETE_ARRAY(capability.supportedPageSizes);
    }
    capability.supportedPageSizesCount = 0;
}

void ReleaseCapability(Print_PrinterCapability &capability)
{
    ReleaseCapabilityPageSizes(capability);
    SAFE_DELETE_ARRAY(capability.supportedColorModes);
    capability.supportedColorModesCount = 0;
    SAFE_DELETE_ARRAY(capability.supportedDuplexModes);
    capability.supportedDuplexModesCount = 0;
    SAFE_DELETE_ARRAY(capability.supportedMediaTypes);
    SAFE_DELETE_ARRAY(capability.supportedQualities);
    capability.supportedQualitiesCount = 0;
    SAFE_DELETE_ARRAY(capability.supportedPaperSources);
    SAFE_DELETE_ARRAY(capability.supportedResolutions);
    capability.supportedResolutionsCount = 0;
    SAFE_DELETE_ARRAY(capability.supportedOrientations);
    capability.supportedOrientationsCount = 0;
    SAFE_DELETE_ARRAY(capability.advancedCapability);
}

void ReleaseDefaultValue(Print_DefaultValue &defaultValue)
{
    SAFE_DELETE_ARRAY(defaultValue.defaultPageSizeId);
    SAFE_DELETE_ARRAY(defaultValue.defaultMediaType);
    SAFE_DELETE_ARRAY(defaultValue.defaultPaperSource);
    SAFE_DELETE_ARRAY(defaultValue.otherDefaultValues);
}

void ReleasePrinterInfo(Print_PrinterInfo &printerInfo)
{
    SAFE_DELETE_ARRAY(printerInfo.printerId);
    SAFE_DELETE_ARRAY(printerInfo.printerName);
    SAFE_DELETE_ARRAY(printerInfo.description);
    SAFE_DELETE_ARRAY(printerInfo.location);
    SAFE_DELETE_ARRAY(printerInfo.makeAndModel);
    SAFE_DELETE_ARRAY(printerInfo.printerUri);
    SAFE_DELETE_ARRAY(printerInfo.detailInfo);
    ReleaseCapability(printerInfo.capability);
    ReleaseDefaultValue(printerInfo.defaultValue);
}

void PageSizeArrayConvert(const OHOS::Print::PrinterCapability &cap, Print_PrinterInfo &nativePrinterInfo)
{
    ReleaseCapabilityPageSizes(nativePrinterInfo.capability);
    std::vector<PrintPageSize> pageSizeVector;
    std::vector<Print_PageSize> nativePageSizeVector;
    cap.GetSupportedPageSize(pageSizeVector);
    for (const auto &pageSize : pageSizeVector) {
        nativePageSizeVector.push_back({
            CopyString(pageSize.GetId()),
            CopyString(pageSize.GetName()),
            pageSize.GetWidth(),
            pageSize.GetHeight()
        });
    }
    nativePrinterInfo.capability.supportedPageSizes = CopyArray<Print_PageSize>(nativePageSizeVector,
        nativePrinterInfo.capability.supportedPageSizesCount);
    PRINT_HILOGD("Get nativePageSizeVector size = %{public}zu from printerCap", nativePageSizeVector.size());
}

void DuplexArrayConvert(const OHOS::Print::PrinterCapability &cap, Print_PrinterInfo &nativePrinterInfo)
{
    std::vector<uint32_t> pageDuplexVector;
    cap.GetSupportedDuplexMode(pageDuplexVector);
    nativePrinterInfo.capability.supportedDuplexModes = CopyArray<uint32_t, Print_DuplexMode>(
        pageDuplexVector, nativePrinterInfo.capability.supportedDuplexModesCount, ConvertDuplexMode);
    PRINT_HILOGD("Get pageDuplexVector size = %{public}zu from printerCap", pageDuplexVector.size());
}

void ColorModeArrayConvert(const OHOS::Print::PrinterCapability &cap, Print_PrinterInfo &nativePrinterInfo)
{
    std::vector<uint32_t> colorModeVector;
    cap.GetSupportedColorMode(colorModeVector);
    nativePrinterInfo.capability.supportedColorModes = CopyArray<uint32_t, Print_ColorMode>(
        colorModeVector, nativePrinterInfo.capability.supportedColorModesCount, ConvertColorMode);
    PRINT_HILOGD("Get colorModeVector size = %{public}zu from printerCap", colorModeVector.size());
}

void OrientationArrayConvert(const OHOS::Print::PrinterCapability &cap, Print_PrinterInfo &nativePrinterInfo)
{
    std::vector<uint32_t> orientationVector;
    cap.GetSupportedOrientation(orientationVector);
    nativePrinterInfo.capability.supportedOrientations = CopyArray<uint32_t, Print_OrientationMode>(
        orientationVector, nativePrinterInfo.capability.supportedOrientationsCount, ConvertOrientationMode);
    PRINT_HILOGD("Get orientationVector size = %{public}zu from printerCap", orientationVector.size());
}

void ResolutionArrayConvert(const OHOS::Print::PrinterCapability &cap, Print_PrinterInfo &nativePrinterInfo)
{
    std::vector<PrintResolution> resolutionVector;
    std::vector<Print_Resolution> nativeResolutionVector;
    cap.GetResolution(resolutionVector);
    for (const auto &resolution : resolutionVector) {
        nativeResolutionVector.push_back({resolution.GetHorizontalDpi(), resolution.GetVerticalDpi()});
    }
    nativePrinterInfo.capability.supportedResolutions =
        CopyArray<Print_Resolution>(nativeResolutionVector, nativePrinterInfo.capability.supportedResolutionsCount);
    PRINT_HILOGD("Get nativeResolutionVector size = %{public}zu from printerCap", nativeResolutionVector.size());
}

void QualityArrayConvert(const OHOS::Print::PrinterCapability &cap, Print_PrinterInfo &nativePrinterInfo)
{
    std::vector<uint32_t> qualityVector;
    cap.GetSupportedQuality(qualityVector);
    nativePrinterInfo.capability.supportedQualities = CopyArray<uint32_t, Print_Quality>(
        qualityVector, nativePrinterInfo.capability.supportedQualitiesCount, ConvertQuality);
    PRINT_HILOGD("Get qualityVector size = %{public}zu from printerCap", qualityVector.size());
}

void MediaTypeArrayConvert(const OHOS::Print::PrinterCapability &cap, Print_PrinterInfo &nativePrinterInfo)
{
    std::vector<std::string> mediaTypeVector;
    cap.GetSupportedMediaType(mediaTypeVector);
    if (mediaTypeVector.size() > 0) {
        Json::Value mediaTypeJson;
        for (auto const &mediaType: mediaTypeVector) {
            mediaTypeJson.append(mediaType);
        }
        nativePrinterInfo.capability.supportedMediaTypes = CopyString(PrintJsonUtil::WriteString(mediaTypeJson));
    }
    PRINT_HILOGD("Get mediaTypeVector size = %{public}zu from printerCap", mediaTypeVector.size());
}

void DefaultPageMarginConvert(const OHOS::Print::PrinterCapability &cap, Print_PrinterInfo &nativePrinterInfo)
{
    uint32_t leftMargin = 0;
    uint32_t topMargin = 0;
    uint32_t rightMargin = 0;
    uint32_t bottomMargin = 0;
    PrintMargin margin;
    cap.GetMinMargin(margin);
    if (margin.HasLeft()) {
        leftMargin = margin.GetLeft();
    }
    if (margin.HasTop()) {
        topMargin = margin.GetTop();
    }
    if (margin.HasRight()) {
        rightMargin = margin.GetRight();
    }
    if (margin.HasBottom()) {
        bottomMargin = margin.GetBottom();
    }
    nativePrinterInfo.defaultValue.defaultMargin = {leftMargin, topMargin, rightMargin, bottomMargin};
}

void SetNativePrinterInfoFromCap(const OHOS::Print::PrinterCapability &cap, Print_PrinterInfo &nativePrinterInfo)
{
    PageSizeArrayConvert(cap, nativePrinterInfo);
    DuplexArrayConvert(cap, nativePrinterInfo);
    ColorModeArrayConvert(cap, nativePrinterInfo);
    OrientationArrayConvert(cap, nativePrinterInfo);
    MediaTypeArrayConvert(cap, nativePrinterInfo);
    ResolutionArrayConvert(cap, nativePrinterInfo);
    DefaultPageMarginConvert(cap, nativePrinterInfo);
    QualityArrayConvert(cap, nativePrinterInfo);
    ConvertColorMode(cap.GetColorMode(), nativePrinterInfo.defaultValue.defaultColorMode);
    ConvertDuplexMode(cap.GetDuplexMode(), nativePrinterInfo.defaultValue.defaultDuplexMode);
}

void ParseMediaOpt(const Json::Value &cupsOpt, Print_PrinterInfo &nativePrinterInfo)
{
    if (PrintJsonUtil::IsMember(cupsOpt, "defaultPageSizeId") && cupsOpt["defaultPageSizeId"].isString()) {
        std::string defaultPageSizeId = cupsOpt["defaultPageSizeId"].asString();
        PRINT_HILOGD("defaultPageSizeId %{public}s", defaultPageSizeId.c_str());
        nativePrinterInfo.defaultValue.defaultPageSizeId = CopyString(defaultPageSizeId);
    }
    if (PrintJsonUtil::IsMember(cupsOpt, "media-type-supported") && cupsOpt["media-type-supported"].isString()) {
        std::string mediaTypeSupported = cupsOpt["media-type-supported"].asString();
        PRINT_HILOGD("cupsOptionsStr media-type-supported %{public}s", mediaTypeSupported.c_str());
        nativePrinterInfo.capability.supportedMediaTypes = CopyString(mediaTypeSupported);
    }
    if (PrintJsonUtil::IsMember(cupsOpt, "media-type-default") && cupsOpt["media-type-default"].isString()) {
        std::string mediaTypeDefault = cupsOpt["media-type-default"].asString();
        PRINT_HILOGD("cupsOptionsStr media-type-default %{public}s", mediaTypeDefault.c_str());
        nativePrinterInfo.defaultValue.defaultMediaType = CopyString(mediaTypeDefault);
    }
    if (PrintJsonUtil::IsMember(cupsOpt, "media-source-default") && cupsOpt["media-source-default"].isString()) {
        std::string mediaSourceDefault = cupsOpt["media-source-default"].asString();
        PRINT_HILOGD("cupsOptionsStr media-source-default %{public}s", mediaSourceDefault.c_str());
        nativePrinterInfo.defaultValue.defaultPaperSource = CopyString(mediaSourceDefault);
    }
    if (PrintJsonUtil::IsMember(cupsOpt, "media-source-supported") && cupsOpt["media-source-supported"].isString()) {
        std::string mediaSourceSupported = cupsOpt["media-source-supported"].asString();
        PRINT_HILOGD("cupsOptionsStr media-source-supported %{public}s", mediaSourceSupported.c_str());
        nativePrinterInfo.capability.supportedPaperSources = CopyString(mediaSourceSupported);
    }
}

bool ParseResolutionObject(const Json::Value &jsonObject, Print_Resolution &resolution)
{
    if (!PrintJsonUtil::IsMember(jsonObject, "horizontalDpi")) {
        PRINT_HILOGW("horizontalDpi missing");
        return false;
    }
    if (!jsonObject["horizontalDpi"].isInt()) {
        PRINT_HILOGW("horizontalDpi is not string");
        return false;
    }
    int xDpi = jsonObject["horizontalDpi"].asInt();
    if (!PrintJsonUtil::IsMember(jsonObject, "verticalDpi")) {
        PRINT_HILOGW("verticalDpi missing");
        return false;
    }
    if (!jsonObject["verticalDpi"].isInt()) {
        PRINT_HILOGW("verticalDpi is not string");
        return false;
    }
    int yDpi = jsonObject["verticalDpi"].asInt();
    resolution.horizontalDpi = static_cast<uint32_t>(xDpi);
    resolution.verticalDpi = static_cast<uint32_t>(yDpi);
    return true;
}

void ParsePrinterOpt(const Json::Value &cupsOpt, Print_PrinterInfo &nativePrinterInfo)
{
    if (PrintJsonUtil::IsMember(cupsOpt, "printer-location") && cupsOpt["printer-location"].isString()) {
        std::string pLocation = cupsOpt["printer-location"].asString();
        PRINT_HILOGD("printer-location: %{public}s", pLocation.c_str());
        nativePrinterInfo.location = CopyString(pLocation);
    }
    std::string keyword = "orientation-requested-default";
    if (PrintJsonUtil::IsMember(cupsOpt, keyword) && cupsOpt[keyword].isString()) {
        std::string orientationString = cupsOpt[keyword].asString();
        PRINT_HILOGD("default orientation: %{public}s", orientationString.c_str());
        int orientationValue = 0;
        if (ConvertStringToInt(orientationString.c_str(), orientationValue)) {
            uint32_t defaultOrientation = static_cast<uint32_t>(orientationValue);
            ConvertOrientationMode(defaultOrientation, nativePrinterInfo.defaultValue.defaultOrientation);
        }
    }
    keyword = "printer-resolution-default";
    if (PrintJsonUtil::IsMember(cupsOpt, keyword) && cupsOpt[keyword].isString()) {
        std::string resolutionString = cupsOpt[keyword].asString();
        Json::Value resolutionJson;
        if (!PrintJsonUtil::Parse(resolutionString, resolutionJson)) {
            if (!ParseResolutionObject(resolutionJson, nativePrinterInfo.defaultValue.defaultResolution)) {
                PRINT_HILOGW("ParseResolutionObject fail");
            }
        }
    }
}

bool ConvertStringToUint32(const char *src, uint32_t &dst)
{
    if (src == nullptr || src[0] == '\0') {
        return false;
    }
    errno = 0;
    char *endPtr = nullptr;
    dst = strtoul(src, &endPtr, NUMBER_BASE);
    if (errno == ERANGE || endPtr == src || *endPtr != '\0') {
        PRINT_HILOGW("ConvertStringToUint32 fail: %{public}s", src);
        return false;
    }
    return true;
}

void ParseCupsCopyOpt(const Json::Value &cupsOpt, Print_PrinterInfo &nativePrinterInfo)
{
    if (PrintJsonUtil::IsMember(cupsOpt, "copies-default") && cupsOpt["copies-default"].isString()) {
        std::string defaultCopies = cupsOpt["copies-default"].asString();
        uint32_t defaultCopiesNum = 0;
        if (!ConvertStringToUint32(defaultCopies.c_str(), defaultCopiesNum)) {
            PRINT_HILOGE("copies-default error: %{public}s", defaultCopies.c_str());
            return;
        }
        nativePrinterInfo.defaultValue.defaultCopies = defaultCopiesNum;
        PRINT_HILOGD("copies-default: %{public}d", defaultCopiesNum);
    }
    if (PrintJsonUtil::IsMember(cupsOpt, "copies-supported") && cupsOpt["copies-supported"].isString()) {
        std::string copySupport = cupsOpt["copies-supported"].asString();
        uint32_t copySupportNum = 0;
        if (!ConvertStringToUint32(copySupport.c_str(), copySupportNum)) {
            PRINT_HILOGE("copies-supported error: %{public}s", copySupport.c_str());
            return;
        }
        nativePrinterInfo.capability.supportedCopies = copySupportNum;
        PRINT_HILOGD("copies-supported: %{public}d", copySupportNum);
    }
}

void ParseCupsOptions(const Json::Value &cupsOpt, Print_PrinterInfo &nativePrinterInfo)
{
    ParsePrinterOpt(cupsOpt, nativePrinterInfo);
    ParseCupsCopyOpt(cupsOpt, nativePrinterInfo);
    ParseMediaOpt(cupsOpt, nativePrinterInfo);
    Json::Value advancedCapJson;
    std::string keyword = "multiple-document-handling-supported";
    AddJsonFieldStringToJsonObject(cupsOpt, keyword, advancedCapJson);
    nativePrinterInfo.capability.advancedCapability = CopyString((PrintJsonUtil::WriteString(advancedCapJson)).c_str());
}

int32_t ParseInfoOption(const std::string &infoOption, Print_PrinterInfo &nativePrinterInfo)
{
    Json::Value infoJson;
    if (!PrintJsonUtil::Parse(infoOption, infoJson)) {
        PRINT_HILOGW("infoOption can not parse to json object");
        return E_PRINT_INVALID_PARAMETER;
    }
    if (!PrintJsonUtil::IsMember(infoJson, "printerUri") || !infoJson["printerUri"].isString() ||
        !PrintJsonUtil::IsMember(infoJson, "make") || !infoJson["make"].isString()) {
        PRINT_HILOGW("The infoJson does not have a necessary attribute.");
        return E_PRINT_INVALID_PARAMETER;
    }
    nativePrinterInfo.makeAndModel = CopyString(infoJson["make"].asString());
    nativePrinterInfo.printerUri = CopyString(infoJson["printerUri"].asString());
    if (!PrintJsonUtil::IsMember(infoJson, "cupsOptions")) {
        PRINT_HILOGW("The infoJson does not have a cupsOptions attribute.");
        return E_PRINT_NONE;
    }
    Json::Value cupsOpt = infoJson["cupsOptions"];
    ParseCupsOptions(cupsOpt, nativePrinterInfo);
    return E_PRINT_NONE;
}

void ParsePrinterPreference(const PrinterInfo &info, Print_PrinterInfo &nativePrinterInfo)
{
    if (!info.HasPreferences()) {
        PRINT_HILOGW("The printerInfo does not have preferences");
        return;
    }
    PrinterPreferences preferences;
    info.GetPreferences(preferences);

    if (preferences.HasDefaultDuplexMode()) {
        ConvertDuplexMode(preferences.GetDefaultDuplexMode(), nativePrinterInfo.defaultValue.defaultDuplexMode);
    }
    if (preferences.HasDefaultOrientation()) {
        ConvertOrientationMode(preferences.GetDefaultOrientation(), nativePrinterInfo.defaultValue.defaultOrientation);
    }
    if (!preferences.GetDefaultPageSizeId().empty()) {
        nativePrinterInfo.defaultValue.defaultPageSizeId = CopyString(preferences.GetDefaultPageSizeId());
    }
    if (preferences.HasDefaultPrintQuality()) {
        ConvertQuality(preferences.GetDefaultPrintQuality(), nativePrinterInfo.defaultValue.defaultPrintQuality);
    }
    if (!preferences.GetDefaultMediaType().empty()) {
        nativePrinterInfo.defaultValue.defaultMediaType = CopyString(preferences.GetDefaultMediaType());
    }
}

char *ParseDetailInfo(const PrinterInfo &info)
{
    Json::Value detailInfoJson;
    if (info.HasAlias()) {
        detailInfoJson["alias"] = info.GetAlias();
    }

    Json::Value opsJson;
    if (info.HasOption() && PrintJsonUtil::Parse(info.GetOption(), opsJson)) {
        if (PrintJsonUtil::IsMember(opsJson, "vendorId") && opsJson["vendorId"].isInt()) {
            detailInfoJson["vendorId"] = opsJson["vendorId"].asInt();
        }

        if (PrintJsonUtil::IsMember(opsJson, "productId") && opsJson["productId"].isInt()) {
            detailInfoJson["productId"] = opsJson["productId"].asInt();
        }
    }

    if (detailInfoJson.isNull() || detailInfoJson.empty()) {
        return nullptr;
    }

    return CopyString(PrintJsonUtil::WriteString(detailInfoJson));
}

Print_PrinterInfo *ConvertToNativePrinterInfo(const PrinterInfo &info)
{
    Print_PrinterInfo *nativePrinterInfo = new (std::nothrow) Print_PrinterInfo;
    if (nativePrinterInfo == nullptr) {
        PRINT_HILOGW("Print_PrinterInfo allocate fail.");
        return nullptr;
    }
    if (memset_s(nativePrinterInfo, sizeof(Print_PrinterInfo), 0, sizeof(Print_PrinterInfo)) != 0) {
        PRINT_HILOGW("Print_PrinterInfo memset_s fail.");
        delete nativePrinterInfo;
        nativePrinterInfo = nullptr;
        return nullptr;
    }
    nativePrinterInfo->printerId = CopyString(info.GetPrinterId());
    nativePrinterInfo->printerName = CopyString(info.GetPrinterName());
    nativePrinterInfo->description = CopyString(info.GetDescription());
    nativePrinterInfo->detailInfo = ParseDetailInfo(info);
    nativePrinterInfo->printerState = static_cast<Print_PrinterState>(info.GetPrinterStatus());
    nativePrinterInfo->capability.supportedCopies = COPIES_NUMBER_DEFAULT;
    if (info.HasIsDefaultPrinter() && info.GetIsDefaultPrinter() == true) {
        nativePrinterInfo->isDefaultPrinter = true;
    }
    OHOS::Print::PrinterCapability cap;
    info.GetCapability(cap);

    SetNativePrinterInfoFromCap(cap, *nativePrinterInfo);
    Json::Value capJson;
    if (cap.HasOption() && PrintJsonUtil::Parse(cap.GetOption(), capJson)) {
        if (PrintJsonUtil::IsMember(capJson, "cupsOptions") && capJson["cupsOptions"].isObject()) {
            Json::Value cupsJson = capJson["cupsOptions"];
            ParseCupsOptions(cupsJson, *nativePrinterInfo);
        }
    }

    ParsePrinterPreference(info, *nativePrinterInfo);
    if (info.HasOption()) {
        std::string infoOpt = info.GetOption();
        PRINT_HILOGW("infoOpt json object: %{public}s", infoOpt.c_str());
        ParseInfoOption(infoOpt, *nativePrinterInfo);
    }
    return nativePrinterInfo;
}

void SetPrintOrientationInPrintJob(const Print_PrintJob &nativePrintJob, PrintJob &printJob)
{
    uint32_t ori = static_cast<uint32_t>(nativePrintJob.orientationMode);
    if (ori == ORIENTATION_MODE_PORTRAIT || ori == ORIENTATION_MODE_REVERSE_PORTRAIT) {
        printJob.SetIsLandscape(false);
        printJob.SetIsSequential(true);
    } else if (ori == ORIENTATION_MODE_LANDSCAPE || ori == ORIENTATION_MODE_REVERSE_LANDSCAPE) {
        printJob.SetIsLandscape(true);
        printJob.SetIsSequential(false);
    }
}

void SetPrintMarginInPrintJob(const Print_PrintJob &nativePrintJob, PrintJob &printJob)
{
    PrintMargin margin;
    if (nativePrintJob.printMargin.topMargin > 0) {
        margin.SetTop(nativePrintJob.printMargin.topMargin);
    } else {
        margin.SetTop(0);
    }
    if (nativePrintJob.printMargin.rightMargin > 0) {
        margin.SetRight(nativePrintJob.printMargin.rightMargin);
    } else {
        margin.SetRight(0);
    }
    if (nativePrintJob.printMargin.bottomMargin > 0) {
        margin.SetBottom(nativePrintJob.printMargin.bottomMargin);
    } else {
        margin.SetBottom(0);
    }
    if (nativePrintJob.printMargin.leftMargin > 0) {
        margin.SetLeft(nativePrintJob.printMargin.leftMargin);
    } else {
        margin.SetLeft(0);
    }
    printJob.SetMargin(margin);
}

bool SetPrintPageSizeInPrintJob(const Print_PrintJob &nativePrintJob, PrintJob &printJob)
{
    PRINT_HILOGI("SetPrintPageSizeInPrintJob in.");
    if (nativePrintJob.pageSizeId == nullptr) {
        PRINT_HILOGW("page size is null.");
        return false;
    }
    std::string pageSizeId(nativePrintJob.pageSizeId);
    PrintPageSize pageSize;
    if (!PrintPageSize::FindPageSizeById(pageSizeId, pageSize)) {
        PRINT_HILOGW("use custom native page size: %{public}s.", pageSizeId.c_str());
        pageSize.SetId(pageSizeId);
        pageSize.SetName(pageSizeId);
    }
    printJob.SetPageSize(pageSize);
    PRINT_HILOGI("SetPrintPageSizeInPrintJob out.");
    return true;
}

void SetOptionInPrintJob(const Print_PrintJob &nativePrintJob, PrintJob &printJob)
{
    PRINT_HILOGI("SetOptionInPrintJob in.");
    Json::Value jsonOptions;
    if (nativePrintJob.jobName != nullptr) {
        std::string documentName = std::string(nativePrintJob.jobName);
        jsonOptions["jobName"] = documentName;
        Json::Value jobDesArr;
        jobDesArr.append(documentName);
        jobDesArr.append("0");
        jobDesArr.append("1");
        jsonOptions["jobDesArr"] = jobDesArr;
        jsonOptions["isDocument"] = true;
    }
    if (nativePrintJob.mediaType != nullptr) {
        jsonOptions["mediaType"] = std::string(nativePrintJob.mediaType);
    }
    jsonOptions["borderless"] = nativePrintJob.borderless ? "true" : "false";
    Print_Quality quality = nativePrintJob.printQuality;
    if (quality > static_cast<Print_Quality>(PRINT_QUALITY_HIGH)
        || quality < static_cast<Print_Quality>(PRINT_QUALITY_DRAFT)) {
        quality = static_cast<Print_Quality>(PRINT_QUALITY_NORMAL);
    }
    jsonOptions["printQuality"] = quality;
    jsonOptions["documentFormat"] = GetDocumentFormatString(nativePrintJob.documentFormat);
    jsonOptions["isAutoRotate"] = nativePrintJob.orientationMode == ORIENTATION_MODE_NONE ? true : false;
    if (nativePrintJob.advancedOptions != nullptr) {
        jsonOptions["cupsOptions"] = std::string(nativePrintJob.advancedOptions);
    }
    std::string option = PrintJsonUtil::WriteStringUTF8(jsonOptions);
    PRINT_HILOGD("SetOptionInPrintJob %{public}s", option.c_str());
    printJob.SetOption(option);
    PRINT_HILOGI("SetOptionInPrintJob out.");
}

int32_t ConvertNativeJobToPrintJob(const Print_PrintJob &nativePrintJob, PrintJob &printJob)
{
    if (nativePrintJob.fdList == nullptr || nativePrintJob.copyNumber <= 0) {
        PRINT_HILOGW("ConvertNativeJobToPrintJob invalid param error.");
        return E_PRINT_INVALID_PARAMETER;
    }
    if (!IsValidString(nativePrintJob.printerId)) {
        PRINT_HILOGW("ConvertNativeJobToPrintJob string empty error.");
        return E_PRINT_INVALID_PARAMETER;
    }
    std::vector<uint32_t> fdList;
    for (uint32_t i = 0; i < nativePrintJob.fdListCount; i++) {
        fdList.emplace_back(nativePrintJob.fdList[i]);
    }
    printJob.SetFdList(fdList);
    printJob.SetPrinterId(std::string(nativePrintJob.printerId));
    printJob.SetCopyNumber(nativePrintJob.copyNumber);
    printJob.SetDuplexMode(static_cast<uint32_t>(nativePrintJob.duplexMode));
    printJob.SetColorMode(static_cast<uint32_t>(nativePrintJob.colorMode));

    SetPrintOrientationInPrintJob(nativePrintJob, printJob);
    SetPrintMarginInPrintJob(nativePrintJob, printJob);
    SetPrintPageSizeInPrintJob(nativePrintJob, printJob);
    SetOptionInPrintJob(nativePrintJob, printJob);
    return E_PRINT_NONE;
}

Print_ErrorCode ConvertStringVectorToPropertyList(const std::vector<std::string> &valueList,
    Print_PropertyList *propertyList)
{
    if (valueList.size() == 0) {
        PRINT_HILOGW("empty valueList");
        return PRINT_ERROR_INVALID_PRINTER;
    }
    propertyList->list = new (std::nothrow) Print_Property[valueList.size()];
    if (propertyList->list == nullptr) {
        PRINT_HILOGW("propertyList->list is null");
        return PRINT_ERROR_GENERIC_FAILURE;
    }
    if (memset_s(propertyList->list, valueList.size() * sizeof(Print_Property), 0,
        valueList.size() * sizeof(Print_Property)) != 0) {
        PRINT_HILOGW("memset_s fail");
        delete[] propertyList->list;
        propertyList->list = nullptr;
        return PRINT_ERROR_GENERIC_FAILURE;
    }
    uint32_t count = 0;
    for (size_t i = 0; i < valueList.size(); i++) {
        std::string keyVal = valueList[i];
        auto index = keyVal.find('&');
        if (index == keyVal.npos) {
            continue;
        }
        propertyList->list[count].key = CopyString(keyVal.substr(0, index));
        propertyList->list[count].value = CopyString(keyVal.substr(index + 1));
        count++;
    }
    propertyList->count = count;
    return PRINT_ERROR_NONE;
}
}  // namespace OHOS::Print
