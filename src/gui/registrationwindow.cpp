#include "registrationwindow.h"
#include "ui_registrationwindow.h"
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

RegistrationWindow::RegistrationWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistrationWindow)

{
    ui->setupUi(this);

    // Set the same background and style for the register window
    this->setStyleSheet("background-color: #A7C7E7;");

    // Create the main container for the register window
    QWidget *container = new QWidget(this);
    container->setStyleSheet("background-color: white; border-radius: 15px;");
    container->setFixedSize(300, 250); // Adjust as necessary for your design

    // Set up the form elements, similar to MainWindow
    QLabel *labelUsername = new QLabel("Username:", this);
    QLabel *labelPassword = new QLabel("Password:", this);
    QLineEdit *inputUsername = new QLineEdit(this);
    QLineEdit *inputPassword = new QLineEdit(this);
    QPushButton *btnConfirm = new QPushButton("Confirm", this);
    QPushButton *btnCancel = new QPushButton("Cancel", this);

    // Style the input fields and buttons
    inputUsername->setStyleSheet("border: 1px solid gray; border-radius: 5px; padding: 5px;");
    inputPassword->setStyleSheet("border: 1px solid gray; border-radius: 5px; padding: 5px;");
    inputPassword->setEchoMode(QLineEdit::Password);

    QString buttonStyle = "background-color: #A7C7E7; border-radius: 10px; padding: 8px;";
    btnConfirm->setStyleSheet(buttonStyle);
    btnCancel->setStyleSheet(buttonStyle);

    // Create layout for form elements
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(10);
    layout->addWidget(labelUsername);
    layout->addWidget(inputUsername);
    layout->addWidget(labelPassword);
    layout->addWidget(inputPassword);
    layout->addWidget(btnConfirm);
    layout->addWidget(btnCancel); // Cancel button below Confirm button

    container->setLayout(layout);

    // Create a main layout to center the container
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(container, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    // Connect buttons to respective slots
    connect(btnConfirm, &QPushButton::clicked, this, &RegistrationWindow::onConfirmClicked);
    connect(btnCancel, &QPushButton::clicked, this, &RegistrationWindow::onCancelClicked);

}

RegistrationWindow::~RegistrationWindow()
{
    delete ui;
}

void RegistrationWindow::onConfirmClicked()
{
    QMessageBox::information(this, "Register", "Registration successful!");
    // You can add further logic to handle registration (e.g., saving user data)
}

void RegistrationWindow::onCancelClicked()
{
    this->close();  // Close the register window when Cancel is clicked
}

