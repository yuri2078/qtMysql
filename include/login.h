//
// Created by yuri on 3/14/23.
//

#ifndef YURI_LOGIN_H
#define YURI_LOGIN_H

#include <QDialog>
#include "./mainwindow.h"
#include "./Setting.h"
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class login; }
QT_END_NAMESPACE

class Login : public QDialog {
Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login() override;

private:
    Ui::login *ui;
    MainWindow *mainWindow; // 主窗口指针
    Setting *setting; // 配置文件指针
    QSqlDatabase db; // 数据库文件

    void login_init(); // 登陆初始化
    bool loginDataBase(); // 登陆数据库
    void login(); // 密码正确，登陆函数
};


#endif //YURI_LOGIN_H
