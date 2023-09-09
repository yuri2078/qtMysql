#ifndef MYTABLE_H
#define MYTABLE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MyTable;
}
QT_END_NAMESPACE

class MyTable : public QWidget {
  Q_OBJECT

public:
  MyTable(QWidget *parent = nullptr);
  ~MyTable();

private:
  Ui::MyTable *ui;
};
#endif // DIALOG_H
