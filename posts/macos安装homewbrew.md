---
title: MacOS安装HomewBrew
tags: ['[]']
id: 738
date: 2024-10-28T09:53:40+08:00
---


官方命令：

```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

镜像源：

```
/bin/zsh -c "$(curl -fsSL https://gitee.com/cunkai/HomebrewCN/raw/master/Homebrew.sh)"
```

下载后(终端操作)：
```
sudo vim ~/.bash\_profile
```
```
export PATH="/opt/homebrew/bin:$PATH"
```

在:wq保存退出后
```
source ~/.bash\_profile
```