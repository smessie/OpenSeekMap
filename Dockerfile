FROM gcc:latest

RUN apt update
RUN apt upgrade -y
RUN apt install -y valgrind

WORKDIR /code

CMD sleep 99999999
