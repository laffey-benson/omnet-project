#!/bin/bash
# EnhancedNet 快速启动脚本

# 加载OMNeT++环境
source /home/sfm/omnetpp-6.2.0/setenv

# 使用Qtenv图形界面运行
./out/clang-release/EnhancedNet -u Qtenv -f simulations/omnetpp.ini -n src:simulations
