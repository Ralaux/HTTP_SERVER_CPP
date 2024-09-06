# Nom de l'exécutable final
TARGET = bin/serveur_http

# Compilateur et flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
LDFLAGS = -lpthread

# Dossiers
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
TEST_DIR = tests

# Fichiers source
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Règle par défaut
all: $(TARGET)

# Règle pour créer l'exécutable
$(TARGET): $(OBJS)
	@mkdir -p bin
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Règle pour compiler les fichiers objets
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_DIR) -c $< -o $@

# Nettoyer les fichiers générés
clean:
	rm -rf $(BUILD_DIR) bin/

# Règle pour exécuter les tests
test: $(TARGET)
	@cd $(TEST_DIR) && ./run_tests.sh

.PHONY: all clean test
