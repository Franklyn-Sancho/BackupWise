# Variáveis para o compilador e flags
CXX = g++
CXXFLAGS = -Wall -g
LDFLAGS = -L ../target/release -lrust_lib -lpthread -ldl

# Nome do executável
TARGET = main

# Todos os arquivos .cpp
SRCS = main.cpp event_checker.cpp event_handler.cpp event_selector.cpp config.cpp log.cpp

# Regra padrão
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
