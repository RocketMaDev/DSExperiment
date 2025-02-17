# DS Experiment HDU

这个仓库里的源代码是杭电网安学院的数据结构课程设计的作业。

[English version](README_en.md)

<!-- 别选CQ的课，要求很多 -->

## 特性

1. 所有代码均符合书上的思路，例如在静态查找中是逆序，在数组0处存放哨兵块。
2. 所有头文件均包含注释，讲述如何使用。
3. 所有测试案例编译时无错误、警告，运行时无内存泄露。
4. 可复用性、稳健性。代码考虑了较多edge case并且考虑如何在上层模块中使用。
   **但是不要在生产环境中使用！**
5. 标准化的项目结构。源码和头文件存储在`src/`中，对象文件存储在`build/`中，
   测试文件存储在`test/`中，最后的可执行程序在`./`下。
6. 采用GNU Make编译，方便快捷。
7. 纯C实现。

> 没有上传实验报告是刻意的，我也不会上传。请先学会编译这个项目。

## 编译

所有代码在Arch Linux + gcc的环境下顺利通过所有测试，但未使用过其他环境。
请注意，代码中存在gcc特有指令，VS不一定能顺利编译。

使用GNU Make构建系统，使用`make TARGET=$EXE`来编译目标，目标可以从`Makefile`中找到。
默认开启`-O2`编译，如果想要调试可以使用`make TARGET=$EXE DEBUG=1`开启调试符号。
此时优化等级会降至默认级别。

想要编译一整个实验的相关程序，可以使用`make experiment$i`，`i`可以是1-5，
具体章节生成的对应的程序可以在`Makefile`中找到。

想要清除依赖（`.o`）而不清除生成的程序（有些代码生成的`.o`文件可能不一致），
可以使用`make clean_deps`，想要清除所有程序与依赖，可以使用`make clean`。

## “我有疑问”

当发现编译时无法正常工作，有bug，想要给其他平台做适配等，请参阅[贡献](CONTRIBUTING.md)一节。

## 打包源代码

由于模块的高复用性，后期实验的模块往往需要打包整一个项目。
如果不想每个实验都上交一样的代码，可以检出到对应的标签，打包我在每个实验所上交的代码。
~~不过估计老师也不会看的~~

例如使用`git checkout experiment1`可以回溯到实验1的代码，在使用zip等工具打包后，
使用`git checkout main`回到主线头部。

## 实验具体任务

实验的所有任务我打包放在Releases里了，请自行解压。除了最后一个实验，其他实验都验收通过了，
最后一个实验没有安排验收课，因此说不清楚验收情况请知悉。

## 致谢

排序模块用到了[@zhouxumeng213](https://github.com/zhouxumeng213/sort)的代码，
关于avl树优化验证来自[这篇知乎上的文章](https://zhuanlan.zhihu.com/p/454080520)。

## 博客

看看这个项目的博客： https://rocketma.dev/2025/02/01/datastructure/

## License

本项目基于GPLv3分发，详情请见[LICENSE](LICENSE)。
