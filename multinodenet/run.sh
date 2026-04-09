#!/bin/bash
# MultiNodeNet 快速启动脚本

source /home/sfm/omnetpp-6.2.0/setenv
./out/clang-release/MultiNodeNet -u Qtenv -f simulations/omnetpp.ini -n src:simulations
