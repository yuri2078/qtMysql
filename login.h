//
// Created by yuri on 3/14/23.
//

#ifndef YURI_LOGIN_H
#define YURI_LOGIN_H

#include <QDialog>
#include "mainwindow.h"
#include "Setting.h"
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
    MainWindow *mainWindow;
    Setting *setting;
    QSqlDatabase db;

    void login_init();
    bool loginDataBase();
};


#endif //YURI_LOGIN_H
