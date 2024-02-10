#include "udp.h"

#include "../../../main/setup_wifi.h"

using namespace std;
using namespace esp32_nodes;

void udp_rr_sendto::send(void* data, size_t size){
    if(fd == 0){
        return;
    }
    int err = sendto(fd, data, size, 0, (struct sockaddr *)&src_addr, sizeof(src_addr));
    if (err < 0) {
        abmt::log("error udp_rr_sendto");
    }
}

void udp_rr::init(){

    rr.reset(new udp_rr_sendto);
    rr->data.realloc(param_rx_buff_size);
    out_rr = rr;
}

void udp_rr::open_socket(){
    int err = 0;
    
    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    abmt::die_if(fd == 0, "error creating socekt");
    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    //struct timeval timeout;
    //timeout.tv_sec = 10;
    //timeout.tv_usec = 0;
    //setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(param_port);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    
    err = bind(fd, (struct sockaddr *)&sa, sizeof(sa));
    abmt::die_if(err != 0, "error binding socekt");
    rr->fd = fd;
    socket_opened = true;
}

abmt::time udp_rr::poll(){
    if(wifi_connected == false){
        if(socket_opened){
            close_socket();
        }
        return abmt::time::sec(3);
    } else if(socket_opened == false){
        open_socket();
    }
    
    int rcv_len = recvfrom(fd, out_rr->data.data, param_rx_buff_size, 0, (struct sockaddr *)&rr->src_addr, (socklen_t*)&src_len);
    if(rcv_len < 0){
       return 1;
    }
    out_rr->data.size = rcv_len;
    return 0;
}

void udp_rr::close_socket(){
    socket_opened = false;
    rr->fd = 0;
    close(fd);
}



void udp_snd::open_socket(){
    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(param_port);
    sa.sin_addr.s_addr = inet_addr(param_target.c_str());
    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    socket_opened = true;
}

void udp_snd::tick(){
    if(wifi_connected == false){
        if(socket_opened){
            close_socket();
        }
        return;
    } else if(socket_opened == false){
        open_socket();
    }
    
    char* data = in.data;
    uint32_t size = in.size;
    if(in.size == 0){
        return;
    }
    sendto(fd, in.data, in.size, 0, (struct sockaddr *)&sa, sizeof(sa));
}
void udp_snd::close_socket(){
    socket_opened = false;
    close(fd);
}
