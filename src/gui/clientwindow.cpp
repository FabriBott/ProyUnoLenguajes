#include "clientwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QSplitter>
#include <QMessageBox>
#include <QLabel>
#include <QDateTime>

ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Create contact list
    contactList = new QListWidget(this);
    contactList->setFixedWidth(200);
    contactList->setStyleSheet(
        "QListWidget { background: #407280; border: none; } "
        "QListWidget::item { padding-left: 20px; padding-top: 10px; padding-bottom: 10px; color: white; }"
        );

    // Dummy contacts
    QStringList contacts = { "Alonso", "Caleb", "Bot" };
    for (const QString &name : contacts) {
        QListWidgetItem *item = new QListWidgetItem(name);
        contactList->addItem(item);
    }

    // Chat display area
    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);
    chatDisplay->setStyleSheet("QTextEdit { background: white; border: none; padding: 10px; }");

    // Message input and send button
    messageInput = new QLineEdit(this);
    messageInput->setPlaceholderText("Type your message...");
    sendButton = new QPushButton("Send", this);
    sendButton->setFixedWidth(80);

    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);

    QVBoxLayout *chatLayout = new QVBoxLayout;
    chatLayout->addWidget(chatDisplay);
    chatLayout->addLayout(inputLayout);

    QWidget *chatWidget = new QWidget(this);
    chatWidget->setLayout(chatLayout);

    // Main layout: Split contact list and chat area
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(contactList);
    splitter->addWidget(chatWidget);
    splitter->setStretchFactor(1, 1);

    setCentralWidget(splitter);
    setWindowTitle("Chat Interface");

    // Signals and slots
    connect(sendButton, &QPushButton::clicked, this, &ClientWindow::onSendMessage);
    connect(contactList, &QListWidget::itemClicked, this, &ClientWindow::onContactSelected);
}

void ClientWindow::onSendMessage()
{
    QString message = messageInput->text();
    if (!message.isEmpty()) {
        appendMessage("You", message, true);
        appendMessage(currentContact, "Reply from " + currentContact, false); // simulate response
        messageInput->clear();
    }
}

void ClientWindow::appendMessage(const QString &sender, const QString &message, bool isSender)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm");
    QString bubbleColor = isSender ? "#dcf8c6" : "#f1f0f0"; // greenish vs gray
    QString align = isSender ? "right" : "left";

    QString html = QString(
                       "<div align='%1' style='margin:5px;'>"
                       "<div style='background-color:%2; border-radius:10px; padding:8px; max-width:60%%; display:inline-block;'>"
                       "<b>%3</b><br>%4<br><small>%5</small>"
                       "</div></div>"
                       ).arg(align, bubbleColor, sender, message, time);

    chatDisplay->append(html);
}

void ClientWindow::onContactSelected(QListWidgetItem *item)
{
    currentContact = item->text();
    chatDisplay->clear();
    chatDisplay->append(QString("<i>Chat with %1</i><br>").arg(currentContact));
}

ClientWindow::~ClientWindow() {}

