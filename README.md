### 项目版本和环境

1. QtCreator
2. Cmake-3.26.4
3. Qt5.12.9 MSVC2017 64bit



其中Doxygen中包含代码文档。

Release-1.0.0将可执行文件进行了打包，可直接运行。



**注意：**

在命令行输入时，要一段一段的输入。

比如输入：

trapezoid -name=2d_geometry -width=4 -height=2 -theta=30
extrude -name=3d_geometry -source=2d_geometry -distance=0.5
show

这三段命令，应输入一段后。删除文本后，再次输入。