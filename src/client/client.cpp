#include "client.hpp"
#include <signal.h>

// Variable global para manejar la señal
Client* globalClient = nullptr;




/////////////////////////////////////
// Implementación de ConfigManager //
/////////////////////////////////////

// Inicializa la configuración con un JSON
ConfigManager::ConfigManager(const string& file) : configFile(file), port(DEFAULT_CLIENT_PORT) {
    loadConfig();
}

int ConfigManager::getPort() {
    return port;
}

void ConfigManager::setPort(int newPort) {
    port = newPort;
    saveConfig();
}

// Intenta leer la configuración
void ConfigManager::loadConfig() {
    // Crear directorio si no existe
    system("mkdir -p config");
    
    ifstream configFile(this->configFile);
    if (!configFile.is_open()) {
        cout << "Archivo de configuración no encontrado. Usando puerto predeterminado " << port << endl;
        saveConfig(); // Crear archivo con configuración predeterminada
        return;
    }

    try {
        json config;
        configFile >> config;
        configFile.close();
        
        if (config.contains("port")) {
            port = config["port"];
        }
    } catch (json::exception& e) {
        cerr << "Error al leer configuración: " << e.what() << endl;
    }
}

// Guarda el puerto en un JSON
void ConfigManager::saveConfig() {
    json config;
    config["port"] = port;
    
    ofstream configFile(this->configFile);
    configFile << config.dump(4);
    configFile.close();
}




//////////////////////////////////////
// Implementación de MessageHandler //
//////////////////////////////////////

MessageHandler::MessageHandler(int clientSocket, const string& user) 
    : socket(clientSocket), username(user) {
    
    // Configurar dirección del servidor
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &(serverAddr.sin_addr));
}

// Envía un mensaje de registro con el usuario y puerto
// Espera una respuesta del servidor y la procesa
bool MessageHandler::sendRegistration(int port) {
    // Preparar mensaje de registro
    json registerMsg;
    registerMsg["type"] = "register";
    registerMsg["username"] = username;
    registerMsg["port"] = port;
    
    string msgStr = registerMsg.dump();
    
    // Enviar mensaje de registro
    sendto(socket, msgStr.c_str(), msgStr.length(), 0, 
           (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    
    // Esperar respuesta del servidor
    char buffer[MAX_BUFFER_SIZE];
    struct sockaddr_in responseAddr;
    socklen_t responseAddrLen = sizeof(responseAddr);
    
    int bytesReceived = recvfrom(socket, buffer, MAX_BUFFER_SIZE - 1, 0, 
                               (struct sockaddr*)&responseAddr, &responseAddrLen);
    
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        
        try {
            json response = json::parse(buffer);
            
            // Debugging - print received JSON
            cout << "Respuesta del servidor: " << response.dump() << endl;
            
            // Check if it's a register response - don't check status since it's missing
            if (response["type"] == "register_response") {
                string message = response.value("content", "Registro completado");
                cout << COLOR_GREEN << "Registro exitoso: " << message << COLOR_RESET << endl;
                return true;
            } else {
                string errorMsg = response.value("content", "Error desconocido");
                cerr << COLOR_RED << "Error en registro: " << errorMsg << COLOR_RESET << endl;
            }
        } catch (json::exception& e) {
            cerr << COLOR_RED << "Error al procesar respuesta: " << e.what() << COLOR_RESET << endl;
        }
    } else {
        cerr << COLOR_RED << "No se recibió respuesta del servidor" << COLOR_RESET << endl;
    }
    
    return false;
}

// Envía un mensaje a otro usuario a través del servidor
bool MessageHandler::sendMessage(const string& receiver, const string& content) {
    // Preparar mensaje
    json messageData;
    messageData["type"] = "message";
    messageData["sender"] = username;
    messageData["receiver"] = receiver;
    messageData["content"] = content;
    
    string msgStr = messageData.dump();
    
    // Enviar mensaje al servidor
    sendto(socket, msgStr.c_str(), msgStr.length(), 0, 
           (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    
    cout << COLOR_GREEN << "Tú: " << content << COLOR_RESET << endl;
    return true;
}

// Recibe mensajes entrantes y los muestra en pantalla
void MessageHandler::receiveMessages(bool& running) {
    char buffer[MAX_BUFFER_SIZE];
    struct sockaddr_in senderAddr;
    socklen_t senderAddrLen = sizeof(senderAddr);
    
    while (running) {
        int bytesReceived = recvfrom(socket, buffer, MAX_BUFFER_SIZE - 1, 0, 
                                   (struct sockaddr*)&senderAddr, &senderAddrLen);
        
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            
            try {
                json data = json::parse(buffer);
                
                // Debug - print the entire received message
                // cout << "Message received: " << data.dump() << endl;
                
                if (data["type"] == "message") {
                    string sender = data["sender"];
                    string receiver = data["receiver"];
                    string content = data["content"];
                    
                    // Only process messages that are intended for this user
                    // and not sent by this user
                    if (receiver == username && sender != username) {
                        // Imprimir mensaje recibido en otro color
                        cout << COLOR_BLUE << sender << ": " << content << COLOR_RESET << endl;
                    }
                }
            } catch (json::exception& e) {
                cerr << COLOR_RED << "Error al procesar mensaje: " << e.what() << COLOR_RESET << endl;
            }
        }
    }
}




//////////////////////////////
// Implementación de Client //
//////////////////////////////

// Maneja señales del sistema para cerrar correctamente el cliente
void Client::handleSignal(int signal) {
    if (globalClient) {
        globalClient->running = false;
        cout << COLOR_YELLOW << "\nCerrando aplicación..." << COLOR_RESET << endl;
    }
    exit(0);
}

Client::Client() : clientSocket(-1), running(true), configManager("config/config.json"), messageHandler(nullptr) {
    globalClient = this; // Para manejo de señales
}

Client::~Client() {
    if (clientSocket >= 0) {
        close(clientSocket);
    }
    if (messageHandler) {
        delete messageHandler;
    }
}

// Crea y enlaza un socket UDP
void Client::setupSocket(int port) {
    // Crear socket UDP
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        cerr << COLOR_RED << "Error al crear socket" << COLOR_RESET << endl;
        exit(1);
    }
    
    // Configurar dirección del cliente
    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = INADDR_ANY;
    clientAddr.sin_port = htons(port);
    
    // Enlazar socket
    if (bind(clientSocket, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) < 0) {
        cerr << COLOR_RED << "Error al enlazar socket. Puerto " << port << " posiblemente en uso." << COLOR_RESET << endl;
        cout << "¿Desea probar con otro puerto? (s/n): ";
        char response;
        cin >> response;
        
        if (response == 's' || response == 'S') {
            cout << "Ingrese nuevo puerto: ";
            int newPort;
            cin >> newPort;
            configManager.setPort(newPort);
            
            // Intentar nuevamente con el nuevo puerto
            clientAddr.sin_port = htons(newPort);
            if (bind(clientSocket, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) < 0) {
                cerr << COLOR_RED << "Error al enlazar socket con el nuevo puerto." << COLOR_RESET << endl;
                exit(1);
            }
            // Actualizar el puerto para que se use en los mensajes posteriores
            port = newPort;
        } else {
            exit(1);
        }
    }
}

bool Client::init() {
    // Configurar manejo de señales
    signal(SIGINT, handleSignal);
    
    // Solicitar nombre de usuario
    cout << "Ingrese su nombre de usuario: ";
    cin >> username;
    
    // Configurar socket
    int port = configManager.getPort();
    setupSocket(port);

    // Obtener el puerto actualizado (en caso de que haya cambiado en setupSocket)
    port = configManager.getPort();
    
    cout << "Cliente iniciado en puerto " << port << endl;
    
    // Crear manejador de mensajes
    messageHandler = new MessageHandler(clientSocket, username);
    
    // Registrar usuario en el servidor
    if (!messageHandler->sendRegistration(port)) {
        cerr << COLOR_RED << "No se pudo registrar con el servidor" << COLOR_RESET << endl;
        return false;
    }
    
    return true;
}

void Client::handleUserInput() {
    cout << COLOR_YELLOW << "Chat iniciado. Ingrese 'salir' para terminar." << COLOR_RESET << endl;
    cout << COLOR_YELLOW << "Formato para enviar mensaje: destinatario:mensaje" << COLOR_RESET << endl;
    
    string input;
    cin.ignore(); // Limpiar buffer
    
    while (running) {
        getline(cin, input);
        
        if (input == "salir") {
            running = false;
            break;
        }
        
        // Encontrar delimitador entre destinatario y mensaje
        size_t pos = input.find(":");
        if (pos != string::npos) {
            string receiver = input.substr(0, pos);
            string content = input.substr(pos + 1);
            
            if (!receiver.empty() && !content.empty()) {
                messageHandler->sendMessage(receiver, content);
            } else {
                cout << COLOR_YELLOW << "Formato inválido. Use 'destinatario:mensaje'" << COLOR_RESET << endl;
            }
        } else {
            cout << COLOR_YELLOW << "Formato inválido. Use 'destinatario:mensaje'" << COLOR_RESET << endl;
        }
    }
}

void Client::run() {
    // Bifurcar el proceso
    pid_t pid = fork();
    
    if (pid < 0) {
        cerr << COLOR_RED << "Error al crear proceso hijo" << COLOR_RESET << endl;
        return;
    } else if (pid == 0) {
        // Proceso hijo: recibir mensajes
        messageHandler->receiveMessages(running);
        exit(0);
    } else {
        // Proceso padre: enviar mensajes
        handleUserInput();
        
        // Enviar señal al proceso hijo para terminar
        kill(pid, SIGINT);
    }
}