FROM --platform=linux/arm64/v8 debian:trixie-20261027-slim as build

WORKDIR /app
ARG BUILD_TYPE="Release"
RUN apt update
RUN apt install --yes cmake g++
COPY src /app/src
COPY CMakeLists.txt /app/CMakeLists.txt

RUN mkdir build
RUN cd build && cmake .. -DCMAKE_BUILD_TYPE=${BUILD_TYPE}} && make && chmod +x crtboard-display

FROM scratch as release
COPY --from=build /app/build/crtboard-display /bin/crtboard-display
CMD ["echo", "ready"]
