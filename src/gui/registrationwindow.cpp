#include "registrationwindow.h"
#include "ui_registrationwindow.h"
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
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
    container->setFixedSize(300, 200);

    // Set up the form elements
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

    // ✅ Create horizontal layouts for each label-input pair
    QHBoxLayout *usernameLayout = new QHBoxLayout();
    usernameLayout->addWidget(labelUsername);
    usernameLayout->addWidget(inputUsername);

    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->addWidget(labelPassword);
    passwordLayout->addWidget(inputPassword);

    // Align inputs properly
    labelUsername->setFixedWidth(85);
    labelPassword->setFixedWidth(85);

    // ✅ Horizontal layout for buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(btnCancel);
    buttonLayout->addWidget(btnConfirm);

    // Create main vertical layout
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(8);
    layout->addLayout(usernameLayout);
    layout->addLayout(passwordLayout);
    layout->addLayout(buttonLayout);

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
}

void RegistrationWindow::onCancelClicked()
{
    this->close();
}
