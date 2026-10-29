FROM debian:13.7

WORKDIR /app
COPY src /app/src
COPY CMakeLists.txt /app/CMakeLists.txt

RUN apt install clang
RUN apt install cmake


