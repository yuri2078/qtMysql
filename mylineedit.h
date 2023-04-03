#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <qtmetamacros.h>

class MyLineEdit : public QLineEdit {
public:
  
  Q_OBJECT
  
public:
  
  MyLineEdit(QWidget *parent = nullptr);

public:
  void keyPressEvent(QKeyEvent *event) override;

signals:
  
  void sendMessage();
};

#endif