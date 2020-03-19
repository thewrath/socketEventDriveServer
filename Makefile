CFLAGS = -g -std=c++17 -fpermissive -pthread
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
	
clean:
	@echo "** Removing object files ..."
	rm -f *.o