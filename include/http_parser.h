// http_parser.h
#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include <string>
#include <map>


struct HttpRequest {
    std::string method;
    std::string url;
    std::string http_version;
    std::map<std::string, std::string> headers;
    std::string body;
};

void parse_http_request(const char* request, HttpRequest& req);

#endif // HTTP_PARSER_H