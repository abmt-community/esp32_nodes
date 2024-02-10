#include "tcp.h"

#include "../../../main/setup_wifi.h"

using namespace std;
using namespace esp32_nodes;

tcp_rr::tcp_rr(tcp_server_rr* s, int f): srv(s), fd(f){
    fcntl(fd, F_SETFL, O_NONBLOCK);
    int nodelay = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay));
}

void tcp_rr::read(){
    int len = 0;
    char rx_buffer[512];
    do {
        len = recv(fd, rx_buffer, sizeof(rx_buffer), 0);
        if(len > 0){
            new_data = true;
            std::string s(rx_buffer,len);
            data += {rx_buffer,len};
        }
        if(len == 0){
            close();
        }
    } while(len > 0);
}

void tcp_rr::send(void* data, size_t size){
    if(closed == false){
        ::send(fd,data,size,0);
    }
}

void tcp_rr::close(){
    if(closed == false){
        closed = true;
        ::close(fd);
    }
}

tcp_rr::~tcp_rr(){
    close();
}

void tcp_server_rr::open_socket(){
    int err = 0;
    
    listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    abmt::die_if(listen_fd == 0, "error creating socekt");
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(param_port);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    
    err = bind(listen_fd, (struct sockaddr *)&sa, sizeof(sa));
    abmt::die_if(err != 0, "error binding socekt");
    err = listen(listen_fd, 1);
    abmt::die_if(err != 0, "error listening to socekt");
    fcntl(listen_fd, F_SETFL, O_NONBLOCK);
    
    socket_opened = true;
}

abmt::time tcp_server_rr::poll(){
    if(wifi_connected == false){
        if(socket_opened){
            close_socket();
        }
        return abmt::time::sec(3);
    } else if(socket_opened == false){
        open_socket();
    }
    
    int con_fd = accept(listen_fd, NULL, NULL);
    
    if(con_fd < 0){
        //abmt::log("no new connection");
    }else{
        connections.push_back(make_shared<tcp_rr>(this, con_fd));
    }
    
    fd_set set;
    int max_fd = listen_fd;
    FD_ZERO (&set);
	FD_SET (listen_fd, &set);
    
    for (auto it = connections.begin() ; it != connections.end(); ) {
        (*it)->read();
        if((*it)->new_data){
            (*it)->new_data = false;
            out_rr = (*it);
            return 0;
        }
        if ( (*it)->closed && (*it)->new_data == false ) {
            // only close when there is no new data.
            it = connections.erase(it);
        } else {
            FD_SET ((*it)->fd, &set);
            if((*it)->fd > max_fd){
                max_fd = (*it)->fd;
            }
            ++it;
        }
    }
    
    struct timeval tv;
    tv.tv_sec = 5;
	tv.tv_usec = 0;
	select (max_fd + 1, &set, NULL, NULL, &tv);
    return 1;
}

void tcp_server_rr::close_socket(){
    socket_opened = false;
    connections.clear();
    close(listen_fd);
}
