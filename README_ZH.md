# 打印框架服务<a name="ZH-CN_TOPIC_0000001124232845"></a>

-   [简介](#section11660541593)
-   [目录](#section161941989596)
-   [约束](#section119744591305)
-   [说明](#section1312121216216)
-   [相关仓](#section1371113476307)

## 简介<a name="section11660541593"></a>

打印框架服务（Distributed Print Service，DPS） 支持三方应用创建打印任务，拉起后台打印任务管理，管理打印扩展和打印任务。
提供打印扩展框架，实现三方打印扩展的接入，管理打印任务与打印机之间的关系，启动、暂停/恢复、取消打印任务，查询打印进度等。

打印框架服务覆盖范围包含：打印管理、打印管理服务和打印扩展管理：

-   **打印管理**

    支持应用创建打印任务，查询自身打印任务列表及任务信息、支持应用启动、取消打印任务、支持应用设置打印任务属性、支持应用响应打印任务UI界面中的打印任务属性变化，重新布局生成打印预览数据。

-   **打印管理服务**

    对接打印管理，接受应用的打印任务相关请求，执行相应权限校验，提供后台打印任务管理与打印管理之间的跨设备通信通道，管理系统中打印扩展。包括打印扩展的新增、移除、使用状态、监听状态变化等，管理打印任务队列。包括：打印格式化数据的传递、打印任务的数据缓存等；对接使用中的打印扩展，分发和管理打印任务。

-   **打印扩展管理**

    定义打印扩展的生命周期、定义打印扩展与打印管理服务之间的接口、定义打印任务，管理打印任务与打印机之间的关系，启动、取消打印任务，查询打印进度、定义打印文档信息，用于应用向打印扩展传输打印数据、定义打印机信息，用于打印任务UI界面呈现

  首先，从应用端发起打印请求到PrintManager，再从打印系统管理PrintSA发送到PrinterSpooler打印服务，拉起打印预览界面到应用层。由PrintSpooler向PrintSA发送开启发现能力指令，再由PrintSA向打印扩展接口PrintExtension发送开启发现能力指令，再由PrintExtension向华为打印扩展接口HwPrintExtension发送开启发现能力发现打印机。再由HwPrintExtension向PrintSA上报发现打印机信息，最后返回到PrintSpooler。最后由PrintSpooler发送打印任务到PrintSA，再发送到PrintExtension后发送到HwPrintExtension，最后发送给打印机。

**图 1**  部件架构图<a name="fig371116145419"></a>

![](figures/print-architecture.png)


## 目录<a name="section161941989596"></a>

```
foundation/print_print_fwk        #打印框架服务
├── frameworks       
│   └── innerkitsimpl             # Native接口
    └── kits\extension            # 打印扩展管理
├── interfaces\kits               # 接口层代码
│   └── jsnapi                     
        └── extension             # 管理扩展生命周期接口 
        └── extensionctx          # 管理扩展上下文
│   └── napi                      # JavaScript接口
├── services                      # 服务层代码
└── test                          # 测试用例资源
```

## 约束<a name="section119744591305"></a>

-   如需使用打印框架服务完整功能，需要申请code ohos.permission.PRINT、ohos.permission.MANAGE_PRINT_JOB权限。

## 说明<a name="section1312121216216"></a>
部件内子模块职责说明：

-   **打印管理（Print NAPI）**

     对接打印应用的JS API，负责创建打印任务，监听打印任务状态变化等接口的实现

-   **打印管理（InnerPrint NAPI）**

     对接打印后台管理应用、三方打印扩展的内部JS API，负责打印机发现、预览、启动打印、监听打印机、打印任务状态变化等接口的实现

-   **打印管理客户端（Print Manager Client）**

    负责打印服务在客户端的接口实现，对外(JS应用或者Native应用）提供统一的打印服务能力

- **打印管理客户端（Print Manager Client）**

    对接打印管理，接受应用的打印任务相关请求，执行权限校验，并拉起后台打印任务管理；管理系统中打印扩展。包括打印扩展的新增、移除、修改属性（已安装的、默认的、当前使用的）、查询状态、监听变化等；管理打印任务队列。包括：打印格式化数据的传递、打印任务的数据缓存等；对接使用中的打印扩展，分发和管理打印任务

- **打印拓展（Print Extension）**

    提供打印拓展框架，提供打印机的标准接入能力，负责定义打印拓展应用的生命周期管理，定义打印扩展与打印管理服务之间的接口；定义打印任务，管理打印任务与打印机之间的关系，启动、暂停/恢复、取消打印任务，查询打印进度；定义打印文档信息，用于应用向打印扩展传输打印数据；定义打印机信息，用于打印任务UI界面呈现

- **三方打印扩展（PrintPA）**

    打印机厂商实现具体打印功能提供

- **打印后台管理应用（Printspooler）** 
  
    系统打印管理应用

## 相关仓<a name="section1371113476307"></a>

元能力子系统

[**ability_runtime**](https://gitee.com/openharmony/ability_ability_runtime)

