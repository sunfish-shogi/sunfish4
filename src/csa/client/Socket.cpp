/* Socket.cpp
 *
 * Kubo Ryosuke
 */

#include "csa/client/Socket.hpp"
#include "logger/Logger.hpp"

#include <cstring>
#include <csignal>
#include <sys/types.h>
#include <errno.h>

#ifdef WIN32
# include <winsock2.h>
using ssize_t = int;
#else
# include <unistd.h>
# include <dirent.h>
# include <strings.h>
# include <sched.h>
# include <sys/socket.h>
# include <sys/param.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <netdb.h>
#endif

namespace sunfish {

Socket::Socket() :
    keepalive_(0),
    opened_(false) {
}

Socket::~Socket() {
  disconnect();
}

bool Socket::connect() {
  struct hostent* he;
  struct sockaddr_in sin;

  he = gethostbyname(host_.c_str());
  if (he == nullptr) {
    LOG(error) << "an error occured in gethostbyname function. (h_errno: " << h_errno << ")";
    return false;
  }

  sock_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_ == -1) {
    LOG(error) << "an error occured in socket function. (errno: " << errno << ")";
    return false;
  }
  opened_ = true;

#ifdef UNIX
  // keep-alive
  if (0 != setsockopt(sock_, SOL_SOCKET, SO_KEEPALIVE,
      (void*)&keepalive_, sizeof(keepalive_))) {
    LOG(error) << "an error occured in setsockopt function. (errno: " << errno << ")";
    return false;
  }
  if (0 != setsockopt(sock_, IPPROTO_TCP, TCP_KEEPINTVL,
      (void*)&keepintvl_, sizeof(keepintvl_))) {
    LOG(error) << "an error occured in setsockopt function. (errno: " << errno << ")";
    return false;
  }
  if (0 != setsockopt(sock_, IPPROTO_TCP, TCP_KEEPCNT,
      (void*)&keepcnt_, sizeof(keepcnt_))) {
    LOG(error) << "an error occured in setsockopt function. (errno: " << errno << ")";
    return false;
  }
# ifdef BSD
  int keepon = keepidle_ != 0 ? 1 : 0;
  if (0 != setsockopt(sock_, SOL_SOCKET,  SO_KEEPALIVE,
      (void*)&keepon, sizeof(keepon))) {
    LOG(error) << "an error occured in setsockopt function. (errno: " << errno << ")";
    return false;
  }
# else // BSD
  if (0 != setsockopt(sock_, IPPROTO_TCP, TCP_KEEPIDLE,
      (void*)&keepidle_, sizeof(keepidle_))) {
    LOG(error) << "an error occured in setsockopt function. (errno: " << errno << ")";
    return false;
  }
# endif // BSD
#endif // UNIX

  memcpy(&sin.sin_addr, he->h_addr, sizeof(struct in_addr));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port_);
  if (::connect(sock_, (struct sockaddr*)(&sin), sizeof(sin)) == -1) {
    LOG(error) << "an error occured in connet function. (errno: " << errno << ")";
    disconnect();
    return false;
  }

  MSG(info) << "connected to " << host_ << ':' << port_;

  return true;
}

void Socket::disconnect() {
  if (!opened_) {
    return;
  }

#if WIN32
  closesocket(sock_);
#else
  close(sock_);
#endif
  opened_ = false;
  MSG(info) << "disconnected from " << host_ << ':' << port_;
}

bool Socket::receiveString(std::string& out) {
  for (;;) {
    if (getline(receivedStringStream_, out)) {
      return true;
    }

    char buffer[1024];
    ssize_t size = recv(sock_,
                        buffer,
                        sizeof(buffer),
                        0);
    if (size == -1) {
      return false;
    }

    receivedStringStream_.str(std::string(buffer, size));
    receivedStringStream_.clear(std::stringstream::goodbit);
  }
}

bool Socket::sendString(const std::string& str) {
  return ::send(sock_,
                str.c_str(),
                str.length(),
                0) != -1;
}

} // namespace sunfish
