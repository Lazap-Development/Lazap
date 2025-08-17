#include "addons/discord_rpc/RichPresence.h"

#include <csignal>
#include <iostream>
#include <string>

#include "addons/discord_rpc/discord_rpc.h"

std::time_t startTime;
bool RichPresence::initialized = false;
auto& rpcManager = discord::RPCManager::get();

void RichPresence::Initialize(const std::string& applicationId) {
  if (initialized) return;

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
}

void RichPresence::UpdatePresence(const std::string& details,
                                  const std::string& state) {
  if (!initialized) return;

  rpcManager.getPresence()
      .setState(state)
      .setDetails(details)
      .setStartTimestamp(startTime)
      .setLargeImageKey("spoiledunknown")
      .setSmallImageKey("github")
      .setLargeImageText("Large Image Text")
      .setSmallImageText("Small Image Text")
      .setInstance(false)
      .refresh();
}

void RichPresence::Shutdown() { rpcManager.shutdown(); }
