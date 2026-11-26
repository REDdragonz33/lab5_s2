CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic
SOURCES = main.cpp world.cpp perlin.cpp
TARGET = lr5_app

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)
