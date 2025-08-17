#pragma once
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include <string>

namespace discord::platform {
inline size_t getProcessID() noexcept { return ::getpid(); }

inline const char* getTempPath() noexcept {
  static const char* path = []() {
    const char* tmp = ::getenv("XDG_RUNTIME_DIR");
    tmp = tmp ? tmp : ::getenv("TMPDIR");
    tmp = tmp ? tmp : ::getenv("TMP");
    tmp = tmp ? tmp : ::getenv("TEMP");
    return tmp ? tmp : "/tmp";
  }();
  return path;
}

class PipeConnection {
  PipeConnection() noexcept { m_address.sun_family = AF_UNIX; }

 public:
  static PipeConnection& get() noexcept {
    static PipeConnection instance;
    return instance;
  }

  bool open() noexcept {
    if (m_isOpen || m_socket != -1) {
      return false;
    }

    m_socket = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (m_socket == -1) {
      return false;
    }

    ::fcntl(m_socket, F_SETFL, O_NONBLOCK);
#ifdef SO_NOSIGPIPE
    int optval = 1;
    ::setsockopt(m_socket, SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval));
#endif

    for (int i = 0; i < 10; ++i) {
      std::string path =
          std::string(getTempPath()) + "/discord-ipc-" + std::to_string(i);
      std::strncpy(m_address.sun_path, path.c_str(),
                   sizeof(m_address.sun_path) - 1);
      m_address.sun_path[sizeof(m_address.sun_path) - 1] =
          '\0';  // null-terminate

      if (::connect(m_socket, reinterpret_cast<sockaddr*>(&m_address),
                    sizeof(m_address)) == 0) {
        m_isOpen = true;
        return true;
      }
    }

    m_socket = -1;
    return false;
  }

  bool close() noexcept {
    if (!m_isOpen || m_socket == -1) {
      return false;
    }

    ::close(m_socket);
    m_socket = -1;
    m_isOpen = false;
    return true;
  }

  [[nodiscard]] bool isOpen() const noexcept { return m_isOpen; }

  bool write(const void* data, size_t length) noexcept {
    if (!m_isOpen || m_socket == -1) {
      return false;
    }

    ssize_t written = ::send(m_socket, data, length, MSG_FLAGS);
    if (written < 0) {
      this->close();
      return false;
    }

    return static_cast<size_t>(written) == length;
  }

  bool read(void* data, size_t length) noexcept {
    if (!m_isOpen || m_socket == -1) {
      return false;
    }

    ssize_t received = ::recv(m_socket, data, length, MSG_FLAGS);
    if (received < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        return false;
      }

      this->close();
      return false;
    }

    if (received == 0) {
      this->close();
      return false;
    }

    return static_cast<size_t>(received) == length;
  }

 private:
#ifdef MSG_NOSIGNAL
  static constexpr int MSG_FLAGS = MSG_NOSIGNAL;
#else
  static constexpr int MSG_FLAGS = 0;
#endif

  sockaddr_un m_address{};
  int m_socket = -1;
  bool m_isOpen = false;
};
}  // namespace discord::platform
