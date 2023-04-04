//
// Created by yuri on 3/15/23.
//

#ifndef YURI_SETTING_H
#define YURI_SETTING_H

#include <QFile>
#include <QJsonDocument>
#include <QVariantMap> // 可以存储任何数据的map 容器

/*
读取 设置 json 数据的类
*/

class Setting {
public:
  // mysql 结构体
  typedef struct {
    int port; // 登陆端口号
    QString host_name; // 登陆用户名
    // 用户名 / 密码
    QMap<QString, QString> user_info;
  } Mysql;

public:
  explicit Setting();
  ~Setting();

  bool setDebug(bool = false);

  bool getFromFile(const QString &file_name);
  void close();

  Mysql *getMysql(); // 获取mysql 数据
  QMap<QString, int> *getLoginUi(); // 获取登陆ui设置
  QMap<QString, bool> *getSettings(); // 获取系统设置数据

private:
  QVariantMap *json;            // 保存的json文件
  Mysql *mysql;                 // 保存mysql数据结构
  QMap<QString, int> *login_ui;  // 保存login_ui 数据结构
  QMap<QString, bool> *settings; // 保存系统设置

  bool file_is_open; //
  bool is_debug; // 是否是debug 模式
};

#endif // YURI_SETTING_H
