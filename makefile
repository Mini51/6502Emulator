# Directory for build outputs
BUILD_DIR := build

CXXFLAGS := -std=c++11

# Build targets
all: $(BUILD_DIR)/My6502Executable

# Link the executable
$(BUILD_DIR)/My6502Executable: $(BUILD_DIR)/example.o $(BUILD_DIR)/mos6502.o
	$(CXX) $(CXXFLAGS) $(BUILD_DIR)/example.o $(BUILD_DIR)/mos6502.o -o $(BUILD_DIR)/My6502Executable

# Compile example.cpp to example.o
$(BUILD_DIR)/example.o: examples/example.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c examples/example.cpp -o $(BUILD_DIR)/example.o

# Compile mos6502.cpp to mos6502.o
$(BUILD_DIR)/mos6502.o: src/mos6502.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c src/mos6502.cpp -o $(BUILD_DIR)/mos6502.o

# Clean build files
clean:
	rm -rf $(BUILD_DIR)