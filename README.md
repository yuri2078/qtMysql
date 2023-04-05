# yuriQt

> 这是我本人的简单qt项目，是对简单qt控件的整理应用
>
> 本项目 基于 `Qt6` 进行开发

## 目录结构

```ABAP
├── CMakeLists.txt
├── README.md
├── images
├── images.qrc
├── include
│   ├── login.h -- 登陆
│   ├── mainwindow.h -- 登陆后主界面
│   ├── myclient.h -- MyTcpSocket
│   ├── mylineedit.h -- MyLineEdit
│   ├── myserver.h -- MyTcpServer
│   └── setting.h
├── main.cpp -- main.cpp
├── mainWindow.css
├── settings.json -- 配置文件
├── src
│   ├── login.cpp
│   ├── mainwindow.cpp
│   ├── myclient.cpp
│   ├── mylineedit.cpp
│   ├── myserver.cpp
│   └── setting.cpp
├── ui
│   ├── login.ui -- 登陆界面
│   └── mainwindow.ui -- 登陆成功主界面
```



## 实现功能

1. MySQL 数据库登陆
2. c/s 通信测试
