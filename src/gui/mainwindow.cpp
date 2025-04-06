#include "mainwindow.h"
#include "registrationwindow.h"
#include "ui_mainwindow.h"
#include "clientwindow.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client(new Client(this))
{
    ui->setupUi(this);
    setupUI();
    setupConnections();
}

void MainWindow::setupUI() {
    // Set window background color
    this->setStyleSheet("background-color: #407280;");

    // Create container widget
    QWidget *container = new QWidget(this);
    container->setStyleSheet("background-color: white; border-radius: 15px;");
    container->setFixedSize(300, 250);

    // Create and style UI elements
    inputUsername = new QLineEdit(this);
    inputPassword = new QLineEdit(this);
    inputPassword->setEchoMode(QLineEdit::Password);

    btnLogin = new QPushButton("Login", this);
    btnRegister = new QPushButton("Register", this);

    // Setup layouts
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(new QLabel("Username:"));
    layout->addWidget(inputUsername);
    layout->addWidget(new QLabel("Password:"));
    layout->addWidget(inputPassword);
    layout->addWidget(btnLogin);
    layout->addWidget(btnRegister);

    container->setLayout(layout);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(container, 0, Qt::AlignCenter);
    setCentralWidget(centralWidget);
}

void MainWindow::setupConnections() {
    // Connect UI buttons using the dynamically created buttons
    connect(btnLogin, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(btnRegister, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);

    // Connect only necessary client signals
    connect(client, &Client::authComplete, this, &MainWindow::handleAuthResult);
    connect(client, &Client::errorOccurred, this, &MainWindow::onErrorOccurred);
}


void MainWindow::onLoginClicked() {
    QString username = inputUsername->text();
    QString password = inputPassword->text();

    if(username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Username and password cannot be empty");
        return;
    }

    if(client->login(username, password)) {
        client->startConnection();  // Start network operations
    }
}

void MainWindow::onRegisterClicked() {
    RegistrationWindow *regWindow = new RegistrationWindow(client, this);
    regWindow->show();
}

void MainWindow::handleAuthResult(bool success, const QString& message) {
    if(success) {
        // Hide login window and show chat window
        this->hide();
        ClientWindow *chatWindow = new ClientWindow(client); // Pass client to chat window
        chatWindow->show();
    } else {
        QMessageBox::warning(this, "Authentication Failed", message);
    }
}

void MainWindow::onErrorOccurred(const QString& error) {
    QMessageBox::critical(this, "Error", error);
}

MainWindow::~MainWindow() {
    delete ui;
}
