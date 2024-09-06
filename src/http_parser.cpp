#include "http_parser.h"
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>



void parse_http_request(const char* request, HttpRequest& req) {
    std::istringstream request_stream(request);
    std::string line;

    // Lire la première ligne de la requête
    if (std::getline(request_stream, line)) {
        std::istringstream line_stream(line);
        line_stream >> req.method >> req.url >> req.http_version;
        
        std::cout << line << std::endl;

        // Vérifier la version HTTP
        if (req.http_version != "HTTP/1.1") {
            std::cerr << "Unsupported HTTP version: " << req.http_version << std::endl;
        }
    } else {
        std::cerr << "Failed to parse request line" << std::endl;
        return;
    }
    
    // Lire les en-têtes
    while (std::getline(request_stream, line) && line != "\r") {
        size_t pos = line.find(':');
        if (pos != std::string::npos) {
            std::string header_name = line.substr(0, pos);
            std::string header_value = line.substr(pos + 1);
            // Supprimer les espaces inutiles
            header_name.erase(header_name.find_last_not_of(" \t") + 1);
            header_value.erase(0, header_value.find_first_not_of(" \t"));
            std::cout << "Header: " << header_name << "; Value: " << header_value << std::endl;
            req.headers[header_name] = header_value;
        }
    }

    // Lire le corps de la requête si présent
    if (req.headers.find("Content-Length") != req.headers.end()) {
        size_t content_length = std::stoi(req.headers["Content-Length"]);
        if (content_length > 0) {
            std::vector<char> body(content_length);
            request_stream.read(body.data(), content_length);
            req.body.assign(body.begin(), body.end());
        }
    }
}
