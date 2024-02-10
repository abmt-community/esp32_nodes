#ifndef ESP32_NODES_TCP_H
#define ESP32_NODES_TCP_H ESP32_NODES_TCP_H

#include <abmt/blob.h>
#include <abmt/os.h>
#include <abmt/time.h>
#include <abmt/mutex.h>
#include <abmt/io/rcv_rsp.h>
#include <string>
#include <cstdint>
#include <lwip/err.h>
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/netdb.h>
#include <lwip/tcp.h>
#include <lwip/tcpip.h>

namespace esp32_nodes{
    
struct tcp_server_rr;

struct tcp_rr: public abmt::io::rcv_rsp{

    volatile bool new_data = false;
    volatile bool closed = false;
    
    tcp_server_rr* srv;
    int fd;

    void read();
    
    tcp_rr(tcp_server_rr* s, int fd);
    virtual void send(void* data, size_t size);
	virtual void close();
	virtual ~tcp_rr();
    
};

//@node: auto
//@raster: source
struct tcp_server_rr{
    uint16_t param_port = 12345;
    abmt::io::rcv_rsp_ptr out_rr;
    
    std::vector<std::shared_ptr<tcp_rr>> connections;
    abmt::mutex server_lock;
    
    int listen_fd = 0;

    bool loop_is_locked = false;
    bool socket_opened = false;
    void open_socket();
    void close_socket();
    
    abmt::time poll();
};


} // namespace esp32_nodes

#endif // ESP32_NODES_TCP_H
