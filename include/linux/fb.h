struct fb_var_screeninfo {
  int xoffset;
  int bits_per_pixel;
  int yoffset;
  int xres;
  int yres;
};

struct fb_fix_screeninfo {
  int line_length;
};

const unsigned long FBIOGET_VSCREENINFO = 0;
const unsigned long FBIOGET_FSCREENINFO = 1;
