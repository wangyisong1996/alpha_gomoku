SOURCES=$(wildcard *.cpp)
HEADERS=$(wildcard *.h)
OBJECTS=$(SOURCES:%.cpp=%.o)
TARGET=main.exe

CPPFLAGS=-O2 -std=c++11
LDFLAGS=-lglfw3 -lopengl32 -lgdi32

all: $(TARGET)
$(TARGET): $(OBJECTS) $(HEADERS)
	@echo Now Generating $(TARGET) ...
	g++ $(OBJECTS) $(LDFLAGS) -o $(TARGET)

%.o: %.cpp $(HEADERS)
	@echo Now Compiling $< ...
	g++ $(CPPFLAGS) -c $< -o $@

clean:
	rm -f *.o *.exe

run: $(TARGET)
	./$(TARGET)

