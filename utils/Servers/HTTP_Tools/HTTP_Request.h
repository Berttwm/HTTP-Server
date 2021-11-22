#ifndef HTTPTools_Request_H
#define HTTPTools_Request_H

#include "HTTP_Info.h"

class HTTP_Request
{
private:
    std::unordered_map<std::string, std::string> m_misc_headers;
    Method m_method;
    std::string m_uri;

    std::string m_root_file_name; // root file in case URI equals '/' (i.e. '/' = index.html)

    void extract(const std::string &content);


public:
    HTTP_Request(const std::string &content);

    const Method get_method() const;
    const std::string get_uri() const;
    const std::string get_header(const std::string &key) const;

};

#endif // HTTPTools_Request_H

/* The request I am basing my protocol on */
// GET / HTTP/1.1
// Host: 127.0.0.1:12346
// User-Agent: Mozilla/5.0 (X11; U; SunOS sun4v; en-US; rv:1.7) Gecko/20140814
// Accept: text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5
// Accept-Language: en-us,en;q=0.5
// Accept-Encoding: gzip,deflate
// Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7
// Keep-Alive: 300
// Connection: keep-alive
