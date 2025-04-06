#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QWidget>
#include "ui_registrationwindow.h"
#include "../../include/client.hpp"

namespace Ui {
class RegistrationWindow;
}

class RegistrationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrationWindow(Client *client, QWidget *parent = nullptr);
    ~RegistrationWindow();

private slots:
    void onConfirmClicked();
    void onCancelClicked();

private:
    Ui::RegistrationWindow *ui;
    Client *m_client;  //client pointer
};

#endif // REGISTRATIONWINDOW_H
