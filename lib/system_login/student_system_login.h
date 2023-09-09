#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class StudentSystemLogin;
}
QT_END_NAMESPACE

class StudentSystemLogin : public QWidget {
  Q_OBJECT

public:
  StudentSystemLogin(QWidget *parent = nullptr);
  ~StudentSystemLogin();

signals:
    void readyLogin(const QString username, const QString password);

private:
  void init();
  void login();
  void registered();

private:
  Ui::StudentSystemLogin *ui;
};
#endif // DIALOG_H
