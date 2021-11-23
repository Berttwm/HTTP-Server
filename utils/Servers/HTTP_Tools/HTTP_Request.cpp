#include "HTTP_Request.h"

HTTP_Request::HTTP_Request(const std::string &content)
: m_root_file_name("index.html") // standard search
{
    this->extract(content);
}

const Method HTTP_Request::get_method() const
{
    return m_method;
}

const std::string HTTP_Request::get_uri() const
{
    if(m_uri == "/") return m_root_file_name;
    return m_uri;
}

const std::string HTTP_Request::get_header(const std::string &key) const
{
    auto res = m_misc_headers.find(key);
    if (res == m_misc_headers.end()) {
        return "";
    }
    return res->second;
}
/* extract important information from HTTP request */
void HTTP_Request::extract(const std::string &content)
{
    size_t pos_curr = 0;
    size_t pos_prev = 0;

    // Step 1: Extract Method
    pos_curr = content.find_first_of(' ', pos_prev);
    auto res = method_map.find(content.substr(pos_prev, pos_curr - pos_prev));
    if(res == method_map.end()) 
    {
        perror("No HTTP-Method substring found");
        exit(EXIT_FAILURE);
    }
    m_method = res->second;

    // Step 2: Extract URI
    pos_curr += 1; // advance pos_curr by 1
    pos_prev = pos_curr;
    pos_curr = content.find_first_of(' ', pos_prev);
    if (pos_curr == std::string::npos) {
        perror("No URI substring found");
        exit(EXIT_FAILURE);
    }
    m_uri = content.substr(pos_prev, pos_curr - pos_prev);

    // Step 3: Extract Version
    pos_curr += 1; // advance by one character (currently sitting on whitespace)
    pos_prev = pos_curr;
    pos_curr = content.find_first_of(LINE_ENDING, pos_prev);
    if (pos_curr == std::string::npos || content.substr(pos_prev, (pos_curr - pos_prev) - 1) == HTTP_VERSION) {
        perror("No valid HTTP-Version substring found");
        exit(EXIT_FAILURE);
    }

    // Step 4: Getting all additional headers 
    pos_curr += 2; // advance to next line (currently sitting on CRLF sequence -> '\r\n')
    pos_prev = pos_curr;

    std::string key; // for m_misc_headers (key)
    std::string value; // for m_misc_headers (value)

    while(pos_curr != std::string::npos)
    {
        pos_curr = content.find_first_of(':', pos_prev);
        if (pos_curr == std::string::npos) {
            break;
        }
        key = content.substr(pos_prev, pos_curr - pos_prev);
        pos_curr += 2; // advance by two characters (currently sitting on a colon followed by whitespace)
        pos_prev = pos_curr;

        // value
        pos_curr = content.find_first_of(LINE_ENDING, pos_prev);
        if (pos_curr == std::string::npos) {
            break;
        }
        value = content.substr(pos_prev, pos_curr - pos_prev);
        pos_curr += 2; // advance to next line (currently sitting on CRLF sequence -> '\r\n')
        pos_prev = pos_curr;

        m_misc_headers.insert(std::make_pair(key, value));
    }
    // Completed Extraction

    // debug statement ---
    // for(auto x: m_misc_headers)
    // {
    //     std::cout << x.first << ":" << x.second << std::endl;
    // }

}
// special method for client - generate a generic HTTP stream
std::string HTTP_Request::generate_HTTP_Request()
{
    std::string response;

    /* HTTP_Response creation */
    // step 1: HTTP GET header 
    response.append("GET");
    response.append(" ");
    response.append("/");
    response.append(" ");
    response.append(HTTP_VERSION);
    response.append(LINE_ENDING);

    // step 2: HTTP Host header 
    response.append("Host: ");
    response.append("127.0.0.1:12346");
    response.append(LINE_ENDING);

    // step 3: HTTP User-Agent
    response.append("User-Agent: ");
    response.append("Mozilla/5.0 (X11; U; SunOS sun4v; en-US; rv:1.7) Gecko/20140814");
    response.append(LINE_ENDING);

    // step 4: HTTP Accept header 
    response.append("ccept: ");
    response.append("text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5");
    response.append(LINE_ENDING);

    // step 5: HTTP Accept-Language header
    response.append("Accept-Language: ");
    response.append("en-us,en;q=0.5");
    response.append(LINE_ENDING);

    // step 6: Accept-Encoding heading
    response.append("Accept-Encoding: ");
    response.append("gzip,deflate");
    response.append(LINE_ENDING);
    
    // step 7: Accept-Charset heading
    response.append("Accept-Charset: ");
    response.append("ISO-8859-1,utf-8;q=0.7,*;q=0.7");
    response.append(LINE_ENDING);

    // step 8: Keep-Alive heading
    response.append("Keep-Alive: ");
    response.append("300");
    response.append(LINE_ENDING);

    // step 9: Connection heading
    response.append("Connection: ");
    response.append("keep-alive");
    response.append(LINE_ENDING);

    return response;
}
