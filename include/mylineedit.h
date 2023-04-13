#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLabel>
#include <QLineEdit>
#include <QStateMachine>
#include <qtmetamacros.h>

// 自定义单行注释类,默认不带标签,需要设置显示标签并设置标签字体才会显示

class MyLineEdit : public QLineEdit {

    Q_OBJECT
    // Qt 中用来声明类的属性的宏。使用 Q_PROPERTY 宏可以将一个 C++ 对象成员（或者 getter 和 setter 函数）注册为一个 Qt 对象的属性
    // Q_PROPERTY(type name READ getter WRITE setter NOTIFY notifySignal)
    // type：属性的类型。
    // name：属性的名称。
    // getter：获取属性值的成员函数名称。
    // setter：设置属性值的成员函数名称。
    // notifySignal：当属性值发生变化时，发射的信号。

public:
    MyLineEdit(QWidget* parent = nullptr);

    QLabel* label;

    void focusOutEvent(QFocusEvent* event) override;
    void focusInEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void setEnableLabel(bool has_label);

  signals:
    
    void sendMessage();

private:
    QStateMachine* machine; // 状态机
    QState* fous_in; // 获得焦点
    QState* fous_out; // 失去焦点

    bool has_label;

    void setDefaultStyle();
};

#endif