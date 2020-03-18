CFLAGS = -g -std=c++17 -fpermissive
LIB_DIR = lib/
LIBS= 
SRC_DIR = src/
BUILD_DIR = bin/
FIND_RECUR = $(shell find src -type f -name "*.cpp")

all: app

objects :  
	g++ -c $(SRC_DIR)*.cpp  $(FIND_RECUR) $(CFLAGS)

app: objects
	@echo "** Building the exec"
	g++ -o $(BUILD_DIR)app.out *.o $(CFLAGS) $(LIBS)
	
clean:
	@echo "** Removing object files ..."
	rm -f *.o