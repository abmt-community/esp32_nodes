#include "esp_http.h"


#include <abmt/util/str_utils.h>

using namespace std;
using namespace esp32_nodes;

void esp_http::tick(){
    auto data = in_rr->data.str();
    auto hdr_end = data.find("\r\n\r\n");
    if(hdr_end != std::string::npos){
        if (data.rfind("GET ", 0) == 0) {
            auto file_name = data.substr(4,data.find(" ",4) - 4);
            if(file_name.size() == 0){
                string rsp = "HTTP/1.1 500 Internal Server Error\r\n\r\n<html><body>no file</body></html>";
                in_rr->send(rsp);
                return;
            }
            if(file_name[file_name.size()-1] == '/'){
                file_name += "index.html";
            }
            auto file = html_files.find(file_name);
            string rsp;
            if(file != html_files.end()){
                rsp = "HTTP/1.1 200 OK\r\nContent-Encoding:gzip\r\n\r\n";// + file->second;
                in_rr->send(rsp);
                in_rr->send((void*)file->second.p, file->second.s);
                //in_rr->send((void*)file->second.data(),file->second.size());
            }else{
                rsp = "HTTP/1.1 404 Not Found\r\n\r\n<html><body>404 file not found</body></html>";
                in_rr->send(rsp);
            }
            in_rr->close();
        }
    } // hdr complete
}