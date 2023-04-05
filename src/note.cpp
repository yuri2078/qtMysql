#include "../include/note.h"

#include <QFileDialog>
#include <QKeyEvent>

Note::Note(QWidget *parent) : QTextEdit(parent) {
  QFile file(":css/note.css");
  file.open(QIODevice::ReadOnly);
  setStyleSheet(file.readAll());
  file.close();
  move(50, 50);
}

void Note::saveFile() {
  QString file_name = QFileDialog::getSaveFileName(this, "请打开一个文件", "/home/yuri", "");
  QFile file(file_name, this);
  file.open(QFile::WriteOnly);
  if (file.isOpen()) {
    file.write(toPlainText().toUtf8());
  } else {
    qDebug() << "打开失败 -> " << file.error();
  }
  file.close();
}

void Note::openFile() {
  QString file_name = QFileDialog::getOpenFileName(this, "请选择保存的位置", "/home/yuri", "");
  QFile file(file_name, this);
  file.open(QFile::ReadOnly);
  if (file.isOpen()) {
    setPlainText(file.readAll());
  } else {
    qDebug() << "打开失败 -> " << file.error();
  }
  file.close();
}

void Note::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Tab) {
    insertPlainText(" ");
  } else {
    QTextEdit::keyPressEvent(event);
  }
}