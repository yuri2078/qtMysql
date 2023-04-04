#include "../include/mylineedit.h"
// #include <qnamespace.h>

MyLineEdit::MyLineEdit(QWidget *parent) : QLineEdit(parent) {
  setStyleSheet("MyLineEdit {  \
                  border: 1px solid pink;  \
                  border-bottom: 2px solid #999999;  \
                  background-color: transparent;  \
                  font-size: 18px;  \
                  color: #333333;  \
                }  \
                MyLineEdit:hover {  \
                  border-bottom: 2px solid #3EB1E8;   \
                }  \
                MyLineEdit:focus {  \
                  border-bottom: 2px solid #5EBD3E;  \
                }                                         \
  ");
}

void MyLineEdit::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Return && !text().isEmpty()) {
    qDebug() << text();
    emit sendMessage();
  } else {
    QLineEdit::keyPressEvent(event);
  }
}