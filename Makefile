# --- Makefile (simple version) ---

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Iinclude

SRC = src/apps/simulate.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = simulate

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)
