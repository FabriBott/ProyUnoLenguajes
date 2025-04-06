#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlineedit.h>
#include <qpushbutton.h>
#include "../../include/client.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void handleAuthResult(bool success, const QString& message);
    void onErrorOccurred(const QString& error);

private:
    Ui::MainWindow *ui;
    Client *client;
    QPushButton *btnLogin;
    QPushButton *btnRegister;
    QLineEdit *inputUsername;
    QLineEdit *inputPassword;

    void setupUI();
    void setupConnections();
};

#endif // MAINWINDOW_H

