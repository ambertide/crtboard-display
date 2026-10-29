#include "Serial.hpp"
#include "PacketType.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <exception>
#include <fcntl.h>
#include <memory>
#include <termios.h>
#include <unistd.h>

crtboard::Serial::Serial() {
  this->serialFileDescriptor =
      open("/dev/ttyGS0", O_RDWR | O_NOCTTY | O_NDELAY);
  struct termios config;
  tcgetattr(this->serialFileDescriptor, &config);
  config.c_iflag &=
      ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
  /* set raw input, 1 character trigger */
  /* raw mode, like Version 7 terminal driver */
  cfmakeraw(&config);
  config.c_cflag |= CS8 | CREAD | CLOCAL;

  cfsetospeed(&config, B38400);
  cfsetispeed(&config, B38400);
  if (tcsetattr(this->serialFileDescriptor, TCSANOW, &config) < 0) {
    printf("Unable to properly set tty\n");
  }
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

uint8_t crtboard::Serial::readByte() {
  uint8_t nextChar;
  int readNByte = 0;
  do {
    readNByte = read(this->serialFileDescriptor, &nextChar, 1);
  } while (readNByte != 1);
  return nextChar;
}

crtboard::PacketType crtboard::Serial::parsePacketType() {
  if (this->serialFileDescriptor < 0) {
    printf("Error: serial file not properly opened\n");
    throw std::exception();
  }
  printf("Attempting to read\n");
  uint8_t nextChar = this->readByte();
  printf("READ %x\n", nextChar);
  switch (nextChar) {
  case 0:
    return PacketType::DEBUG;
  default:
    throw std::exception();
  }
}

int crtboard::Serial::ParsePacketSize() {
  if (this->serialFileDescriptor < 0) {
    printf("Error: serial file not properly opened\n");
    throw std::exception();
  }
  uint8_t lsb = this->readByte();
  uint8_t rab = this->readByte();
  uint8_t msb = this->readByte();
  return (lsb << 16) | (rab << 8) | msb;
}

void crtboard::Serial::recvPacket() {
  auto type = this->parsePacketType();
  auto size = this->ParsePacketSize();
  printf("Attempting to read %d of type %d\n", size, type);
  if (size == 0) {
    return;
  }
  for (int i = 0; i < size; i++) {
    this->bytebuffer.push_back(this->readByte());
  }
  // After read we need to extract and empty the bytebuffer.
  auto payload = std::make_unique<uint8_t[]>(size);
  // Copy the memory of to payload
  std::memcpy(payload.get(), this->bytebuffer.data(), size);
  // Delete the byte buffer, btw we probably never needed a byte buffer
  // but that's neither here nor there
  this->bytebuffer.clear();
  // Finally call to the callback.
  if (this->callbacks[type]) {
    this->callbacks[type](std::move(payload), size);
  }
}
