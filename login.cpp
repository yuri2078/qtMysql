//
// Created by yuri on 3/14/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ligon.h" resolved

#include "login.h"
#include "ui_login.h"
#include <QMovie>
#include <QMessageBox>

Login::Login(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::login),
        setting(new Setting(":/settings.json")),
                mainWindow(new MainWindow(this, &db)) {
    setWindowIcon(QIcon(":/images/login/icon.png"));
    ui->setupUi(this);
    this->setWindowTitle("登陆");
    auto bg_login = new QMovie(":/images/login/back.gif", QByteArray(), this);
    bg_login->start();
    ui->bg->setMovie(bg_login);

    connect(ui->button_close, &QPushButton::clicked, [this]() {
        this->close();
    });

    connect(ui->button_min, &QPushButton::clicked, [this]() {
        this->showMinimized();
    });

    ui->username->lineEdit()->setPlaceholderText("QQ号码/手机/邮箱");
    ui->password->setPlaceholderText("密码");

    login_init();

    connect(this->mainWindow, &MainWindow::loginEnd, [this]() {
        this->show();
        mainWindow->hide();
    });

    setModal(true);

}

Login::~Login() {
    delete ui;
    delete setting;
    db.close();
}

void Login::login_init() {
    auto &user_info = setting->getMysql()->user_info;
    for (auto iter = user_info.begin(); iter != user_info.end(); iter++) {
        this->ui->username->addItem(iter.key());
    }
    this->ui->password->setText(user_info.begin().value());
    connect(ui->button_login, &QPushButton::clicked, [=]() {
        if (user_info.find(ui->password->text()) == user_info.end()) {
            QMessageBox::critical(this, "错误", "密码错误捏!");
        } else {
            if (loginDataBase()) {
                login();
            } else {
                QMessageBox::critical(this, "错误", "登陆失败!");
            }
        }
    });

    ui->remember_password->setChecked(setting->getSettings()["is_remember_password"]);
    connect(ui->remember_password, &QCheckBox::stateChanged, [=](int state) {
        setting->getSettings()["is_remember_password"] = (state == Qt::Checked);
    });
}

bool Login::loginDataBase() {
    auto mysql = this->setting->getMysql();
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(mysql->host_name);
    db.setPort(mysql->port);
    db.setUserName(ui->username->lineEdit()->text());
    db.setPassword(ui->password->text());
    return db.open();
}

void Login::login() {
    this->hide();
    mainWindow->show();
}


