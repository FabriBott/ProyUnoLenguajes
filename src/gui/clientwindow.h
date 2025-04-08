#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();

private slots:
    void onSendMessage();
    void onContactSelected(QListWidgetItem *item);

private:
    QListWidget *contactList;
    QTextEdit *chatDisplay;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    QString currentContact;

    void appendMessage(const QString &sender, const QString &message, bool isSender);
};

#endif // CLIENTWINDOW_H
