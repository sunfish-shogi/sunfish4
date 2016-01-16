/* Socket.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_CSA_CLIENT_SOCKET_HPP__
#define SUNFISH_CSA_CLIENT_SOCKET_HPP__

#include "common/Def.hpp"

#include <sstream>
#include <string>

namespace sunfish {

class Socket {
public:

  using SocketType = int;

  class AutoDisconnector {
  public:

    AutoDisconnector(Socket& socket) : socket_(socket) {
    }

    ~AutoDisconnector() {
      socket_.disconnect();
    }

  private:

    Socket& socket_;

  };

  Socket();
  Socket(const Socket&) = delete;
  Socket(Socket&&) = delete;

  ~Socket();

  template <class T>
  void setHost(T&& host) {
    host_ = host;
  }

  void setPort(int port) {
    port_ = port;
  }

  void setKeepalive(int keepalive,
                    int keepidle,
                    int keepintvl,
                    int keepcnt) {
    keepalive_ = keepalive;
    keepidle_ = keepidle;
    keepintvl_ = keepintvl;
    keepcnt_ = keepcnt;
  }

  bool connect();

  void disconnect();

  bool receiveString(std::string& out);

  bool sendString(const std::string& str);

private:

  std::string host_;
  int port_;

  int keepalive_;
  int keepidle_;
  int keepintvl_;
  int keepcnt_;

  SocketType sock_;
  bool opened_;

  std::istringstream receivedStringStream_;

};

} // namespace sunfish

#endif // SUNFISH_CSA_CLIENT_SOCKET_HPP__
