//
// Created by yuri on 3/15/23.
//

#ifndef YURI_SETTING_H
#define YURI_SETTING_H

#include <QJsonDocument>
#include <QVariantMap>
#include <QFile>

class Setting {
public:
    typedef struct {
        int port;
        QString host_name;
        // 用户名 / 密码
        QMap<QString, QString> user_info;
    }Mysql;

public:
    explicit Setting(const QString &file_name);
    ~Setting();
    bool setDebug(bool = false);
    Mysql * getMysql();
    QMap<QString, int> & getLoginUi();
    QMap<QString, bool> & getSettings();


private:
    QVariantMap *json; // 保存的json文件
    Mysql *mysql; // 保存mysql数据结构
    QMap<QString, int> login_ui;// 保存login_ui 数据结构
    QMap<QString, bool> settings; // 保存系统设置

    bool file_is_open; //
    bool is_debug;
};


#endif //YURI_SETTING_H
