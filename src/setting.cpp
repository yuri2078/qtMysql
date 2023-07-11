//
// Created by yuri on 3/15/23.
//

#include "../include/setting.h"

Setting::Setting() {
  settings = nullptr;
  login_ui = nullptr;
  json = nullptr;
  mysql = nullptr;

  file_is_open = false;

  // 默认不开启debug模式
  this->is_debug = false;
}

Setting::~Setting() {
  writeJson();
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
  fileName = file_name;
  if (file_is_open) {
    writeJson();
  }

  QFile file(file_name); // 打开文件
  close();               // 先关闭，并且进行初始化

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
    if (!file_is_open) {
      qDebug() << "文件未打开!";
    }
    return mysql;
  }

  mysql = new Mysql;
  QVariantMap mysql_map = json->value("MySql").toMap();
  mysql->port = mysql_map["port"].toInt();
  mysql->host_name = mysql_map["hostname"].toString();

  auto user_info = mysql_map["user_info"].toMap();
  for (auto iter = user_info.begin(); iter != user_info.end(); iter++) {
    mysql->user_info[iter.key()] = iter.value().toString();
  }
  if (is_debug) {
    qDebug() << "port -> " << mysql->port;
    qDebug() << "host_name -> " << mysql->host_name;
    for (auto iter = user_info.begin(); iter != user_info.end(); iter++) {
      qDebug() << "username -> " << iter.key() << ", password -> "
               << iter.value().toString();
    }
  }

  return mysql;
}

// 返回loginui 的指针
QMap<QString, int> *Setting::getLoginUi() {
  // 如果文件没有打开，或者他已经被分配过内存了，就直接返回数据
  if (login_ui != nullptr || !file_is_open) {
    if (!file_is_open) {
      qDebug() << "文件未打开!";
    }
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

QMap<QString, bool> *Setting::getSettings() {
  // 如果文件没有打开，或者他已经被分配过内存了，就直接返回数据
  if (settings != nullptr || !file_is_open) {
    if (!file_is_open) {
      qDebug() << "文件未打开!";
    }
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

// 将数据更新进json文件里
void Setting::writeJson() {
  // 先获取所有数据
  getSettings();
  getMysql();
  getLoginUi();

  QJsonObject main_obj, set_ojb, sql_ojb, login_obj, user_obj;
  for (auto begin = settings->begin(); begin != settings->end(); begin++) {
    set_ojb.insert(begin.key(), begin.value());
  }

  sql_ojb.insert("hostname", mysql->host_name);
  sql_ojb.insert("port", mysql->port);
  for (auto begin = mysql->user_info.begin(); begin != mysql->user_info.end(); begin++) {
    user_obj.insert(begin.key(), begin.value());
  }
  sql_ojb.insert("user_info", user_obj);

  for (auto begin = login_ui->begin(); begin != login_ui->end(); begin++) {
    login_obj.insert(begin.key(), begin.value());
  }

  main_obj.insert("Login_ui", login_obj);
  main_obj.insert("Settings", set_ojb);
  main_obj.insert("MySql", sql_ojb);

  QFile file(fileName);
  file.open(QIODevice::ReadWrite);
  QJsonDocument doc(main_obj);
  file.resize(0);
  file.write(doc.toJson());
  file.close();
}
