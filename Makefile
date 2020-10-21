CXX       = g++
CXX_STD   = -std=c++17
CXX_W     = -Wall -Wextra -Wpedantic
CXX_DEBUG = -ggdb3 -Og -DDEBUG
CXX_SAN   = -fsanitize=address,leak,undefined
CXX_NOSAN = $(CXX_STD) $(CXX_W) $(CXX_DEBUG)
CXX_FLAGS = $(CXX_NOSAN) $(CXX_SAN)
TARGET    = test_mapreduce
SRC       = $(TARGET).cc
FILES     = mapreduce.cc $(SRC)
RM        = rm -fv

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) -o $(TARGET) $(SRC) $(CXX_FLAGS)

clean:
	$(RM) $(TARGET)

grind: $(SRC)
	$(CC) $(C_NOSAN) -o $(TARGET) $(SRC)
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

warn: $(SRC)
	clang++ $(CXX_FLAGS) -Weverything -Wno-c++98-compat -o $(TARGET) $(SRC)

tidy: $(FILES)
	clang-tidy -extra-arg=$(CXX_STD) $(FILES)

format: $(FILES)
	clang-format -style="{BasedOnStyle: google, IndentWidth: 4}" -i --verbose $(FILES)

lint: $(FILES)
	cpplint --filter=-legal/copyright $(FILES)
