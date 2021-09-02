#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVector>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void initUI();
    void addItems();
    void getUserPasswd();
public slots:
    void clickItems();

private:
    Ui::Widget *ui;
    QLabel *title;
    QVector<QPushButton*> items;
//    QPushButton *btn1;
//    QPushButton *btn2;
//    QPushButton *btn3;
//    QPushButton *btn4;
    QTextEdit *edit;
    QMap<QString, QString>itemToPath;
    QString passwd;
};

#endif // WIDGET_H
