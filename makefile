# Definições do compilador e opções de compilação

# Compilador C++
CXX = g++

# Opções de compilação para debug
CXXFLAGS_DEBUG = -std=c++20 -Wall -Wextra -g -O0 -I lib

# Opções de compilação para release
CXXFLAGS_RELEASE = -std=c++20 -Wall -Wextra -O3 -DNDEBUG -I lib

# Modo de compilação (debug ou release)
MODE ?= debug

# Altere para "release" para compilar em modo release
ifeq ($(MODE), release)
	CXXFLAGS = $(CXXFLAGS_RELEASE)
else
	CXXFLAGS = $(CXXFLAGS_DEBUG)
endif

# Diretórios para código-fonte, objetos e saída
SRC_DIRS = src
OBJ_DIR = objects
OUTPUT_DIR = bin
TESTS_DIR = tests

# Definir o nome do executável
OUTPUT_NAME = $(notdir $(CURDIR))
OUTPUT = $(OUTPUT_DIR)/$(OUTPUT_NAME)$(EXT)

# Variáveis para comandos e sistema operacional
ifeq ($(OS),Windows_NT)
	RM = rmdir /q /s
	MKDIR = mkdir
	FIXPATH = $(subst /,\,$1)
	EXT = .exe
	Cleanup = cls
	CommandCreate = if not exist $@ ($(MKDIR) $@)
	ExecuteTest = g++ $(CXXFLAGS) $(TESTS_DIR)/TestMatriz.cpp objects/matriz/Matriz.o $(INCLUDES) -o $(TESTS_DIR)/TestMatriz$(EXT)

else
	RM = rm -f
	MKDIR = mkdir -p
	FIXPATH = $1
	EXT = .run
	Cleanup = clear
	CommandCreate = if [ ! -d "$@" ]; then mkdir "$@"; fi
	ExecuteTest = g++ $(CXXFLAGS) $(TESTS_DIR)/TestMatriz.cpp objects/matriz/Matriz.o $(INCLUDES) -o $(TESTS_DIR)/TestMatriz$(EXT)
endif

# Encontrar todos os arquivos .cpp nos diretórios de src, /**/* sub
SOURCES = $(wildcard $(SRC_DIRS)/**/*.cpp)

# Criar uma lista de arquivos objeto a partir dos arquivos de origem
OBJECTS = $(patsubst $(SRC_DIRS)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Variável para arquivos de dependência
DEPS = $(OBJECTS:.o=.d)

# Diretórios de inclusão
INCLUDE_DIRS = include
INCLUDES = $(patsubst %,-I%, $(INCLUDE_DIRS))

# Diretórios de biblioteca (opcional)
LIB_DIRS = lib
LIBS = $(patsubst %,-L%, $(LIB_DIRS))

# Regras principais
all: $(OUTPUT)

# Criar diretórios se não existirem
$(OUTPUT_DIR) $(OBJ_DIR):
	@$(CommandCreate)

# Compilar arquivos .cpp em arquivos .o
$(OBJ_DIR)/%.o: $(SRC_DIRS)/%.cpp | $(OBJ_DIR)
	@mkdir "$(dir $@)" 2>nul || echo ...
	@echo "Compilando $<..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(@F): $<" > $(@:.o=.d)

# Criar o executável
$(OUTPUT): $(OBJECTS) | $(OUTPUT_DIR)
	@echo "Compilando o executavel $@ com os arquivos: $^"
	@$(CXX) -o $@ $^ $(LIBS)
	@echo "Compilacao concluida com sucesso!"

# Inclui arquivos de dependência
-include $(DEPS)

# Regra para limpeza dos diretórios de saída
clean:
	@echo "Deletando arquivos objeto..."
	@if exist "$(OBJ_DIR)\*.o" (del "$(OBJ_DIR)\*.o")
	
	@echo "Deletando arquivos de dependencia..."
	@if exist "$(OBJ_DIR)\*.d" (del "$(OBJ_DIR)\*.d")
	
	@echo "Deletando a pasta $(OUTPUT_DIR)..."
	@if exist "$(OUTPUT_DIR)" ($(RM) "$(OUTPUT_DIR)")
	
	@echo "Deletando a pasta $(OBJ_DIR)..."
	@if exist "$(OBJ_DIR)" ($(RM) "$(OBJ_DIR)")

	@echo "Limpeza concluida com sucesso!"

# Regra para executar o programa
run: all
	@$(Cleanup)
	@echo "Executando o programa..."
	@$(OUTPUT)

# Regra para gerar documentação
docs:
	@$(Cleanup)
	@echo "Deletando documentacao antiga..."
	@if exist "docs" ($(RM) "docs") || if exist "docs" then ($(RM) "docs")
	@echo "Documentacao antiga deletada com sucesso!"
	
	@echo "Gerando documentacao..."
	@doxygen Doxyfile
	@echo "Documentacao gerada com sucesso!"

# Regra para executar testes
test: all
	@echo "Executando os testes..."
	@$(ExecuteTest)
	@$(TESTS_DIR)/TestMatriz$(EXT)

## Regra para inicializar a estrutura de diretórios
init:
	@mkdir "$(SRC_DIRS)/Main" "$(INCLUDE_DIRS)" "$(LIB_DIRS)" "$(TESTS_DIR)/log" 2>nul || echo "Alguns diretorios ja existem."
	@$(Cleanup)

.PHONY: all clean run test docs init