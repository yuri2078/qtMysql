#ifndef NOTE_H
#define NOTE_H

#include <QTextEdit>
#include <QFile>

// 自定义 笔记 类型
class Note : public QTextEdit {
  Q_OBJECT

public:
  Note(QWidget *parent = nullptr);
  ~Note();

  QString file_name; // 文件名
  bool isSave; // 当前文件是否保存
  QTextStream *data;
  QFile file;

  void keyPressEvent(QKeyEvent *e) override;
  bool open(const QString &file_name, QIODeviceBase::OpenMode flag = QIODevice::ReadWrite | QIODevice::Text);

public:
  void saveFile(bool new_file = true);
  void openFile();
};

#endif