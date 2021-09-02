#include "widget.h"
#include "ui_widget.h"
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QInputDialog>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    initUI();
    //setBaseSize(10000, 10000);
}

/**
 * @brief Widget::addItems
 */
void Widget::addItems() {
    QPushButton *btn0 = new QPushButton("搜狗输入法");
    QPushButton *btn1 = new QPushButton("微信");
    QPushButton *btn2 = new QPushButton("QQ");
    QPushButton *btn3 = new QPushButton("远程连接");
    QPushButton *btn4 = new QPushButton("全部配置");

    items.push_back(btn0);
    items.push_back(btn1);
    items.push_back(btn2);
    items.push_back(btn3);
    items.push_back(btn4);

    //
    QVector<QString> btnPath({"sougou.sh", "wechat.sh", "qq.sh", "remoteSSH.sh", "all"});

    for(int i = 0; i < items.size(); ++i) {
        itemToPath[items[i]->text()] = btnPath[i];
        items[i]->setFixedSize(105, 35);
        connect(items[i], SIGNAL(clicked()), this, SLOT(clickItems()));
    }
}

void Widget::initUI() {
    this->resize(500, 500);
    QHBoxLayout *titleLayout = new QHBoxLayout;
    title = new QLabel(this);
    QFont ft;
    ft.setPointSize(14);
    title->setFont(ft);
    title->setAlignment(Qt::AlignCenter);
    title->setText("Ubuntu 自动化配置工具");
    titleLayout->addWidget(title);
    titleLayout->setContentsMargins(0, 30, 0, 80);
    //setContentsMargins(int left， int top， int right， int bottom)

    QGridLayout *itemLayout = new QGridLayout;
    itemLayout->setVerticalSpacing(60);
    itemLayout->setHorizontalSpacing(30);
    addItems();

    int x = 0, y = 0;
    for(int i = 0; i < items.size(); ++i) {
        itemLayout->addWidget(items[i], x, y);
        y++;
        if(y >= 3) {
            y = 0;
            x++;
        }
    }
    QHBoxLayout *outputLayout = new QHBoxLayout;
    outputLayout->setContentsMargins(0, 70, 0, 30);

    edit = new QTextEdit();
    edit->setReadOnly(true);
    outputLayout->addWidget(edit);
    QVBoxLayout *globalLayout = new QVBoxLayout;
    globalLayout->addLayout(titleLayout);
    globalLayout->addLayout(itemLayout);
    globalLayout->addLayout(outputLayout);

    globalLayout->setContentsMargins(40,20,40,10);
    this->setLayout(globalLayout);
}

/**
 * 暂时不使用，需要使用 root 执行软件
 * @brief Widget::getUserPasswd
 */
void Widget::getUserPasswd() {
    bool flag = false;
    QString passwdNew = QInputDialog::getText(this, "获取执行权限",
                                             "请输入密码：",
                                              QLineEdit::Password,
                                             "",
                                             &flag
                                             );

    if (flag && !passwdNew.isEmpty()) {
        if(!passwd.length() || passwd != passwdNew) {
            passwd = passwdNew;
        }
    }
}
void Widget::clickItems() {
    edit->clear();
    QPushButton *tmpBtn = qobject_cast<QPushButton*>(sender());
    QString path= itemToPath[tmpBtn->text()];
    QString cmd = QString("/bin/bash ../") + path;

    QProcess process;

    process.start(cmd);
    //等待命令执行结束
    process.waitForFinished();
    //获取命令执行的结果
    QByteArray output = process.readAllStandardOutput();
    QByteArray err = process.readAllStandardError();
    edit->insertPlainText(QString(output));
}

Widget::~Widget()
{
    delete ui;
}
