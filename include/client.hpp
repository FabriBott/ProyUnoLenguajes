#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "common.hpp"
#include "message.hpp"
#include <QObject>
#include <QString> 

// Declaración de la clase ConfigManager
class ConfigManager {
private:
    string configFile;
    int port;

public:
    ConfigManager(const string& file = "config/config.json");
    
    int getPort();
    void setPort(int newPort);
    void loadConfig();
    void saveConfig();
};

// Declaración de la clase MessageHandler
class MessageHandler {
private:
    int socket;
    string username;
    string password;
    string authType;  // "login" o "register"
    struct sockaddr_in serverAddr;
        
public:
    MessageHandler(int clientSocket, const string& user, const string& pwd, const string& type);
    bool sendAuthRequest(int port);
    bool sendMessage(const string& receiver, const string& content);
    void receiveMessages(bool& running);
};

// Declaración de la clase Client
class Client : public QObject { 
    Q_OBJECT 
public:
    Client(QObject* parent = nullptr);
    ~Client();
    
    // GUI use:
    bool login(const QString& user, const QString& pass);
    bool registerUser(const QString& user, const QString& pass);
    bool sendMessage(const QString& receiver, const QString& content);
    void startConnection(); 
    bool sendMessage(const QString &message);
    
signals:
    void messageReceived(const QString& sender, const QString& message);
    void disconnected();
    void authComplete(bool success, const QString& message);
    void errorOccurred(const QString& error);

private:
    void handleDisconnection();  // Add this declaration
    void setupSocket(int port);
    void run();

    // Member variables
    int clientSocket;
    bool running;
    ConfigManager configManager;
    MessageHandler* messageHandler;
    std::string username;
    std::string password;
    std::string operationType;
};

#endif // CLIENT_HPP