#!/bin/bash
# EnhancedNet 命令行运行脚本（Cmdenv）

# 加载OMNeT++环境
source /home/sfm/omnetpp-6.2.0/setenv

# 使用Cmdenv命令行界面运行
./out/clang-release/EnhancedNet -u Cmdenv -f simulations/omnetpp.ini -n src:simulations
