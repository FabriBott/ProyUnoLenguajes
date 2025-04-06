#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include "../../include/client.hpp"

namespace Ui {
class ClientWindow;
}

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientWindow(Client *client, QWidget *parent = nullptr);
    ~ClientWindow();

private slots:
    void onSendMessage();
    void onMessageReceived(const QString &message);
    void onDisconnected();

private:
    Ui::ClientWindow *ui;
    Client *m_client;  // Store the client pointer
    QLineEdit *messageInput;
    QPushButton *sendButton;
    QTextEdit *chatDisplay;
};

#endif // CLIENTWINDOW_H
