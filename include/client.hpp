#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "common.hpp"
#include "message.hpp"

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
        struct sockaddr_in serverAddr;

    public:
        MessageHandler(int clientSocket, const string& user);
        
        bool sendRegistration(int port);
        bool sendMessage(const string& receiver, const string& content);
        void receiveMessages(bool& running);
};

// Declaración de la clase Client
class Client {
private:
    int clientSocket;
    string username;
    bool running;
    ConfigManager configManager;
    MessageHandler* messageHandler;
    
    void setupSocket(int port);

public:
    Client();
    ~Client();
    
    bool init();
    void run();
    void handleUserInput();
    
    static void handleSignal(int signal);
};

#endif // CLIENT_HPP