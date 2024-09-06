#include "response.h"
#include <sstream>
#include <iostream>
#include <cstring> // Pour std::memcpy

// Fonction pour obtenir le message correspondant à un code de statut HTTP
std::string get_status_message(int status_code) {
    switch (status_code) {
        case 200: return "OK";
        case 201: return "Created";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 500: return "Internal Server Error";
        default: return "Unknown Status";
    }
}

// Fonction pour créer une réponse HTTP avec un corps de taille arbitraire
void create_http_response(HttpResponse* response, int status_code, const char* content_type, const char* body, size_t body_length) {
    response->status_code = status_code;
    response->content_type = content_type;
    response->body.assign(body, body + body_length);  // Copie du corps de la réponse dans le vecteur
}

// Fonction pour formater la réponse HTTP en une chaîne de caractères
std::string format_http_response(const HttpResponse& response) {
    std::ostringstream http_response;

    // Ligne de statut
    http_response << "HTTP/1.1 " << response.status_code << " " << get_status_message(response.status_code) << "\r\n";

    // En-têtes
    http_response << "Content-Type: " << response.content_type << "\r\n";
    http_response << "Content-Length: " << response.body.size() << "\r\n";
    http_response << "\r\n";

    // Corps de la réponse
    http_response.write(response.body.data(), response.body.size());

    return http_response.str();
}
