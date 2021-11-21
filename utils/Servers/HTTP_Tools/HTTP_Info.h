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

#endif
