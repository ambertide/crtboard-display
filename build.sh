docker buildx build -t crtboard-display:latest .
container=$(docker create crtboard-display:latest)
mkdir build
docker cp ${container}:/bin/crtboard-display ./build/crtboard-build
docker rm -f ${container}
scp build/crtboard-build $CRTBOARD_HOST:/home/crt/crtboard-display