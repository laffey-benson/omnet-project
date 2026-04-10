#!/bin/bash
# OMNeT++ 项目自动同步脚本
# 功能：自动添加所有改动、提交并推送到 GitHub

cd /home/sfm/文档/omnet-project

# 检查是否有 git 仓库
if [ ! -d .git ]; then
    echo "[$(date)] 错误：不是 Git 仓库"
    exit 1
fi

# 获取远程仓库信息
if ! git rev-parse --verify origin/main >/dev/null 2>&1; then
    echo "[$(date)] 错误：无远程仓库 origin"
    exit 1
fi

# 检查是否有改动
if git diff-index --quiet HEAD -- 2>/dev/null; then
    echo "[$(date)] 没有需要同步的改动"
    exit 0
fi

# 添加所有改动
git add .

# 获取改动文件列表
CHANGES=$(git diff --cached --name-only)
FILE_COUNT=$(echo "$CHANGES" | wc -l)

# 设置提交信息（如果有多于5个文件，用摘要）
if [ "$FILE_COUNT" -gt 5 ]; then
    COMMIT_MSG="自动同步更新 $(date '+%Y-%m-%d %H:%M')"
else
    # 列出前5个文件作为提交信息
    FILES=$(echo "$CHANGES" | head -5 | tr '\n' ', ')
    COMMIT_MSG="自动同步: $FILES"
fi

# 提交
git commit -m "$COMMIT_MSG"

# 推送
if git push origin main; then
    echo "[$(date)] 成功推送到 GitHub ($FILE_COUNT 个文件)"
else
    echo "[$(date)] 推送失败"
    exit 1
fi
