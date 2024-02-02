#ifndef MENUDIALOG_H
#define MENUDIALOG_H

#include <QDialog>

namespace Ui {
class MenuDialog;
}

class MenuDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MenuDialog(QWidget *parent = nullptr);
    ~MenuDialog();

private slots:
    void on_MapB_clicked();
    void receivelogin();
    void receivemainwindow();
    void on_ExitB_clicked();
    void receiveteam();
    void receiveschool();

    void on_TeamB_clicked();

    void on_MessageB_clicked();

signals:
    void showmainwindow();
    void quit();
    void showteam();

private:
    Ui::MenuDialog *ui;

};

#endif // MENUDIALOG_H
