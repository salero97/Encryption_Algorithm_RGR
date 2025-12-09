CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -fPIC -O3
LDFLAGS_LIB := -shared
LDFLAGS_MAIN := -ldl

BIN_DIR := bin
LIBS_DIR := libs
OBJ_DIR := obj
SRC_DIR := src
INCLUDE_DIR := include

MAIN_SOURCES := \
	src/main.cpp \
	src/utils.cpp \
	src/funcs.cpp \
	src/vigenere/vigenere_cipher.cpp \
	src/vigenere/vigenere_cipher_interface.cpp \
	src/great/great_cipher.cpp \
	src/great/great_cipher_interface.cpp \
	src/bigram/bigram_cipher.cpp \
	src/bigram/bigram_cipher_interface.cpp

MAIN_OBJECTS := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(MAIN_SOURCES))

VIGENERE_SOURCES := \
	src/vigenere/vigenere_cipher.cpp \
	src/vigenere/vigenere_wrapper.cpp

VIGENERE_OBJECTS := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(VIGENERE_SOURCES))

GREAT_SOURCES := \
	src/great/great_cipher.cpp \
	src/great/great_wrapper.cpp

GREAT_OBJECTS := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(GREAT_SOURCES))

BIGRAM_SOURCES := \
	src/bigram/bigram_cipher.cpp \
	src/bigram/bigram_wrapper.cpp

BIGRAM_OBJECTS := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(BIGRAM_SOURCES))


.PHONY: all clean create_dirs

all: create_dirs $(BIN_DIR)/Encryption_Algorithm_RGR $(LIBS_DIR)/libvigenere.so $(LIBS_DIR)/libgreat.so $(LIBS_DIR)/libbigram.so
	@echo "✓ Сборка завершена!"

create_dirs:
	@mkdir -p $(BIN_DIR) $(LIBS_DIR) $(OBJ_DIR)/vigenere $(OBJ_DIR)/great $(OBJ_DIR)/bigram

$(BIN_DIR)/Encryption_Algorithm_RGR: $(MAIN_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS_MAIN)
	@echo "✓ Главная программа собрана"

$(LIBS_DIR)/libvigenere.so: $(VIGENERE_OBJECTS)
	$(CXX) $(LDFLAGS_LIB) $^ -o $@
	@echo "✓ libvigenere.so создана"

$(LIBS_DIR)/libgreat.so: $(GREAT_OBJECTS)
	$(CXX) $(LDFLAGS_LIB) $^ -o $@
	@echo "✓ libgreat.so создана"

$(LIBS_DIR)/libbigram.so: $(BIGRAM_OBJECTS)
	$(CXX) $(LDFLAGS_LIB) $^ -o $@
	@echo "✓ libbigram.so создана"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIBS_DIR)
	@echo "✓ Очистка завершена"

install: all
	@sudo install -m 755 $(BIN_DIR)/Encryption_Algorithm_RGR /usr/local/bin/
	@echo "✓ Установка завершена"
