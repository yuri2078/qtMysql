#include "../include/qmysql.h"
#include <QDebug>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <QMessageBox>
#include <QSqlError>

QMysql::QMysql() :
  QSqlDatabase("QMYSQL") {
  sqlInit();
}

void QMysql::sqlInit() {
  map[QMYSQL::showTables] = "show tables;";
  map[QMYSQL::showDatabases] = "show databases;";
}

QSqlQuery QMysql::exec(const QMYSQL &sql) {
  return exec(map[sql]);
}

QSqlQuery QMysql::exec(const QString &sql) {
  QSqlQuery query(*this);
  if (!isOpen() || query.exec(sql) == false) {
     QMessageBox::critical(new QWidget(), "错误", "查询数据库时发生错误：" + query.lastError().text());
  }
  return query;
}