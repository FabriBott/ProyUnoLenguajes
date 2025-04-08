#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QDialog>

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();

private slots:
    void onSendMessage();
    void onContactSelected(QListWidgetItem *item);
    void onAddContactClicked();
    void onCancelAddContact();
    void onConfirmAddContact();

private:
    // UI components
    QListWidget *contactList;
    QPushButton *addContactButton;
    QTextEdit *chatDisplay;
    QLineEdit *messageInput;
    QPushButton *sendButton;

    // Dialog for adding contact
    QDialog *addContactDialog;
    QLineEdit *contactNameInput;

    // State
    QString currentContact;

    // Helpers
    void appendMessage(const QString &sender, const QString &message, bool isSender);
};

#endif // CLIENTWINDOW_H

