#ifndef ESP32_NODES_UDP_H
#define ESP32_NODES_UDP_H ESP32_NODES_UDP_H

#include <abmt/blob.h>
#include <abmt/os.h>
#include <abmt/time.h>
#include <string>
#include <cstdint>
#include <lwip/err.h>
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/netdb.h>
#include <lwip/udp.h>
#include <lwip/tcpip.h>
#include <abmt/io/rcv_rsp.h>

namespace esp32_nodes{
    
struct udp_rr_sendto: public abmt::io::rcv_rsp{
    struct sockaddr_storage src_addr;
    int fd = 0;
    
    void send(void* data, size_t size);
};
        
//@node: auto
//@raster: source
struct udp_rr{
    std::shared_ptr<udp_rr_sendto> rr;
    abmt::io::rcv_rsp_ptr out_rr;
    
    uint16_t param_port = 12345;
    size_t param_rx_buff_size = 4096;
    
    int src_len = sizeof(sockaddr_storage);
    int fd = 0;
    
    bool socket_opened = false;
    void open_socket();
    void close_socket();
    
    void init();
    abmt::time poll();
};

//@node: auto
struct udp_snd{
    abmt::blob_shared in;
    std::string param_target = "192.168.1.1";
    uint16_t param_port = 12345;
    struct sockaddr_in sa;
    int fd;
    
    bool socket_opened = false;
    void open_socket();
    void close_socket();
    void tick();
};


} // namespace esp32_nodes

#endif // ESP32_NODES_UDP_H
