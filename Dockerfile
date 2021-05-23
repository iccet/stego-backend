FROM gcc:latest

ARG BUILD_DIR=build

RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y build-essential software-properties-common python-dev autotools-dev libblas-dev libicu-dev libbz2-dev libboost-all-dev
RUN apt-add-repository "deb http://security.debian.org/debian-security stretch/updates main"

RUN apt-get update
RUN apt-get install -y qt5-default cmake libgl1-mesa-dev

WORKDIR /app

COPY . ./

RUN cmake -DCMAKE_BUILD_TYPE=Debug -S . -B ${BUILD_DIR}

WORKDIR ${BUILD_DIR}

RUN cmake --build . --target StegoBackend -- -j 3

ENTRYPOINT [ "/bin/bash", "./StegoBackend" ]
