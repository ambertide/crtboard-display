# `crtboard-display`

This repository contains the software used to control the
Raspberry Pi 4 B+ for crtboard

## Build

Since I develop on an arm mac and deploy on a arm Raspberry Pi,
I elected to use docker for builds if you:

```sh
env CRTBOARD_HOST=... BUILD_TYPE=Debug ./build.sh
```

This automatically create a container containing the linux/arm64 build
of `crtboard-display` as a Debug build and deploy to your raspberry
pi's `/home/crt` directory.
