# openwifi：完全开源的SDR WiFi基带FPGA

> **来源**：微信公众号文章 - 橙子随记
> **原文链接**：https://mp.weixin.qq.com/s/FzoCNPnRCSw8fVOm0gQbxg
> **爬取时间**：2026-04-13
> **说明**：微信公众号文章中的图片通过JavaScript动态加载，无法直接爬取

---

## 概述

**openwifi**（https://github.com/open-sdr/openwifi）是一个开创性的开源项目，它实现了基于软件定义无线电（SDR）和FPGA的完整IEEE 802.11 Wi-Fi协议栈，包括基带处理、Linux驱动和用户空间软件。该项目由open-sdr组织维护，已获得**4.6k+ Stars、771 Forks**，是Wi-Fi领域少有的全开源硬件+软件实现。

项目核心目标是让研究者、开发者能够完全掌控Wi-Fi的PHY/MAC层，实现传统商用芯片无法提供的灵活性。它兼容Linux mac80211框架，支持标准Wi-Fi工具（如hostapd、wpa_supplicant、iw），同时提供丰富的侧信道数据（如CSI）和可配置参数，特别适合教育、科研、安全研究和创新应用（如无线TSN）。

---

## 项目背景与动机

传统Wi-Fi芯片是封闭黑盒，无法修改底层行为。openwifi填补了这一空白：它从**2017年启动**（欧盟ORCA项目），**2019年开源**，是全球首个免费开源的Wi-Fi FPGA基带设计。

它强调"开源优先"，但通过订阅模式（openwifi.tech）提供高级特性、支持和商业许可。仓库分为三个互补仓库：

- **openwifi**：Linux驱动、用户空间工具（本仓库）
- **openwifi-hw**：FPGA硬件设计（Verilog）
- **openwifi-hw-img**：预编译FPGA镜像

> **系统架构概览**：数据帧经mac80211进入SDR驱动，再由FPGA处理PHY层，最终通过AD9361等RF前端发射/接收。

> **原文章图片**：openwifi系统架构概览（左侧为Linux协议栈 + SDR驱动 + FPGA基带，右侧为典型硬件实物）

---

## 技术架构

openwifi采用**FPGA + ARM + Linux**的SoC架构：

### FPGA部分（openwifi-hw）
- 实现PHY（OFDM调制解调、OpenOFDM_TX/RX）
- 部分MAC（DCF/CSMA/CA、10µs SIFS）
- 关键模块：tx_intf、rx_intf、xpu（控制中心）、openofdm_tx/rx

### Linux驱动（openwifi/driver/sdr.c）
- 实现mac80211 ieee80211_ops API
- 支持TX/RX中断、TSF时间戳同步、滤波等

### 用户空间
- sdrctl工具通过nl80211 testmode命令直接操作FPGA寄存器
- 支持hostapd等标准工具

### 关键创新
- **TSF时间戳**精确同步侧信道数据（CSI、IQ样本）
- **FDD模式** + AD9361 SPI快速切换（TX/RX turnaround仅0.6µs）
- 可配置频率（70MHz–6GHz）、带宽（2/10/20MHz）和MAC参数（CCA门限、RTS/CTS、CW等）

---

## 关键特性

### 侧信道能力（科研杀手级）

- **CSI**：实时信道状态信息、频偏、均衡器数据
  - 支持CSI Fuzzer（人为操纵信道响应）
  - 支持CSI Radar（Wi-Fi感知运动，联合雷达-通信）
- **IQ Capture**：实时AGC/RSSI + 双天线IQ样本流

### 性能指标（最佳情况，FMCOMMS2 @ 2.4GHz）

| 指标 | 数值 |
|------|------|
| TCP吞吐 | 40–50 Mbps |
| iperf UDP | 50 Mbps（支持AMPDU） |
| EVM | -38 dB |
| MCS0灵敏度 | -92 dBm |
| MCS7灵敏度 | -73 dBm |

---

## 支持硬件平台

项目支持多种低成本/高性能Zynq + AD936x组合（无需额外RF板卡）：

| 板卡名称 | 描述 | 是否需Vivado付费许可 |
|---------|------|-------------------|
| zc706_fmcs2 | ZC706 + FMCOMMS2/3/4 | 是 |
| zed_fmcs2 | ZedBoard + FMCOMMS2/3/4 | 否 |
| adrv9364z7020 | ADRV9364-Z7020 + BOB | 否 |
| antsdr / antsdr_e200 | MicroPhase增强型ADALM-PLUTO | 否 |
| sdrpi | 树莓派尺寸HexSDR | 否 |
| …（更多见README） | … | … |

- 支持非官方板卡（如neptunesdr、LibreSDR）
- 无硬件也可通���**w-iLab.t**远程实验

> **原文章图片**：典型硬件实物（PYNQ-Z1 + AD936x板卡运行openwifi，屏幕显示AP界面）

> **原文章图片**：AntSDR E200板卡（低成本Zynq 7020 + AD9361，支持70MHz–6GHz）

---

## 安装与快速启动

1. 下载预编译镜像（1.5.0版）：`openwifi-1.5.0-shahecheng.img.xz`
2. 烧录至≥16GB SD卡，配置BOOT分区（复制对应board_name文件）
3. 上电 → SSH `root@192.168.10.122`（密码`openwifi`）
4. 执行 `setup_once.sh` → `./wgd.sh`（启动AP + Web服务器）
5. 客户端连接SSID `openwifi`，访问 `http://192.168.13.1`

> **提示**：详细步骤、已知问题（Vivado评估版2小时限制等）见README和known_issue。支持动态FPGA加载、无线互联网共享等。

---

## 高级应用与科研价值

- **安全研究**：包注入、fuzzing、Monitor模式
- **感知与雷达**：CSI Radar实现无设备运动检测
- **TSN（时间敏感网络）**：订阅版提供无线TSN特性，片上总线ping RTT低至300µs
- **调试**：dmesg日志 + Xilinx ILA探针FPGA状态机

---

## 社区、开发与许可

- **许可证**：AGPL-3.0开源（部分模块GPL-2.0/BSD），第三方模块需自行合规
- **订阅版**（openwifi+ / Wireless TSN）提供：
  - FPGA源码
  - W-TSN特性
  - 优先支持
  - 商业许可
- **贡献**：贡献指南完善，讨论区活跃；18位贡献者，定期发布

---

## 局限性与未来展望

### 局限性
- 当前最佳吞吐50Mbps（非Wi-Fi 6/7级）
- 部分板卡5GHz功率较低
- 需遵守本地频谱法规（建议有线测试）

### 已知问题
- Vivado评估许可超时
- 特定板卡时序挑战

### 未来规划
- 持续推进802.11ax
- 更丰富感知功能
- 无线TSN商用化

---

## 总结

openwifi不仅是"开源Wi-Fi芯片"，更是SDR时代Wi-Fi研究的**革命性平台**。它将FPGA的可编程性与Linux生态完美融合，为教育、学术安全研究、下一代无线协议创新提供了前所未有的透明度和控制力。

**适用场景**：
- 复现商用Wi-Fi
- CSI感知实验
- 无线TSN开发
- 安全研究
- 无线协议创新

**一句话评价**：无论你是研究者、开发者还是安全爱好者，openwifi都是进入可编程无线世界的首选起点。

---

## 附录：原文章图片说明

微信公众号文章原包含以下图片（通过JavaScript动态加载，无法直接爬取）：

1. **系统架构概览**：openwifi系统架构图（左侧为Linux协议栈 + SDR驱动 + FPGA基带，右侧为典型硬件实物）
2. **硬件实物图**：PYNQ-Z1 + AD936x板卡运行openwifi，屏幕显示AP界面
3. **AntSDR E200板卡图**：低成本Zynq 7020 + AD9361板卡展示

如需查看原图，请访问：https://mp.weixin.qq.com/s/FzoCNPnRCSw8fVOm0gQbxg

---

> **版权说明**：本文仅供学习交流使用，内容版权归原作者/公众号所有。