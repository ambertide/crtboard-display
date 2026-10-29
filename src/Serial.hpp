#pragma once
#include "PacketType.hpp"
#include <functional>
#include <memory>
#include <stdint.h>
#include <unordered_map>
#include <vector>

namespace crtboard {

typedef std::function<void(std::unique_ptr<uint8_t[]>, int)>
    OnPacketRecvCallback;

class Serial {
private:
  std::unordered_map<PacketType, OnPacketRecvCallback> callbacks;
  std::vector<uint8_t> bytebuffer;
  int serialFileDescriptor;

  uint8_t readByte();
  PacketType parsePacketType();
  int ParsePacketSize();

public:
  Serial();
  ~Serial();

  /**
   * @brief Register a callback for a specific packet receive event.
   *
   * @param type Type of the packet to trigger the callback
   * @param onTypeReceived Callback to be triggered.
   */
  void registerPacketCallback(PacketType type,
                              OnPacketRecvCallback onTypeReceived);

  /**
   * @brief Recieve a single CRTBOARD packet
   *
   * @return Packet data
   */
  void recvPacket();
};
} // namespace crtboard
