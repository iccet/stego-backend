FROM gcc:latest

ARG BUILD_DIR=build

RUN apt-get -q update && apt-get install -y software-properties-common
RUN apt-add-repository "deb http://security.debian.org/debian-security stretch/updates main"

RUN apt-get -q update && apt-get upgrade -y > /dev/null
RUN apt-get install -y libblas-dev libicu-dev libbz2-dev
RUN apt-get install -y libboost-dev libboost-program-options-dev libboost-log-dev
RUN apt-get install -y qt5-default cmake libgl1-mesa-dev librdkafka-dev

WORKDIR /app

COPY . ./

RUN cmake -DCMAKE_BUILD_TYPE=Debug -S . -B ${BUILD_DIR}

WORKDIR ${BUILD_DIR}

RUN cmake --build . --target StegoBackend -- -j 3

ENTRYPOINT [ "/bin/bash", "./StegoBackend" ]
