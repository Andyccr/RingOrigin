---
title: Linux解压/压缩的大集合
id: 514
date: 2020-03-13T22:02:51+08:00
---


此文章将介绍 如何在Linux命令行下，压缩，解压，打包一个或多个文件

打包

*   .tar

假设有一个文件（夹）foo,要打包为archive.tar

tar cf archive.tar foo

假设有多个文件（夹）foo和bar ,要打包为archive.tar

tar cf archive.tar foo bar

压缩

.tar.gz

假设有文件（夹）foo和bar,要压缩archive.tar.gz

tar cfz archive.tar.gz foo bar

*   .tar.xz

假设有文件（夹）foo和bar,要压缩为archive.tar.xz1

tar cfz archive.tar.xz foo bar

*   .zip

假设有文件foo和bar,要压缩为archive.zip (！！注意，不适用于文件夹！！)

zip archive.zip foo bar

假设有文件夹foo和bar,要压缩为archive.zip

zip -r archive.zip foo bar

*   .7z

假设有文件（夹）foo和bar,要压缩为archive.7z

7z a archive.7z foo bar

解压

*   .tar.\*

解压/解包.tar.\*文件，假设为archive.tar.gz

tar xf archive.tar.gz

*   .7z

解压.7z文件，假设为archive.7z

7z e archive.7z

*   .zip

解压.zip文件，假设为archive.zip

unzip archive.zip