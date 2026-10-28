#include "Serial.hpp"
#include "PacketType.hpp"
#include <fcntl.h>
#include <memory>
#include <unistd.h>

crtboard::Serial::Serial() {
  this->serialFileDescriptor = open("/dev/ttyGS0", O_RDWR | O_NOCTTY);
}

crtboard::Serial::~Serial() {
  if (this->serialFileDescriptor >= 0) {
    close(this->serialFileDescriptor);
  }
}

void crtboard::Serial::registerPacketCallback(
    crtboard::PacketType type, crtboard::OnPacketRecvCallback onTypeRecv) {
  this->callbacks[type] = onTypeRecv;
}

void crtboard::Serial::recvPacket() {
  uint8_t packetType;
  read(this->serialFileDescriptor, &packetType, 1);
  auto pType = static_cast<PacketType>(packetType);
  int payloadSize;
  read(this->serialFileDescriptor, &payloadSize, 3);
  auto payload = std::make_unique<uint8_t>(payloadSize);
  if (payloadSize > 0) {
    read(this->serialFileDescriptor, payload.get(), payloadSize);
    if (this->callbacks[pType]) {
      this->callbacks[pType](std::move(payload), payloadSize);
    }
  }
}
