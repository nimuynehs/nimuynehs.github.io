#include "teamdialog.h"
#include "ui_teamdialog.h"

TeamDialog::TeamDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TeamDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Developers"));
    setWindowIcon(QIcon(":/new/SHU/SH.ico"));
}

TeamDialog::~TeamDialog()
{
    delete ui;
}

void TeamDialog::on_backc_clicked()
{
    this->hide();
    emit backmenu();
}

void TeamDialog::receivecaidan()
{
    this->show();
}
