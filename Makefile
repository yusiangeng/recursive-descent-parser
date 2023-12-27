CXX = g++
CXXFLAGS = -std=c++17 -Wall
BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src
TEST_PATH := tests
TARGET_NAME := parse
TEST_TARGET_NAME := run-tests
ifeq ($(OS),Windows_NT)
	TARGET_NAME := $(addsuffix .exe,$(TARGET_NAME))
	TEST_TARGET_NAME := $(addsuffix .exe,$(TEST_TARGET_NAME))
endif
TARGET := $(BIN_PATH)/$(TARGET_NAME)
TEST_TARGET := $(BIN_PATH)/$(TEST_TARGET_NAME)
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
SRC_TEST := $(wildcard $(TEST_PATH)/*.cpp)
OBJ_TEST := $(filter-out $(OBJ_PATH)/main.o, $(OBJ)) $(SRC_TEST:$(TEST_PATH)/%.cpp=$(OBJ_PATH)/%.o)

# -------------------------------------

default: makedir $(TARGET) 

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# -------------------------------------

.PHONY: test
test: makedir $(TEST_TARGET)
	ls bin && echo "Running tests..." && $(TEST_TARGET)

$(TEST_TARGET): $(OBJ_TEST)
	$(CXX) $(CXXFLAGS) -o $@ $^

# -------------------------------------

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(OBJ_PATH)/%.o: $(TEST_PATH)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# -------------------------------------

.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH)

.PHONY: clean
clean:
	rm -rvf $(BIN_PATH)/*
	rm -rvf $(OBJ_PATH)/*
