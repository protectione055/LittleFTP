# 基于TCP的FTP：LittleFTP

包括了完整的VS项目，可以在VS2019下直接打开并编译源码。

命令行下用`gcc server.c -lwsock32`/`gcc client.c -lwsock32`分别对服务器和客户端进行编译。

目前只实现了文件的传输部分，还有很多需要完善的地方。
预计下个版本会加入目录切换功能。
