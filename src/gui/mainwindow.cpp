#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "registrationwindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set window background color (pastel blue)
    this->setStyleSheet("background-color: #A7C7E7;");

    // Create the main container (white rounded rectangle)
    QWidget *container = new QWidget(this);
    container->setStyleSheet("background-color: white; border-radius: 15px;");
    container->setFixedSize(300, 250); // Increased height to fit Register button

    // Create username and password fields
    QLabel *labelUsername = new QLabel("Username:", this);
    QLabel *labelPassword = new QLabel("Password:", this);
    QLineEdit *inputUsername = new QLineEdit(this);
    QLineEdit *inputPassword = new QLineEdit(this);
    QPushButton *btnLogin = new QPushButton("Login", this);
    QPushButton *btnRegister = new QPushButton("Register", this);

    // Set styles for input fields
    inputUsername->setStyleSheet("border: 1px solid gray; border-radius: 5px; padding: 5px;");
    inputPassword->setStyleSheet("border: 1px solid gray; border-radius: 5px; padding: 5px;");
    inputPassword->setEchoMode(QLineEdit::Password);

    // Set style for buttons
    QString buttonStyle = "background-color: #A7C7E7; border-radius: 10px; padding: 8px;";
    btnLogin->setStyleSheet(buttonStyle);
    btnRegister->setStyleSheet(buttonStyle);

    // Create layout for form elements
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(10);
    layout->addWidget(labelUsername);
    layout->addWidget(inputUsername);
    layout->addWidget(labelPassword);
    layout->addWidget(inputPassword);
    layout->addWidget(btnLogin);
    layout->addWidget(btnRegister);

    container->setLayout(layout);

    // Create a main layout to center the container
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addStretch();
    mainLayout->addWidget(container, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    setCentralWidget(centralWidget);

    // Connect button clicks
    connect(btnLogin, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(btnRegister, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLoginClicked()
{
    QMessageBox::information(this, "Login", "Login button clicked!");
}

void MainWindow::onRegisterClicked()
{
    // Create and show the register window
    RegistrationWindow *registerWindow = new RegistrationWindow(this);
    registerWindow->show();
}

