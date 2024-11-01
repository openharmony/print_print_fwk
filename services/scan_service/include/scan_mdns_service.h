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

#ifndef SCAN_MDNS_SERVICE_H
#define SCAN_MDNS_SERVICE_H

#include <unordered_set>
#include "i_mdns_event.h"
#include "mdns_event_stub.h"
#include "mdns_client.h"
#include "mdns_common.h"
#include "scanner_info.h"
#include "scan_log.h"
#ifdef SANE_ENABLE
#include "sane/sane.h"
#include "sane/saneopts.h"
#endif
#include "scan_service_ability.h"

namespace OHOS::Scan {
using namespace OHOS::NetManagerStandard;

class ScanMDnsDiscoveryObserver : public DiscoveryCallbackStub {
public:
    explicit ScanMDnsDiscoveryObserver(const MDnsServiceInfo &info) : expected_(info) {}
    virtual ~ScanMDnsDiscoveryObserver() = default;
    void HandleStartDiscover(const MDnsServiceInfo &serviceInfo, int32_t retCode) override{}
    void HandleStopDiscover(const MDnsServiceInfo &serviceInfo, int32_t retCode) override;
    void HandleServiceLost(const MDnsServiceInfo &serviceInfo, int32_t retCode) override{}
    void HandleServiceFound(const MDnsServiceInfo &info, int32_t retCode) override;
    int32_t OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option) override
    {
        SCAN_HILOGD("GetScannerList HandleSerieFound OnRemoteRequest");
        return DiscoveryCallbackStub::OnRemoteRequest(code, data, reply, option);
    }
public:
    MDnsServiceInfo expected_;
};

class ScanMDnsResolveObserver : public ResolveCallbackStub {
public:
    explicit ScanMDnsResolveObserver(const MDnsServiceInfo &info):_serviceInfo(info) {}
    virtual ~ScanMDnsResolveObserver() = default;
    int32_t OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption& option) override
    {
        SCAN_HILOGD("GetScannerList ScanMDnsResolveObserver OnRemoteRequest");
        return ResolveCallbackStub::OnRemoteRequest(code, data, reply, option);
    }
    void HandleResolveResult(const MDnsServiceInfo& info, int32_t retCode) override;
public:
    MDnsServiceInfo _serviceInfo;
};

class ScanMdnsService {
public:
    ScanMdnsService(const ScanMdnsService&) = delete;
    ScanMdnsService& operator=(const ScanMdnsService&) = delete;
    static ScanMdnsService& GetInstance()
    {
        static ScanMdnsService instance;
        return instance;
    }
    void SetServiceInfo(const MDnsServiceInfo& info);
    void SetMDnsResolveCallBack(sptr<ScanMDnsResolveObserver>& cb);
    void SetMDnsDiscoveryCallBack(sptr<ScanMDnsDiscoveryObserver>& cb);
    void SetServiceType(std::string stype);
    sptr<ScanMDnsResolveObserver> GetMDnsResolveCallBack();
    sptr<ScanMDnsDiscoveryObserver> GetMDnsDiscoveryCallBack();
    MDnsServiceInfo& GetServiceInfo();
    static std::string GetServiceAttribute(MDnsServiceInfo& serviceInfo, std::string keyStr);

    bool onStartDiscoverService();
    bool onStopDiscoverService();
    bool onResolveService(MDnsServiceInfo& serviceInfo);
    void ToMDnsScaner(MDnsServiceInfo& serviceInfo);

public:
    sptr<ScanMDnsDiscoveryObserver> _scanMDnsDiscoveryCallBack;
    sptr<ScanMDnsResolveObserver> _scanMDnsResolveCallBack;
    
private:
    ScanMdnsService(){};
    MDnsServiceInfo _serviceInfo;
};
}
#endif // !SCAN_MDNS_SERVICE_H
