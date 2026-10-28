#include "PacketType.hpp"
#include "Serial.hpp"
#include "stdio.h"
#include <iostream>
#include <ostream>

int main() {
  crtboard::Serial serial;
  serial.registerPacketCallback(crtboard::PacketType::DEBUG,
                                [](auto payload, auto size) {
                                  std::cout << "Recieved debug frame with size "
                                            << size << "Frames: " << std::endl;
                                  for (int i = 0; i < size; i++) {
                                    std::cout << std::hex << payload.get()[i];
                                  }
                                  std::cout << std::endl;
                                });
}
