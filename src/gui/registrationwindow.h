#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QWidget>
#include "ui_registrationwindow.h"

namespace Ui {
class RegistrationWindow;
}

class RegistrationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrationWindow(QWidget *parent = nullptr);
    ~RegistrationWindow();

private slots:
    void onConfirmClicked();
    void onCancelClicked();

private:
    Ui::RegistrationWindow *ui;
};

#endif // REGISTRATIONWINDOW_H

