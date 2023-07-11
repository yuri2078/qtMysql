#ifndef QMYSQL_H
#define QMYSQL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QHash>

enum QMYSQL { showDatabases,
              showTables };

class QMysql : public QSqlDatabase {
public:
  QMysql();
  virtual ~QMysql() = default;

  QSqlQuery exec(const QMYSQL &sql);
  QSqlQuery exec(const QString &sql);

private:
  QHash<QMYSQL, QString> map;

  void sqlInit();
};

#endif