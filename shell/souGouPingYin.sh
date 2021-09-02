#!/bin/bash
# Author: Linux猿
# date: 2021-05-26

# 安装 im-config fcitx
apt install -y im-config fcitx

# 设置 fcitx
im-config -n fcitx

# 下载搜狗拼音软件包
wget -P /tmp/ https://ime.sogoucdn.com/dl/index/1612260778/sogoupinyin_2.4.0.3469_amd64.deb

# 安装搜狗拼音软件包
apt install -y /tmp/sogoupinyin_2.4.0.3469_amd64.deb


