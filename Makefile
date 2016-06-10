# SOURCES:=$(wildcard *.cpp)
SOURCES:=main.cpp Constants.cpp Graphics.cpp RNG.cpp State.cpp \
			UCT.cpp Node.cpp BasicAI.cpp NormalAI.cpp math.cpp
HEADERS:=$(wildcard *.h)
OBJECTS:=$(SOURCES:%.cpp=%.o)
CXX:=g++
TARGET:=main.exe
USE_CNN:=0
DEBUG:=0

CPPFLAGS=-O2 -std=c++11

uname:=$(shell uname)
ifeq ($(uname), Linux)
	LDFLAGS=-lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -ldl
else
	LDFLAGS=-lglfw3 -lopengl32 -lgdi32
endif

ifeq ($(USE_CNN), 1)
	LDFLAGS+=-lboost_system -L ~/caffe/distribute/lib -lcaffe -lglog
	CPPFLAGS+=-I ~/caffe/distribute/include
	SOURCES+=dcnn.cpp
	OBJECTS+=dcnn.o
endif

ifeq ($(DEBUG), 1)
	SOURCES+=RandomAI.cpp
	OBJECTS+=RandomAI.o
endif

all: $(TARGET)
$(TARGET): $(OBJECTS) $(HEADERS)
	@echo Now Generating $(TARGET) ...
	g++ $(OBJECTS) $(LDFLAGS) -o $(TARGET)

main.o: main.cpp
	@echo Now Compiling $< ...
	$(CXX) $^ $(CPPFLAGS) -c -o $@

Constants.o: Constants.cpp
	@echo Now Compiling $< ...
	$(CXX) $^ $(CPPFLAGS) -c -o $@

Graphics.o: Graphics.cpp
	@echo Now Compiling $< ...
	$(CXX) $^ $(CPPFLAGS) -c -o $@

RNG.o: RNG.cpp
	@echo Now Compiling $< ...
	$(CXX) $^ $(CPPFLAGS) -c -o $@

State.o: State.cpp
	@echo Now Compiling $< ...
	$(CXX) $^ $(CPPFLAGS) -c -o $@

UCT.o: UCT.cpp
	@echo Now Compiling $< ...
	$(CXX) $^ $(CPPFLAGS) -c -o $@

Node.o: Node.cpp
	@echo Now Compiling $< ...
	$(CXX) $^ $(CPPFLAGS) -c -o $@

BasicAI.o: BasicAI.cpp
	@echo Now Compiling $< ...
	$(CXX) $^ $(CPPFLAGS) -c -o $@

NormalAI.o: NormalAI.cpp
	@echo Now Compiling $< ...
	$(CXX) $^ $(CPPFLAGS) -c -o $@

RandomAI.o: RandomAI.cpp
	@echo Now Compiling $< ...
	$(CXX) $^ $(CPPFLAGS) -c -o $@

math.o: math.cpp
	@echo Now Compiling $< ...
	$(CXX) $^ $(CPPFLAGS) -c -o $@

dcnn.o: dcnn.cpp
	@echo Now Compiling $< ...
	$(CXX) $^ $(CPPFLAGS) -c -o $@

clean:
	rm -f *.o *.exe

run: $(TARGET)
	./$(TARGET)