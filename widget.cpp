#include "widget.h"
#include "ui_widget.h"
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

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

    QFile file("/home/linuxy/Lact/shell/config.json");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "File open error";
    }

    QByteArray data=file.readAll();
    file.close();

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(data, &jsonError);

    // 解析未发生错误
    if (!document.isNull() && (jsonError.error == QJsonParseError::NoError)) {
        if (document.isObject()) { // JSON 文档为数组
            QJsonObject object = document.object();  // 转化为对象
            object.begin();
            for(auto iter = object.begin(); iter != object.end(); ++iter) {
                qDebug()<<iter.key()<<endl;
                qDebug()<<iter.value().toString()<<endl;
                QPushButton *btn = new QPushButton(iter.key());
                btn->setFixedSize(105, 35);
                items.push_back(btn);
                itemToPath[iter.key()] = iter.value().toString();
                connect(btn, SIGNAL(clicked()), this, SLOT(clickItems()));
            }
        }
    }

    QPushButton *btn = new QPushButton("全部配置");
    itemToPath["全部配置"] = "./shell/all.sh";
    btn->setFixedSize(105, 35);
    items.push_back(btn);
    connect(btn, SIGNAL(clicked()), this, SLOT(clickItems()));
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
    QString cmd = QString("/bin/bash /home/linuxy/Lact/shell/") + path;


        FILE* fp = popen(cmd.toLocal8Bit().data(), "r");
        if (nullptr != fp) {
            char buf[1024] = {0};
            char result[2000] = {0};

            while (fgets(buf, sizeof(buf), fp))
            {
                strcat(result, buf);
                if (strlen(result) > sizeof(buf))
                {
                    break;
                }
                edit->insertPlainText(QString(buf));
            }
            //edit->insertPlainText(QString(output));
            //m_pEdtResult->setPlainText(tr("%1").arg(result));
            pclose(fp);         // 记得释放资源
            fp = nullptr;
        }
}

Widget::~Widget()
{
    delete ui;
}
