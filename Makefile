CFLAGS = -g -std=c++17 -fpermissive -pthread -lprotobuf -Wall -Werror -ljsoncpp
LIB_DIR = lib/
LIBS= 
SRC_DIR = src/
BUILD_DIR = bin/
FIND_RECUR = $(shell find src -type f -name "*.cpp")

PROTO_SRC_DIR = src/protobuffer/message_definitions/
PROTO_DST_DIR = src/protobuffer/build/

objects:  
	g++ -c $(SRC_DIR)*.cpp $(PROTO_DST_DIR)*.pb.cc $(FIND_RECUR) $(CFLAGS)

protobuffers:
	protoc -I=$(PROTO_SRC_DIR) --cpp_out=$(PROTO_DST_DIR) $(PROTO_SRC_DIR)/base.proto

server: objects
	@echo "** Building the server"
	g++ -o $(BUILD_DIR)server.out *.o $(CFLAGS) $(LIBS)

run_server:
	@echo "Start server"
	./bin/server.out
	
clean:
	@echo "** Removing object files ..."
	rm -f *.o