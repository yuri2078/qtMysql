//
// Created by yuri on 3/15/23.
//


#include "Setting.h"


Setting::Setting(const QString &file_name) {
    QFile file(file_name);
    file_is_open = file.open(QFile::ReadOnly | QIODevice::Text);
    if(!file_is_open){
        qDebug() << "文件打开失败!";
        return;
    } else {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        this->json = new QVariantMap(doc.toVariant().toMap());
        this->mysql = nullptr;
    }

    file.close();
    getMysql();
    getLoginUi();
    getSettings();
}


Setting::Mysql * Setting::getMysql() {
    if(mysql != nullptr) {
        return mysql;
    }
    mysql = new Mysql;
    QVariantMap mysql_map = json->value("MySql").toMap();
    mysql->port = mysql_map["port"].toInt();
    mysql->host_name = mysql_map["host_name"].toString();

    qDebug() << "port -> " << mysql->port;
    qDebug() << "host_name -> " << mysql->host_name;

    auto user_info = mysql_map["user_info"].toMap();
    for(auto iter = user_info.begin(); iter != user_info.end(); iter++) {
        mysql->user_info[iter.key()] =  iter.value().toString();
        qDebug() << "username -> " << iter.key() << ", password -> " << iter.value().toString();
    }
    return mysql;
}

Setting::~Setting() {
    delete json;
    delete mysql;
}

QMap<QString, int> &Setting::getLoginUi() {
    QVariantMap login_ui_map = json->value("Login_ui").toMap();
    for(auto iter = login_ui_map.begin(); iter != login_ui_map.end(); iter++) {
        login_ui[iter.key()] = iter.value().toInt();
        qDebug() <<  iter.key() << "  :  " << iter.value().toInt();
    }
    return login_ui;
}

QMap<QString, bool> &Setting::getSettings() {
    QVariantMap settings_map = json->value("Settings").toMap();
    for(auto iter = settings_map.begin(); iter != settings_map.end(); iter++) {
        settings[iter.key()] = iter.value().toBool();
        qDebug() <<  iter.key() << "  :  " << iter.value().toBool();
    }
    return settings;
}
