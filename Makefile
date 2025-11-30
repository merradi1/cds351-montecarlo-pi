CXX      = g++
CXXFLAGS = -O3 -fopenmp

TARGET   = par_mc
SRC      = par_mc.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
