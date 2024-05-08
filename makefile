CXX := g++

SRC_PATH := src
INCLUDE_PATH := src/include
BUILD_PATH := build 


SOURCE_FILES := $(wildcard $(SRC_PATH)/*.cpp) $(wildcard $(INCLUDE_PATH)/*.cpp)

TARGET := build



workflow:
	@echo "compiling for linux"
	@$(CXX) -o ${TARGET}/mos6502 $(SOURCE_FILES)
	@echo "compiling for windows"
	@$(CXX) -o ${TARGET}/mos6502.exe $(SOURCE_FILES)
