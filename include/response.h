#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <vector>

// Représente une réponse HTTP
struct HttpResponse {
    int status_code;
    std::string content_type;
    std::vector<char> body;  // Utilisation d'un vecteur de caractères pour le corps
};

// Fonction pour créer une réponse HTTP simple avec un corps de taille arbitraire
void create_http_response(HttpResponse* response, int status_code, const char* content_type, const char* body, size_t body_length);

// Fonction pour formater la réponse HTTP en une chaîne de caractères à envoyer
std::string format_http_response(const HttpResponse& response);

#endif // RESPONSE_H
