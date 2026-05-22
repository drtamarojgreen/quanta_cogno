CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc
LDFLAGS = -pthread

SRC_DIR = src
CORE_DIR = $(SRC_DIR)/core
API_DIR = $(SRC_DIR)/api
APP_DIR = $(SRC_DIR)/app
BUILD_DIR = build

CORE_SRCS = $(wildcard $(CORE_DIR)/*.cpp)
API_SRCS = $(wildcard $(API_DIR)/*.cpp)
APP_SRCS = $(wildcard $(APP_DIR)/*.cpp)
MAIN_SRC = $(SRC_DIR)/main.cpp

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(CORE_SRCS) $(API_SRCS) $(APP_SRCS) $(MAIN_SRC))
TEST_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(CORE_SRCS) $(API_SRCS) $(APP_SRCS) $(SRC_DIR)/unit_tests.cpp $(SRC_DIR)/test_runner_main.cpp)

TARGET = $(BUILD_DIR)/quanta_cogno
TEST_TARGET = $(BUILD_DIR)/test_runner

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(TEST_OBJS) -o $@ $(LDFLAGS)

verify: $(TEST_TARGET)
	@echo "Running verification suite..."
	@./$(TEST_TARGET) > docs/testing_logs.md 2>&1
	@echo "Verification report generated in docs/testing_logs.md"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
