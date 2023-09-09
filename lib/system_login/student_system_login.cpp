#include "student_system_login.h"
#include "./ui_student_login.h"

#include <QFile>

StudentSystemLogin::StudentSystemLogin(QWidget *parent) :
  QWidget(parent), ui(new Ui::StudentSystemLogin) {
  ui->setupUi(this);
  setWindowTitle("system_login");
  init();
}

StudentSystemLogin::~StudentSystemLogin() {
  delete ui;
}

void StudentSystemLogin::init() {
  QFile file(":css/system_login.css");
  file.open(QFile::ReadOnly);
  setStyleSheet(file.readAll());
  file.close();

  connect(ui->login, &QPushButton::clicked, this, &StudentSystemLogin::login);
}


void StudentSystemLogin::login() {
  emit readyLogin(ui->username->text(), ui->username->text());
}

void StudentSystemLogin::registered() {

}
