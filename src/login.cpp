//
// Created by yuri on 3/14/23.
//
// You may need to build the project (run Qt uic code generator) to get
// "ui_ligon.h" resolved

#include "../include/login.h"
#include "../ui/ui_login.h"
#include <QMessageBox>
#include <QMovie>

Login::Login(QWidget *parent)
    : QDialog(parent), ui(new Ui::login),
      mainWindow(new MainWindow(this, &db)),
      setting(new Setting) {
  // 默认登陆设置(图标，标题，背景之类的)
  setWindowIcon(QIcon(":/images/login/icon.png"));
  ui->setupUi(this);
  this->setWindowTitle("登陆");
  auto bg_login = new QMovie(":/images/login/back.gif", QByteArray(), this);
  bg_login->start();
  ui->bg->setMovie(bg_login);

  QFile file(":css/login.css",this);
  file.open(QFile::ReadOnly | QFile::Text);
  setStyleSheet(file.readAll());
  file.close();

  // 关闭窗口关闭按钮
  connect(ui->button_close, &QPushButton::clicked, [this]() {
    this->close();
  });


  // 最小化按钮
  connect(ui->button_min, &QPushButton::clicked, [this]() {
    this->showMinimized();
  });

  // 设置账号密码默认提示
  ui->username->lineEdit()->setPlaceholderText("QQ号码/手机/邮箱");
  ui->password->setPlaceholderText("密码");

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
  for (auto iter = user_info.begin(); iter != user_info.end(); iter++) {
    this->ui->username->addItem(iter.key());
  }

  /* -------------------------------  设置默认登陆密码 ------------------------------ */
  this->ui->password->setText(user_info.begin().value());
  connect(ui->button_login, &QPushButton::clicked, [=]() {
    if (user_info.find(ui->password->text()) == user_info.end()) {
      QMessageBox::critical(this, "错误", "密码错误捏!");
    } else {
      login();
    }
  });

  /* -------------------------------- 设置是否记住密码 -------------------------------- */
  ui->remember_password->setChecked((*setting->getSettings())["is_remember_password"]);
  connect(ui->remember_password, &QCheckBox::stateChanged, [=](int state) {
    (*setting->getSettings())["is_remember_password"] = (state == Qt::Checked);
  });
}

// 登陆数据库，返回是否登陆成功
bool Login::loginDataBase() {
  auto mysql = this->setting->getMysql();
  db = QSqlDatabase::addDatabase("QMYSQL");
  db.setHostName(mysql->host_name);
  db.setPort(mysql->port);
  db.setUserName(ui->username->lineEdit()->text());
  db.setPassword(ui->password->text());
  return db.open();
}

// 登陆函数
void Login::login() {
  this->hide();
  mainWindow->show();
}
