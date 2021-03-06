#ifndef HTTPTools_Info_H
#define HTTPTools_Info_H

#include <stdio.h>
#include <string>
#include <unordered_map>

const std::string LINE_ENDING = "\r\n";
const std::string HTTP_VERSION = "HTTP/1.1";
const std::string NOT_FOUND_ERROR_MESSAGE = "<html><head><title>Not Found :(</title></head><body>The given document couldn't be found</body></html>";
const std::string INTERNAL_SERVER_ERROR_MESSAGE = "<html><head><title>Internal Server Error :(</title></head><body>Send help</body></html>";

enum class Method {
    OPTIONS,
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    TRACE,
    CONNECT
};

const std::unordered_map<std::string, Method> method_map = {
        {"OPTIONS", Method::OPTIONS },
        {"GET",     Method::GET     },
        {"HEAD",    Method::HEAD    },
        {"POST",    Method::POST    },
        {"PUT",     Method::PUT     },
        {"DELETE",  Method::DELETE  },
        {"TRACE",   Method::TRACE   },
        {"CONNECT", Method::CONNECT }
};

/* The Status Code section has been copied from: https://developer.mozilla.org/en-US/docs/Web/HTTP/Status*/

enum StatusCode {
    CONTINUE                          = 100,
    SWITCHING_PROTOCOLS               = 101,
    OK                                = 200,
    CREATED                           = 201,
    ACCEPTED                          = 202,
    NON_AUTHORITATIVE_INFORMATION     = 203,
    NO_CONTENT                        = 204,
    RESET_CONTENT                     = 205,
    PARTIAL_CONTENT                   = 206,
    MULTIPLE_CHOICES                  = 300,
    MOVED_PERMANENTLY                 = 301,
    FOUND                             = 302,
    SEE_OTHER                         = 303,
    NOT_MODIFIED                      = 304,
    USE_PROXY                         = 305,
    TEMPORARY_REDIRECT                = 307,
    BAD_REQUEST                       = 400,
    UNAUTHORIZED                      = 401,
    PAYMENT_REQUIRED                  = 402,
    FORBIDDEN                         = 403,
    NOT_FOUND                         = 404,
    METHOD_NOT_ALLOWED                = 405,
    NOT_ACCEPTABLE                    = 406,
    PROXY_AUTHENTICATION_REQUIRED     = 407,
    REQUEST_TIME_OUT                  = 408,
    CONFLICT                          = 409,
    GONE                              = 410,
    LENGTH_REQUIRED                   = 411,
    PRECONDITION_FAILED               = 412,
    REQUEST_ENTITY_TOO_LARGE          = 413,
    REQUEST_URI_TOO_LARGE             = 414,
    UNSUPPORTED_MEDIA_TYPE            = 415,
    REQUESTED_RANGE_NOT_SATISFIABLE   = 416,
    EXPECTATION_FAILED                = 417,
    INTERNAL_SERVER_ERROR             = 500,
    NOT_IMPLEMENTED                   = 501,
    BAD_GATEWAY                       = 502,
    SERVICE_UNAVAILABLE               = 503,
    GATEWAY_TIME_OUT                  = 504,
    HTTP_VERSION_NOT_SUPPORTED        = 505
};

const std::unordered_map<int, std::string> statuscode_map = {
        {CONTINUE,                          "Continue"},
        {SWITCHING_PROTOCOLS,               "Switching Protocols"},
        {OK,                                "OK"},
        {CREATED,                           "Created"},
        {ACCEPTED,                          "Accepted"},
        {NON_AUTHORITATIVE_INFORMATION,     "Non-Authoritative Information"},
        {NO_CONTENT,                        "No Content"},
        {RESET_CONTENT,                     "Reset Content"},
        {PARTIAL_CONTENT,                   "Partial Content"},
        {MULTIPLE_CHOICES,                  "Multiple Choices"},
        {MOVED_PERMANENTLY,                 "Moved Permanently"},
        {FOUND,                             "Found"},
        {SEE_OTHER,                         "See Other"},
        {NOT_MODIFIED,                      "Not Modified"},
        {USE_PROXY,                         "Use Proxy"},
        {TEMPORARY_REDIRECT,                "Temporary Redirect"},
        {BAD_REQUEST,                       "Bad Request"},
        {UNAUTHORIZED,                      "Unauthorized"},
        {PAYMENT_REQUIRED,                  "Payment Required"},
        {FORBIDDEN,                         "Forbidden"},
        {NOT_FOUND,                         "Not Found"},
        {METHOD_NOT_ALLOWED,                "Method Not Allowed"},
        {NOT_ACCEPTABLE,                    "Not Acceptable"},
        {PROXY_AUTHENTICATION_REQUIRED,     "Proxy Authentication Required"},
        {REQUEST_TIME_OUT,                  "Request Time-out"},
        {CONFLICT,                          "Conflict"},
        {GONE,                              "Gone"},
        {LENGTH_REQUIRED,                   "Length Required"},
        {PRECONDITION_FAILED,               "Precondition Failed"},
        {REQUEST_ENTITY_TOO_LARGE,          "Request Entity Too Large"},
        {REQUEST_URI_TOO_LARGE,             "Request-URI Too Large"},
        {UNSUPPORTED_MEDIA_TYPE,            "Unsupported Media Type"},
        {REQUESTED_RANGE_NOT_SATISFIABLE,   "Requested range not satisfiable"},
        {EXPECTATION_FAILED,                "Expectation Failed"},
        {INTERNAL_SERVER_ERROR,             "Internal Server Error"},
        {NOT_IMPLEMENTED,                   "Not Implemented"},
        {BAD_GATEWAY,                       "Bad Gateway"},
        {SERVICE_UNAVAILABLE,               "Service Unavailable"},
        {GATEWAY_TIME_OUT,                  "Gateway Time-out"},
        {HTTP_VERSION_NOT_SUPPORTED,        "HTTP Version not supported"}
};

#endif
