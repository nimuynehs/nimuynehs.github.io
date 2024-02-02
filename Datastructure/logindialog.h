#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_loginB_clicked();

    void on_exitB_clicked();

signals:
    void showmenu();
    void quit();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
