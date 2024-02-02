#include "menudialog.h"
#include "ui_menudialog.h"
#include"logindialog.h"
#include"mainwindow.h"
#include <QApplication>
#include<QHBoxLayout>

MenuDialog::MenuDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MenuDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Menu"));// 设置窗体标题
    setWindowIcon(QIcon(":/new/SHU/SH.ico"));
}

MenuDialog::~MenuDialog()
{
    delete ui;
}


void MenuDialog::on_MapB_clicked()
{
    this->hide();
    emit showmainwindow();
}

void MenuDialog::on_ExitB_clicked()
{
    emit quit();
}

void MenuDialog::receivelogin()
{
    this->show();
}

void MenuDialog::receivemainwindow()
{
    this->show();
}

void MenuDialog::receiveteam()
{
    this->show();
}

void MenuDialog::receiveschool()
{
    this->show();
}

void MenuDialog::on_TeamB_clicked()
{
    this->hide();
    emit showteam();
}

void MenuDialog::on_MessageB_clicked()
{
    this->hide();

}

