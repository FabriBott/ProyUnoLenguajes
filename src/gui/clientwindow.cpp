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
#include <QDialog>
#include <QScrollBar>


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

    // Circular "+" button
    addContactButton = new QPushButton("+", this);
    addContactButton->setFixedSize(40, 40);
    addContactButton->setStyleSheet(
        "QPushButton {"
        "background-color: #A7C7E7; color: white; border: none; border-radius: 20px; font-size: 20px;"
        "}"
        "QPushButton:hover { background-color: #8FBBD9; }"
        );

    // Contact layout with list and button
    QVBoxLayout *contactLayout = new QVBoxLayout();
    contactLayout->addWidget(contactList);
    contactLayout->addWidget(addContactButton, 0, Qt::AlignRight);

    QWidget *contactWidget = new QWidget(this);
    contactWidget->setLayout(contactLayout);

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
    splitter->addWidget(contactWidget);
    splitter->addWidget(chatWidget);
    splitter->setStretchFactor(1, 1);

    setCentralWidget(splitter);
    setWindowTitle("Chat Interface");

    // Signals and slots
    connect(sendButton, &QPushButton::clicked, this, &ClientWindow::onSendMessage);
    connect(messageInput, &QLineEdit::returnPressed, this, &ClientWindow::onSendMessage);
    connect(contactList, &QListWidget::itemClicked, this, &ClientWindow::onContactSelected);
    connect(addContactButton, &QPushButton::clicked, this, &ClientWindow::onAddContactClicked);
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
    QString textColor = isSender ? "#228B22" : "#1E90FF"; // green or blue

    QString html = QString(
                       "<p style='color:%1; font-family: sans-serif;'>"
                       "<b>%2</b><br>%3<br><small style='color: gray;'>%4</small>"
                       "</p>"
                       ).arg(textColor, sender, message.toHtmlEscaped(), time);

    chatDisplay->insertHtml(html);
    chatDisplay->insertHtml("<br>");
    chatDisplay->verticalScrollBar()->setValue(chatDisplay->verticalScrollBar()->maximum());
}

void ClientWindow::onContactSelected(QListWidgetItem *item)
{
    currentContact = item->text();
    chatDisplay->clear();
    chatDisplay->append(QString("<i>Chat with %1</i><br>").arg(currentContact));
}

void ClientWindow::onAddContactClicked()
{
    addContactDialog = new QDialog(this);
    addContactDialog->setWindowTitle("Add Contact");
    addContactDialog->setFixedSize(300, 150);

    QLabel *label = new QLabel("Contact name:", addContactDialog);
    contactNameInput = new QLineEdit(addContactDialog);

    QPushButton *btnCancel = new QPushButton("Cancel", addContactDialog);
    QPushButton *btnConfirm = new QPushButton("Confirm", addContactDialog);

    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(btnCancel);
    buttonLayout->addWidget(btnConfirm);

    // Main layout
    QVBoxLayout *layout = new QVBoxLayout(addContactDialog);
    layout->addWidget(label);
    layout->addWidget(contactNameInput);
    layout->addLayout(buttonLayout);

    // Connect buttons
    connect(btnCancel, &QPushButton::clicked, this, &ClientWindow::onCancelAddContact);
    connect(btnConfirm, &QPushButton::clicked, this, &ClientWindow::onConfirmAddContact);

    addContactDialog->exec();
}

void ClientWindow::onCancelAddContact()
{
    addContactDialog->close();
}

void ClientWindow::onConfirmAddContact()
{
    QString name = contactNameInput->text().trimmed();
    if (!name.isEmpty()) {
        QListWidgetItem *newItem = new QListWidgetItem(name);
        contactList->addItem(newItem);

        // Select the new contact automatically
        contactList->setCurrentItem(newItem);
        onContactSelected(newItem);

        addContactDialog->accept();
    } else {
        QMessageBox::warning(this, "Input Error", "Please enter a contact name.");
    }
}

ClientWindow::~ClientWindow() {}

