//
// Created by yuri on 3/15/23.
//

#include "../include/Setting.h"

Setting::Setting() {
  settings = nullptr;
  login_ui = nullptr;
  json = nullptr;
  mysql = nullptr;
}

Setting::~Setting() {
  close();
}

// 如果不是nullptr 就进行delete,并置为null
void Setting::close() {
  if (settings) {
    settings = nullptr;
    delete settings;
  }

  if (login_ui) {
    login_ui = nullptr;
    delete login_ui;
  }

  if (json) {
    delete json;
    json = nullptr;
  }

  if (mysql) {
    delete mysql;
    mysql = nullptr;
  }
  
}

bool Setting::getFromFile(const QString &file_name) {
  QFile file(file_name); // 打开文件

  close(); // 先关闭，并且进行初始化

  // 默认不开启debug模式
  this->is_debug = false;

  // 打开文件
  file_is_open = file.open(QFile::ReadOnly | QIODevice::Text);
  if (!file_is_open) {
    qDebug() << "文件打开失败!";
    return false;
  } else {
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    this->json = new QVariantMap(doc.toVariant().toMap());
  }

  // 读取完毕关闭文件
  file.close();

  getMysql();
  getLoginUi();
  getSettings();
  return true;
}

Setting::Mysql *Setting::getMysql() {
  if (mysql != nullptr || !file_is_open) {
    return mysql;
  }

  mysql = new Mysql;
  QVariantMap mysql_map = json->value("MySql").toMap();
  mysql->port = mysql_map["port"].toInt();
  mysql->host_name = mysql_map["host_name"].toString();

  auto user_info = mysql_map["user_info"].toMap();
  for (auto iter = user_info.begin(); iter != user_info.end(); iter++) {
    mysql->user_info[iter.key()] = iter.value().toString();
  }
  if (is_debug) {
    qDebug() << "port -> " << mysql->port;
    qDebug() << "host_name -> " << mysql->host_name;
    for (auto iter = user_info.begin(); iter != user_info.end(); iter++) {
      qDebug() << "username -> " << iter.key() << ", password -> " << iter.value().toString();
    }
  }

  return mysql;
}

// 返回loginui 的指针
QMap<QString, int> *Setting::getLoginUi() {
  // 如果文件没有打开，或者他已经被分配过内存了，就直接返回数据
  if (login_ui != nullptr || !file_is_open) {
    return login_ui;
  }

  QVariantMap login_ui_map = json->value("Login_ui").toMap();
  login_ui = new QMap<QString, int>;
  for (auto iter = login_ui_map.begin(); iter != login_ui_map.end(); iter++) {
    (*login_ui)[iter.key()] = iter.value().toInt();
  }
  if (is_debug) {
    for (auto iter = login_ui_map.begin(); iter != login_ui_map.end(); iter++) {
      qDebug() << iter.key() << "  :  " << iter.value().toInt();
    }
  }
  return login_ui;
}

//
QMap<QString, bool> *Setting::getSettings() {
  // 如果文件没有打开，或者他已经被分配过内存了，就直接返回数据
  if (settings != nullptr || !file_is_open) {
    return settings;
  }

  QVariantMap settings_map = json->value("Settings").toMap();
  settings = new QMap<QString, bool>;
  for (auto iter = settings_map.begin(); iter != settings_map.end(); iter++) {
    (*settings)[iter.key()] = iter.value().toBool();
  }

  if (is_debug) {
    for (auto iter = settings_map.begin(); iter != settings_map.end(); iter++) {
      qDebug() << iter.key() << "  :  " << iter.value().toBool();
    }
  }
  return settings;
}

// 设置是不是debug 模式，如果是就会打印读取的数据
bool Setting::setDebug(bool flag) {
  this->is_debug = flag;
  return flag;
}
