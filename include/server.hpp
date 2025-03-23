#ifndef SERVER_HPP
#define SERVER_HPP

#include "common.hpp"
#include "user.hpp"
#include "message.hpp"

// Declaración de la clase UserManager
class UserManager {
    private:
        vector<User> users;
        string usersFile;

    public:
        UserManager(const string& file = "data/users.json");
        
        // Métodos para manejo de usuarios
        void loadUsers();
        void saveUsers();
        User* findUser(const string& username);
        bool registerUser(const string& username, const string& ip, int port);
        vector<User>& getAllUsers();
};

// Declaración de la clase MessageManager
class MessageManager {
    private:
        vector<Message> messages;
        string messagesFile;

    public:
        MessageManager(const string& file = "data/messages.json");
        
        // Métodos para manejo de mensajes
        void loadMessages();
        void saveMessages();
        bool addMessage(const Message& msg);
        vector<Message>& getAllMessages();
};

// Declaración de la clase Server
class Server {
    private:
        int serverSocket;
        UserManager userManager;
        MessageManager messageManager;
        
        // Métodos privados para procesar mensajes
        void processRegistration(const json& data, struct sockaddr_in clientAddr);
        void processMessage(const json& data);

    public:
        Server();
        ~Server();
        
        bool init();
        void run();
};

#endif // SERVER_HPP