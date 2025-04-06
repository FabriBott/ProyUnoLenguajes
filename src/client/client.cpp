#include "client.hpp"
#include <QThread>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Client* globalClient = nullptr;

Client::Client(QObject* parent) : QObject(parent),
    clientSocket(-1),
    running(true),
    configManager("config/config.json"),
    messageHandler(nullptr)
{

}

Client::~Client() {
    running = false; // This will stop the thread's loop
    if (clientSocket >= 0) {
        close(clientSocket);
    }
    if (messageHandler) {
        delete messageHandler;
    }
}

bool Client::login(const QString& user, const QString& pass) {
    username = user.toStdString();
    password = pass.toStdString();
    operationType = "login";
    return true;
}

bool Client::registerUser(const QString& user, const QString& pass) {
    username = user.toStdString();
    password = pass.toStdString();
    operationType = "register";
    return true;
}

bool Client::sendMessage(const QString &message) {
    if (!messageHandler) {
        emit errorOccurred("Not connected to server");
        return false;
    }
    bool success = messageHandler->sendMessage("server", message.toStdString());
    if (!success) {
        emit errorOccurred("Failed to send message");
    }
    return success;
}

void Client::handleDisconnection() {
    running = false;
    if (clientSocket >= 0) {
        close(clientSocket);
        clientSocket = -1;
    }
    emit disconnected();
}

void Client::setupSocket(int port) {
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = INADDR_ANY;
    clientAddr.sin_port = htons(port);

    if (bind(clientSocket, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) < 0) {
        close(clientSocket);
        clientSocket = -1;
        throw std::runtime_error("Failed to bind socket");
    }
}

void Client::startConnection() {
    try {
        int port = configManager.getPort();
        setupSocket(port);
        port = configManager.getPort(); // Get updated port if changed

        messageHandler = new MessageHandler(clientSocket, username, password, operationType);

        if (!messageHandler->sendAuthRequest(port)) {
            emit authComplete(false, "Authentication failed");
            return;
        }

        emit authComplete(true, "Authentication successful");

        // Start message processing in a thread
        QThread* thread = QThread::create([this]() {
            this->run();
        });
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        thread->start();

    } catch (const std::exception& e) {
        emit errorOccurred(QString("Error: ") + e.what());
    }
}

void Client::run() {
    if (!messageHandler) {
        emit errorOccurred("No message handler initialized");
        return;
    }

    char buffer[MAX_BUFFER_SIZE];
    struct sockaddr_in senderAddr;
    socklen_t senderAddrLen = sizeof(senderAddr);

    while (running) {  // Controlled by the destructor
        int bytesReceived = recvfrom(clientSocket, buffer, MAX_BUFFER_SIZE - 1, 0,
                                   (struct sockaddr*)&senderAddr, &senderAddrLen);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            try {
                json data = json::parse(buffer);
                if (data["type"] == "message") {
                    emit messageReceived(
                        QString::fromStdString(data["sender"]),
                        QString::fromStdString(data["content"])
                    );
                }
            } catch (...) {
                emit errorOccurred("Error processing message");
            }
        }
    }
}
