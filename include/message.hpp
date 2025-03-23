#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include "common.hpp"
#include <ctime>

// Estructura para mensajes
class Message {
public:
    string type; // "register", "message", etc.
    string sender; // Nombre del remitente
    string receiver; // Nombre del destinatario
    string content; // Contenido del mensaje
    time_t timestamp; // Marca de tiempo
    bool delivered; // Estado de entrega
    string status; // Campo adicional para status

    Message() : timestamp(time(nullptr)), delivered(false) {}

    // Método para convertir a JSON
    json toJson() const {
        json j;
        j["type"] = type;
        j["sender"] = sender;
        j["receiver"] = receiver;
        j["content"] = content;
        j["timestamp"] = timestamp;
        j["delivered"] = delivered;
        
        // Añadir status solo si no está vacío
        if (!status.empty()) {
            j["status"] = status;
        }
        
        return j;
    }

    // Método para cargar desde JSON
    static Message fromJson(const json& j) {
        Message msg;
        if (j.contains("type")) msg.type = j["type"];
        if (j.contains("sender")) msg.sender = j["sender"];
        if (j.contains("receiver")) msg.receiver = j["receiver"];
        if (j.contains("content")) msg.content = j["content"];
        if (j.contains("timestamp")) msg.timestamp = j["timestamp"];
        if (j.contains("delivered")) msg.delivered = j["delivered"];
        if (j.contains("status")) msg.status = j["status"];
        return msg;
    }

    // Método para crear un mensaje de respuesta
    static Message createResponse(const string& type, const string& status, const string& content) {
        Message msg;
        msg.type = type;
        msg.content = content;
        msg.status = status; // Asignar directamente al campo status
        msg.timestamp = time(nullptr);
        return msg;
    }
};
#endif // MESSAGE_HPP