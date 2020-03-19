CFLAGS = -g -std=c++17 -fpermissive -pthread -Wall -Werror
LIB_DIR = lib/
LIBS= 
SRC_DIR = src/
BUILD_DIR = bin/
FIND_RECUR = $(shell find src -type f -name "*.cpp")

objects:  
	g++ -c $(SRC_DIR)*.cpp  $(FIND_RECUR) $(CFLAGS)

server: objects
	@echo "** Building the server"
	g++ -o $(BUILD_DIR)server.out server_main.o server.o $(CFLAGS) $(LIBS)

client: objects
	@echo "** Building the client"
	g++ -o $(BUILD_DIR)client.out client_main.o client.o $(CFLAGS) $(LIBS)

run_server:
	@echo "Start server"
	./bin/server.out

run_client:
	@echo "Start client"
	./bin/client.out
	
clean:
	@echo "** Removing object files ..."
	rm -f *.o