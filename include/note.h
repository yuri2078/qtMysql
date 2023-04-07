#ifndef NOTE_H
#define NOTE_H

#include <QTextEdit>
#include <QFile>

class Note : public QTextEdit {
  Q_OBJECT

public:
  Note(QWidget *parent = nullptr);
  ~Note();

  QString file_name;
  bool isSave;
  QTextStream *data;
  QFile file;

  void keyPressEvent(QKeyEvent *e) override;
  bool open(const QString &file_name , QIODeviceBase::OpenMode flag = QIODevice::ReadWrite | QIODevice::Text);

  

public:
  void saveFile();
  void openFile();

};

#endif