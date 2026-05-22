CXX = g++
CXXFLAGS = -std=c++17 -Wall -I../src -I./cpp/util
BIN_DIR = bin

CARDS_SRC = $(wildcard cards/*.cpp)
CARDS_BIN = $(patsubst cards/%.cpp, $(BIN_DIR)/%, $(CARDS_SRC))

all: $(CARDS_BIN)

$(BIN_DIR)/%: cards/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Example run: make run CARD=DiscoveryClass LOGICAL=discover_environment
run: all
	@./$(BIN_DIR)/$(CARD) $(LOGICAL)

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean run
