#pragma once
#ifndef DISCORD_WINDOWS_H
#define DISCORD_WINDOWS_H

#define WIN32_LEAN_AND_MEAN
#define NOMCX
#define NOSERVICE
#define NOIME
#define NOMINMAX
#include <Windows.h>

#include <cstdint>

namespace discord::platform {
size_t getProcessID() noexcept;

class PipeConnection {
  PipeConnection() noexcept = default;

 public:
  static PipeConnection& get() noexcept;

  bool open() noexcept;
  bool close() noexcept;

  bool write(const void* data, size_t length) const noexcept;
  bool read(void* data, size_t length) noexcept;

  [[nodiscard]] bool isOpen() const noexcept { return m_isOpen; }

 private:
  // Unix methods for Wine compatibility
  bool openUnix() noexcept;
  bool closeUnix() noexcept;
  bool writeUnix(const void* data, size_t length) const noexcept;
  bool readUnix(void* data, size_t length) noexcept;

  HANDLE m_pipe = INVALID_HANDLE_VALUE;
  bool m_isOpen = false;
  bool m_useWineFallback = false;
};
}  // namespace discord::platform

#endif  // DISCORD_WINDOWS_H
