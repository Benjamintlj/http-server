
default: all

all: server client

run-server: server
	./server/build/main

server:
	gcc server/main.c -o ./server/build/main

client:
	gcc client/main.c -o ./client/build/main

clean:
	rm -f ./server/build/* ./client/build/*