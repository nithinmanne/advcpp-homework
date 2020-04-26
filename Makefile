CXX := g++
CXXFLAGS := -O3 -std=c++17

DIRS := ./homework*

all: 
	$(foreach file, $(wildcard $(DIRS)/*.cpp), $(CXX) $(CXXFLAGS) $(file) -o $(file).exe;)