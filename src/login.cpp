//
// Created by yuri on 3/14/23.
//
// You may need to build the project (run Qt uic code generator) to get
// "ui_ligon.h" resolved

#include "../include/login.h"
#include "../ui/ui_login.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QMovie>
#include <qsqlerror.h>

Login::Login(QWidget *parent)
    : QDialog(parent), ui(new Ui::login), mainWindow(new MainWindow(this, &db)),
      setting(new Setting) {
  // 默认登陆设置(图标，标题，背景之类的)
  setWindowIcon(QIcon(":/images/login/icon.png"));
  ui->setupUi(this);
  this->setWindowTitle("登陆");
  auto bg_login = new QMovie(":/images/login/back.gif", QByteArray(), this);
  bg_login->start();
  ui->bg->setMovie(bg_login);
  setWindowFlags(Qt::FramelessWindowHint | windowFlags());

  QFile file(":css/login.css", this);
  file.open(QFile::ReadOnly | QFile::Text);
  setStyleSheet(file.readAll());
  file.close();

  // 关闭窗口关闭按钮
  connect(ui->button_close, &QPushButton::clicked, [this]() { this->close(); });

  // 最小化按钮
  connect(ui->button_min, &QPushButton::clicked, [this]() { this->showMinimized(); });

  // 设置账号密码默认提示
  // ui->username->setPlaceholderText("QQ号码/手机/邮箱");
  ui->username->setEnableLabel(true);
  ui->username->label->setText("QQ号码/手机/邮箱");
  ui->password->setEnableLabel(true);
  ui->password->label->setText("密码");

  // 退出登陆 槽函数关联
  connect(this->mainWindow, &MainWindow::loginEnd, [this]() {
    this->show();
    mainWindow->hide();
  });

  if (setting->getFromFile(QCoreApplication::applicationDirPath().remove("build") + "settings.json") == false) {
    qDebug() << "配置文件打开失败!";
  }
  login_init();
  setModal(true);
}

/* ---------------------------------- 析构函数 ---------------------------------- */
 
Login::~Login() {
  delete ui;
  delete setting;
  db.close();
}

// 登陆初始化
void Login::login_init() {
  // 获取配置文件中的账号和密码
  auto &user_info = setting->getMysql()->user_info;
  // for (auto iter = user_info.begin(); iter != user_info.end(); iter++) {
  //   this->ui->username->addItem(iter.key());
  // }

  /* -------------------------------  设置默认登陆密码 ------------------------------ */
  connect(ui->button_login, &QPushButton::clicked, [=]() {
    if(setting->getSettings()->find("is_auto_login").value() || loginDataBase()) {
      login();
    } else {
      QMessageBox::critical(this, "错误", db.lastError().text());
    }
  });

  /* -------------------------------- 设置是否记住密码 -------------------------------- */
  ui->remember_password->setChecked((*setting->getSettings())["is_remember_password"]);
  ui->auto_login->setChecked((*setting->getSettings())["is_auto_login"]);
  connect(ui->remember_password, &QCheckBox::stateChanged, [=](int state) {
    (*setting->getSettings())["is_remember_password"] = (state == Qt::Checked);
  });
  connect(ui->auto_login, &QCheckBox::stateChanged, [=](int state) {
    (*setting->getSettings())["is_auto_login"] = (state == Qt::Checked);
  });
}

// 登陆数据库，返回是否登陆成功
bool Login::loginDataBase() {
  auto mysql = this->setting->getMysql();
  db.setHostName(mysql->host_name);
  db.setPort(mysql->port);
  db.setUserName(ui->username->text());
  db.setPassword(ui->password->text());
  return db.open();
}

void Login::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    is_move = true;
    my_pos = event->pos();
    return;
  }

  QDialog::mousePressEvent(event);
}

void Login::mouseMoveEvent(QMouseEvent *event) {
  if (is_move) {
    move(event->pos() - my_pos + pos());
  }
}

void Login::mouseReleaseEvent(QMouseEvent *event) {
  is_move = false;
  QDialog::mouseReleaseEvent(event);
}

// 登陆函数
void Login::login() {
  this->hide();
  mainWindow->show();
}
