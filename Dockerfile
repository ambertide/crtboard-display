FROM --platform=linux/arm64/v8 debian:trixie-20261027-slim as build

WORKDIR /app

RUN apt update
RUN apt install --yes cmake g++
COPY src /app/src
COPY CMakeLists.txt /app/CMakeLists.txt

RUN mkdir build
RUN cd build && cmake .. && make && chmod +x crtboard-display

FROM scratch
COPY --from=build /app/build/crtboard-display /bin/crtboard-display
CMD ["echo", "ready"]