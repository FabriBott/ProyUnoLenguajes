#ifndef OCCUPIEDPORT_H
#define OCCUPIEDPORT_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;

class occupiedport : public QDialog
{
    Q_OBJECT

public:
    explicit occupiedport(QWidget *parent = nullptr);
    ~occupiedport();

private slots:
    // Event handler for the Confirm button.
    void onConfirmButtonClicked();

private:
    QLabel *infoLabel;
    QLineEdit *portInput;
    QPushButton *confirmButton;
};

#endif // OCCUPIEDPORT_H


