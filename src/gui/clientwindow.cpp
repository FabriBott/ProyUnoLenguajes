#include "clientwindow.h"
#include "ui_clientwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>

ClientWindow::ClientWindow(Client *client, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientWindow),
    m_client(client)  // Initialize the client
{
    ui->setupUi(this);

    // Create widgets
    messageInput = new QLineEdit(this);
    sendButton = new QPushButton("Send", this);
    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);

    // Setup layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(chatDisplay);
    layout->addWidget(messageInput);
    layout->addWidget(sendButton);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Connect signals from the Client
    connect(m_client, &Client::messageReceived, this, &ClientWindow::onMessageReceived);
    connect(m_client, &Client::disconnected, this, &ClientWindow::onDisconnected);
    connect(sendButton, &QPushButton::clicked, this, &ClientWindow::onSendMessage);
}

void ClientWindow::onSendMessage()
{
    QString message = messageInput->text();
    if (!message.isEmpty()) {
        m_client->sendMessage(message);  // Use client to send message
        chatDisplay->append("You: " + message);
        messageInput->clear();
    }
}

void ClientWindow::onMessageReceived(const QString &message)
{
    chatDisplay->append("Server: " + message);
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


