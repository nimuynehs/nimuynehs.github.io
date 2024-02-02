#include "logindialog.h"
#include "ui_logindialog.h"
#include<QMessageBox>
#include<QHBoxLayout>
#include<menudialog.h>
#include"mainwindow.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Welcome"));// 设置窗体标题
    setWindowIcon(QIcon(":/new/SHU/SH.ico"));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginB_clicked()
{
    if(1)
    {
        this->hide();
        emit showmenu();
    }
}

void LoginDialog::on_exitB_clicked()
{
    emit quit();
}



