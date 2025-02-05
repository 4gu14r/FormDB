FROM gcc:latest

WORKDIR /usr/src/app

COPY ./src .

CMD ["gcc", "main.c", "-o", "app", "&&", "./app"]