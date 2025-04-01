#include "server.hpp"

///////////////////////////////////
// Implementación de UserManager //
///////////////////////////////////

// Esta clase maneja una lista de usuarios y la carga/guarda en un archivo JSON.
UserManager::UserManager(const string& file) : usersFile(file) {
    loadUsers();
}

// Lee el file users
void UserManager::loadUsers() {
    ifstream file(usersFile);
    if (file.is_open()) {
        try {
            json j;
            file >> j;
            file.close();
            
            users.clear();
            for (const auto& item : j) {
                users.push_back(User::fromJson(item));
            }
            
            cout << "Usuarios cargados: " << users.size() << endl;
        } catch (json::exception& e) {
            cerr << "Error al cargar usuarios: " << e.what() << endl;
        }
    } else {
        cout << "No se encontró archivo de usuarios. Se creará uno nuevo." << endl;
        
        // Crear directorio si no existe
        system("mkdir -p data");
    }
}

// Guarda en el file users
void UserManager::saveUsers() {
    json j = json::array();
    
    for (const auto& user : users) {
        j.push_back(user.toJson());
    }
    
    ofstream file(usersFile);
    file << j.dump(4); // Formato con indentación
    file.close();
    
    cout << "Usuarios guardados: " << users.size() << endl;
}

// Busca un usuario por nombre y retorna un puntero a ese usuario si lo encuentra.
User* UserManager::findUser(const string& username) {
    for (auto& user : users) {
        if (user.username == username) {
            return &user;
        }
    }
    return nullptr;
}

// Si el usuario existe, actualiza su IP y puerto.
// Si no existe, lo agrega a la lista y guarda los cambios.
bool UserManager::registerUser(const string& username, const string& password, const string& ip, int port) {
    User* existingUser = findUser(username);

    if (existingUser) {
        // User exists, check password and update connection info
        if (!password.empty() && existingUser->password != password) {
            cout << "Autenticación fallida para usuario: " << username << endl;
            return false;
        }
        
        existingUser->ip = ip;
        existingUser->port = port;
        
        // If existing user has no password yet, set it
        if (existingUser->password.empty() && !password.empty()) {
            existingUser->password = password;
        }
        
        cout << "Usuario actualizado: " << username << " en " << ip << ":" << port << endl;
    } else {
        // Create new user
        User newUser(username, password, ip, port);
        users.push_back(newUser);
        cout << "Nuevo usuario registrado: " << username << " en " << ip << ":" << port << endl;
    }

    saveUsers();
    return true;
}

bool UserManager::authenticateUser(const string& username, const string& password) {
    User* user = findUser(username);
    if (!user) {
        return false; // User not found
    }
    
    // For existing users that might not have a password yet
    if (user->password.empty()) {
        user->password = password;
        saveUsers();
        return true;
    }
    
    return user->password == password;
}

vector<User>& UserManager::getAllUsers() {
    return users;
}




//////////////////////////////////////
// Implementación de MessageManager //
//////////////////////////////////////

// Maneja una lista de mensajes y los guarda en data/messages.json
MessageManager::MessageManager(const string& file) : messagesFile(file) {
    loadMessages();
}

// Carga los mensajes desde messages en un vector
void MessageManager::loadMessages() {
    ifstream file(messagesFile);
    if (file.is_open()) {
        try {
            json j;
            file >> j;
            file.close();
            
            messages.clear();
            for (const auto& item : j) {
                Message msg = Message::fromJson(item);
                msg.type = "message"; // Asegurar consistencia
                messages.push_back(msg);
            }
            
            cout << "Mensajes cargados: " << messages.size() << endl;
        } catch (json::exception& e) {
            cerr << "Error al cargar mensajes: " << e.what() << endl;
        }
    } else {
        cout << "No se encontró archivo de mensajes. Se creará uno nuevo." << endl;
        
        // Crear directorio si no existe
        system("mkdir -p data");
    }
}

// Guarda todos los mensajes 
void MessageManager::saveMessages() {
    json j = json::array();
    int id = 1;
    
    for (const auto& msg : messages) {
        if (msg.type == "message") {
            json msgJson = msg.toJson();
            msgJson["id"] = id++;
            j.push_back(msgJson);
        }
    }
    
    ofstream file(messagesFile);
    file << j.dump(4); // Formato con indentación
    file.close();
    
    cout << "Mensajes guardados: " << messages.size() << endl;
}

// Agrega un mensaje a la lista y guarda los cambios.
bool MessageManager::addMessage(const Message& msg) {
    messages.push_back(msg);
    saveMessages();
    return true;
}

vector<Message>& MessageManager::getAllMessages() {
    return messages;
}




/////////////////////////////////
// Implementación del servidor //
/////////////////////////////////

// Inicializa los objetos UserManager y MessageManager
Server::Server() : serverSocket(-1), 
                  userManager("data/users.json"), 
                  messageManager("data/messages.json") {
}

// Cierra el socket al destruirse el servidor.
Server::~Server() {
    if (serverSocket >= 0) {
        close(serverSocket);
    }
}

// Crea un socket UDP y lo enlaza al puerto del servidor.
bool Server::init() {
    // Crear socket UDP
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        cerr << "Error al crear socket" << endl;
        return false;
    }
    
    // Configurar dirección del servidor
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);
    
    // Enlazar socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Error al enlazar socket" << endl;
        close(serverSocket);
        return false;
    }
    
    cout << "Servidor iniciado en puerto " << SERVER_PORT << endl;
    return true;
}

void Server::processLogin(const json& data, struct sockaddr_in clientAddr) {
    string username = data["username"];
    string password = data["password"];
    int port = data["port"];
    
    // Convertir IP a string
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
    
    bool success = false;
    string responseMessage;
    
    // Buscar usuario
    User* user = userManager.findUser(username);
    
    if (user) {
        // Verificar contraseña
        if (user->password == password) {
            // Actualizar información de conexión
            user->ip = string(clientIP);
            user->port = port;
            userManager.saveUsers();
            success = true;
            responseMessage = "Inicio de sesión exitoso";
        } else {
            responseMessage = "Contraseña incorrecta";
        }
    } else {
        responseMessage = "Usuario no encontrado";
    }
    
    // Crear respuesta
    json responseJson;
    responseJson["type"] = "auth_response";
    responseJson["status"] = success ? "success" : "error";
    responseJson["content"] = responseMessage;
    responseJson["timestamp"] = time(nullptr);
    
    string responseStr = responseJson.dump();
    
    // Enviar respuesta
    sendto(serverSocket, responseStr.c_str(), responseStr.length(), 0, 
           (struct sockaddr*)&clientAddr, sizeof(clientAddr));
}

// Registra un nuevo usuario usando su dirección IP y puerto.
void Server::processRegistration(const json& data, struct sockaddr_in clientAddr) {
    string username = data["username"];
    string password = data["password"];
    int port = data["port"];
    
    // Convertir IP a string
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
    
    bool success = false;
    string responseMessage;
    
    // Verificar si el usuario ya existe
    User* existingUser = userManager.findUser(username);
    
    if (existingUser) {
        responseMessage = "El nombre de usuario ya está en uso";
    } else {
        // Crear nuevo usuario
        User newUser(username, password, string(clientIP), port);
        userManager.getAllUsers().push_back(newUser);
        userManager.saveUsers();
        success = true;
        responseMessage = "Registro exitoso";
    }
    
    // Crear respuesta
    json responseJson;
    responseJson["type"] = "auth_response";
    responseJson["status"] = success ? "success" : "error";
    responseJson["content"] = responseMessage;
    responseJson["timestamp"] = time(nullptr);
    
    string responseStr = responseJson.dump();
    
    // Enviar respuesta
    sendto(serverSocket, responseStr.c_str(), responseStr.length(), 0, 
           (struct sockaddr*)&clientAddr, sizeof(clientAddr));
}

void Server::processMessage(const json& data) {
    string sender = data["sender"];
    string receiver = data["receiver"];
    string content = data["content"];
    
    // Buscar usuario receptor
    User* receiverUser = userManager.findUser(receiver);
    User* senderUser = userManager.findUser(sender);

    if (!receiverUser) {
        cout << "Usuario destinatario no encontrado: " << receiver << endl;
        return;
    }

    bool senderUpdated = senderUser->addContact(receiver);
    bool receiverUpdated = receiverUser->addContact(sender);

    if (senderUpdated || receiverUpdated) {
        userManager.saveUsers();  // Persiste los cambios
    }
    
    if (receiverUser->ip.empty() || receiverUser->port <= 0) {
        cout << "Dirección o puerto inválido para usuario: " << receiver << endl;
        return;
    }

    // Crear mensaje
    Message msg;
    msg.type = "message";
    msg.sender = sender;
    msg.receiver = receiver;
    msg.content = content;
    msg.timestamp = time(nullptr);
    
    // Guardar mensaje
    messageManager.addMessage(msg);
    
    // Preparar mensaje para enviar al destinatario
    json msgJson = msg.toJson();
    string msgStr = msgJson.dump();
    
    // Ensure the IP and port for the receiver are valid
    // if (receiverUser->ip.empty() || receiverUser->port <= 0) {
    //     cout << "Dirección o puerto inválido para usuario: " << receiver << endl;
    //     return;
    // }
    
    // Configurar dirección del destinatario
    struct sockaddr_in receiverAddr;
    memset(&receiverAddr, 0, sizeof(receiverAddr));
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(receiverUser->port);
    
    // Check if the IP address can be converted properly
    if (inet_pton(AF_INET, receiverUser->ip.c_str(), &(receiverAddr.sin_addr)) <= 0) {
        cout << "IP inválida para usuario: " << receiver << ": " << receiverUser->ip << endl;
        return;
    }
    
    // Debug - print destination address
    // char ipStr[INET_ADDRSTRLEN];
    // inet_ntop(AF_INET, &(receiverAddr.sin_addr), ipStr, INET_ADDRSTRLEN);
    // cout << "Enviando mensaje a " << receiver << " en " << ipStr << ":" << receiverUser->port << endl;
    
    // Enviar mensaje al destinatario
    int bytesSent = sendto(serverSocket, msgStr.c_str(), msgStr.length(), 0, 
                          (struct sockaddr*)&receiverAddr, sizeof(receiverAddr));
    
    if (bytesSent > 0) {
        cout << "Mensaje enviado de " << sender << " a " << receiver 
                << " en " << receiverUser->ip << ":" << receiverUser->port 
                << " (" << bytesSent << " bytes)" << endl;
    } else {
        cerr << "Error al enviar mensaje a " << receiver << ": " << strerror(errno) << endl;
    }
}

// Recibe datos y procesa registros o mensajes.
void Server::run() {
    char buffer[MAX_BUFFER_SIZE];
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    
    cout << "Esperando mensajes..." << endl;
    
    while (true) {
        // Recibir datos
        int bytesReceived = recvfrom(serverSocket, buffer, MAX_BUFFER_SIZE - 1, 0, 
                                   (struct sockaddr*)&clientAddr, &clientAddrLen);
        
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            
            try {
                // Parsear JSON
                json data = json::parse(buffer);
                string msgType = data["type"];
                
                // Procesar según tipo de mensaje
                if (msgType == "login") {
                    processLogin(data, clientAddr);
                } else if (msgType == "register") {
                    processRegistration(data, clientAddr);
                } else if (msgType == "message") {
                    processMessage(data);
                }
                // You can keep the old "register" handling for backward compatibility
                // or remove it if you're updating all clients
            } catch (json::exception& e) {
                cerr << "Error al procesar mensaje JSON: " << e.what() << endl;
            }
        }
    }
}