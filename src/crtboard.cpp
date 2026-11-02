#include "PacketType.hpp"
#include "Render.hpp"
#include "Serial.hpp"
#include "stdio.h"
#include <cstdint>
#include <iostream>
#include <memory>
#include <ostream>

int main() {
  crtboard::Serial serial;
  crtboard::Render render;
  serial.registerPacketCallback(
      crtboard::PacketType::DEBUG, [](auto payload, auto size) {
        std::cout << "Recieved debug frame with size " << size
                  << "Frames: " << std::endl;
        for (int i = 0; i < size; i++) {
          std::cout << std::hex << payload.get()[i] << std::endl;
        }
        std::cout << std::endl;
      });
  serial.registerPacketCallback(
      crtboard::PacketType::DRAW,
      [&render](std::unique_ptr<std::uint8_t[]> payload, auto size) {
        std::cout << "Reading data of size..." << size;
        render.render(std::move(payload, size));
      });
  while (true) {
    serial.recvPacket();
  }
}
