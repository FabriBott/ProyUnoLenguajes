#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

namespace Ui {
class ClientWindow;
}

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();

private slots:
    void onSendMessage();
    void onMessageReceived();
    void onDisconnected();

private:
    Ui::ClientWindow *ui;
    QTcpSocket *socket;

    // Widgets
    QLineEdit *messageInput;
    QPushButton *sendButton;
    QTextEdit *chatDisplay;
};

#endif // CLIENTWINDOW_H
