#include "clientwindow.h"
#include "ui_clientwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include "../../include/common.hpp"

ClientWindow::ClientWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientWindow)
{
    ui->setupUi(this);

    // Crear widgets manualmente
    messageInput = new QLineEdit(this);
    sendButton = new QPushButton("Enviar", this);
    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);

    // Layout para organizar los widgets
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(chatDisplay);
    layout->addWidget(messageInput);
    layout->addWidget(sendButton);

    // Crear un widget central y asignarle el layout
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Crear y configurar el socket
    socket = new QTcpSocket(this);
    socket->connectToHost(SERVER_IP, SERVER_PORT);

    // Conectar señales y slots
    connect(socket, &QTcpSocket::readyRead, this, &ClientWindow::onMessageReceived);
    connect(socket, &QTcpSocket::disconnected, this, &ClientWindow::onDisconnected);
    connect(sendButton, &QPushButton::clicked, this, &ClientWindow::onSendMessage);
}

void ClientWindow::onSendMessage()
{
    QString message = messageInput->text();
    if (!message.isEmpty()) {
        socket->write(message.toUtf8());
        messageInput->clear();
    }
}

void ClientWindow::onMessageReceived()
{
    QByteArray data = socket->readAll();
    chatDisplay->append("Server: " + QString(data));
}

void ClientWindow::onDisconnected()
{
    QMessageBox::warning(this, "Disconnected", "Connection to the server lost.");
    close();
}

ClientWindow::~ClientWindow()
{
    delete ui;
}


