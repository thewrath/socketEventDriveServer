CFLAGS = -g -std=c++17 -fpermissive -pthread -Wall -Werror -ljsoncpp
LIB_DIR = lib/
LIBS= 
SRC_DIR = src/
BUILD_DIR = bin/
FIND_RECUR = $(shell find src -type f -name "*.cpp")

objects:  
	g++ -c $(SRC_DIR)*.cpp $(FIND_RECUR) $(CFLAGS)

build: objects
	@echo "** Building the server"
	g++ -o $(BUILD_DIR)server.out *.o $(CFLAGS) $(LIBS)

run: build
	@echo "Start server"
	./bin/server.out ./bin/configuration.json

doc: 
	doxygen Doxyfile && firefox documentation/html/index.html
	
clean:
	@echo "** Removing object files ..."
	rm -f *.o
