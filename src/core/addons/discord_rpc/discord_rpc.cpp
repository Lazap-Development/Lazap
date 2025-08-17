#include <addons/discord_rpc/discord_rpc.h>

#ifndef DISCORD_DISABLE_IO_THREAD
#include <algorithm>
#include <condition_variable>
#include <iostream>
#include <random>
#include <thread>
#endif

#include "addons/discord_rpc/platform.h"
#include "addons/discord_rpc/rpc_connection.h"

namespace {
class Backoff {
 public:
  static Backoff& get() noexcept {
    static Backoff instance;
    return instance;
  }

  double rand01() noexcept { return m_distribution(m_generator); }

  void reset() noexcept {
    m_currentAmount = m_minAmount;
    m_attempts = 0;
  }

  int64_t next() noexcept {
    m_attempts++;
    auto delay = static_cast<int64_t>(static_cast<double>(m_currentAmount) *
                                      2.0 * rand01());
    m_currentAmount = std::min(m_maxAmount, m_currentAmount + delay);
    return m_currentAmount;
  }

 private:
  int64_t m_minAmount = 500;
  int64_t m_maxAmount = 60000;
  int64_t m_currentAmount = m_minAmount;
  int32_t m_attempts = 0;
  std::mt19937_64 m_generator{std::random_device{}()};
  std::uniform_real_distribution<> m_distribution{0.0, 1.0};
};
}  // anonymous namespace

namespace discord {
#ifdef DISCORD_DISABLE_IO_THREAD
struct IOWorker {
  void start() {}
  void stop() {}
  void notify() {}
};
#else
struct IOWorker {
  IOWorker() noexcept = default;
  ~IOWorker() noexcept { stop(); }

  void start() {
    m_running.store(true);
    m_thread = std::thread([this]() {
      constexpr auto timeout = std::chrono::milliseconds(500);
      auto& rpc = RPCManager::get();
      rpc.update();
      while (m_running.load()) {
        std::unique_lock lock(m_waitForIO);
        m_ioReady.wait_for(lock, timeout);
        rpc.update();
      }
    });
  }

  void stop() {
    m_running.store(false);
    notify();
    if (m_thread.joinable()) {
      m_thread.join();
    }
  }

  void notify() { m_ioReady.notify_one(); }

 private:
  std::thread m_thread{};
  std::atomic_bool m_running = true;
  std::mutex m_waitForIO{};
  std::condition_variable m_ioReady{};
};
#endif

Presence& Presence::get() noexcept { return RPCManager::get().getPresence(); }

Presence& Presence::clear() noexcept {
  m_state.clear();
  m_details.clear();
  m_startTimestamp = 0;
  m_endTimestamp = 0;
  m_largeImageKey.clear();
  m_largeImageText.clear();
  m_smallImageKey.clear();
  m_smallImageText.clear();
  m_partyID.clear();
  m_partySize = 0;
  m_partyMax = 0;
  m_partyPrivacy = PartyPrivacy::Private;
  m_matchSecret.clear();
  m_joinSecret.clear();
  m_spectateSecret.clear();
  m_buttons[0] = Button{};
  m_buttons[1] = Button{};
  m_instance = false;
  return *this;
}

void Presence::refresh() const noexcept { RPCManager::get().refresh(); }

RPCManager& RPCManager::initialize() noexcept {
  if (m_initialized) {
    return *this;
  }

  m_ioWorker = new (std::nothrow) IOWorker();
  if (m_ioWorker) {
    m_ioWorker->start();
  }

  m_processID = platform::getProcessID();
  m_initialized = true;

  return *this;
}

RPCManager& RPCManager::shutdown() noexcept {
  if (!m_initialized) {
    return *this;
  }

  if (m_ioWorker) {
    delete m_ioWorker;
    m_ioWorker = nullptr;
  }

  Connection::get().close();
  m_initialized = false;

  return *this;
}

RPCManager& RPCManager::update() noexcept {
  if (!m_initialized) {
    return *this;
  }

  auto& conn = Connection::get();
  if (!conn.isOpen()) {
    if (std::chrono::system_clock::now() < m_nextConnect) {
      return *this;
    }

    updateReconnectTime();
    conn.open(m_clientID);
    return *this;
  }

  // reading
  do {
    std::string buffer;
    if (!conn.read(buffer)) {
      break;
    }

    // fmt::println("Received: {}", buffer);
  } while (true);

  // writing
  // using size to avoid going into infinite loop when requeuing commands
  auto size = m_commandQueue.size();
  if (size > 0) {
    for (size_t i = 0; i < size; ++i) {
      if (auto cmd = m_commandQueue.pop()) {
        if (!conn.write(*cmd)) {
          // requeue
          m_commandQueue.push(std::move(*cmd));
        }
      }
    }
  }

  return *this;
}

RPCManager& RPCManager::refresh() noexcept {
  // add the presence to queue
  auto& msg = m_commandQueue.prepare();
  serializePresence(msg, m_presence, m_processID, m_nonce++);
  m_commandQueue.finish();

  // notify the io worker
  if (m_ioWorker) {
    m_ioWorker->notify();
  }

  return *this;
}

RPCManager& RPCManager::clearPresence() noexcept {
  m_presence.clear();

  // add the presence to queue
  auto& msg = m_commandQueue.prepare();
  serializeEmptyPresence(msg, m_processID, m_nonce++);
  m_commandQueue.finish();

  // notify the io worker
  if (m_ioWorker) {
    m_ioWorker->notify();
  }

  return *this;
}

void RPCManager::updateReconnectTime() noexcept {
  m_nextConnect = std::chrono::system_clock::now() +
                  std::chrono::milliseconds(Backoff::get().next());
}

std::time_t startTime;
bool RichPresence::initialized = false;
auto& rpcManager = RPCManager::get();

void RichPresence::Initialize(const std::string& applicationId) {
  if (initialized) return;
  std::cout << "Initializing Discord Rich Presence...\n";

  rpcManager.setClientID(applicationId)
      .onReady([](discord::User const& user) {
        std::cout << "Client Ready! User: " << user.username << "#"
                  << user.discriminator << " (" << user.id << ")\n";
      })
      .onDisconnected([](int errcode, std::string_view message) {
        std::cout << "Client disconnected: " << errcode << " - " << message;
      })
      .onErrored([](int errcode, std::string_view message) {
        std::cout << "Discord: error with code: " << errcode << message;
      });

  rpcManager.initialize();

  startTime = std::time(nullptr);

  initialized = true;
  std::cout << "Discord Rich Presence initialized successfully.\n";
}

void RichPresence::UpdatePresence(const std::string& state,
                                  const std::string& details,
                                  const std::string& imageText,
                                  const std::string& imageKey) {
  if (!initialized) return;

  rpcManager.getPresence()
      .setState(state)
      .setDetails(details)
      .setStartTimestamp(startTime)
      .setLargeImageKey(imageKey)
      .setLargeImageText(imageText)
      .setInstance(false)
      .refresh();
}

void RichPresence::Shutdown() { rpcManager.shutdown(); }
}  // namespace discord
