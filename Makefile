CXX := g++
CXXFLAGS := -I$(BOOST_ROOT) -O3 -std=c++17

all: 
	$$(CXX) $(CXXFLAGS) *.cpp -o advcppfinal.exe