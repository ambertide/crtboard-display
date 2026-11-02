#include "Render.hpp"
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

crtboard::Render::Render() {
  this->renderFileDescriptor = open("/dev/fb0", O_RDWR);
  ioctl(this->renderFileDescriptor, FBIOGET_FSCREENINFO, &this->fixedInfo);
  ioctl(this->renderFileDescriptor, FBIOGET_VSCREENINFO, &this->userInfo);
  this->memsize = this->userInfo.xres * this->userInfo.yres *
                  this->userInfo.bits_per_pixel / 8;
  this->screenMemory =
      (uint8_t *)mmap(0, this->memsize, PROT_READ | PROT_WRITE, MAP_SHARED,
                      this->renderFileDescriptor, 0);
}

crtboard::Render::~Render() {
  munmap(this->screenMemory, this->memsize);
  close(this->renderFileDescriptor);
}

void crtboard::Render::render(std::unique_ptr<uint8_t[]> data, int size) {
  if (this->locked) {
    return;
  }
  // This is actually probably unnecessary atp
  // since we have a single threaded event loop but
  // at some point I may go std::thread brr
  // at which point this will be relevant
  // but even then this is a horrible solution
  // unsure what is a proper solution, probably some sort of
  // queue or whatever.
  this->locked = true;
  std::cout << "Writing to screen memory data of size: " << this->memsize;
  // However the screen uses RGB565 to fuck with us
  // so a conversion is necessary here, this is probably somewhat vectorizable
  std::memcpy(this->screenMemory, data.get(), this->memsize);
  this->locked = false;
}
