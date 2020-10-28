CXX       = g++
CXX_STD   = -std=c++17
CXX_W     = -Wall -Wextra -Wpedantic
CXX_DEBUG = -ggdb3 -Og -DDEBUG
# you may need to install libs for the sanitizers
CXX_SAN   = -fsanitize=address,leak,undefined

CXX_NOSAN = $(CXX_STD) $(CXX_W) $(CXX_DEBUG) -pthread
CXX_FLAGS = $(CXX_NOSAN) $(CXX_SAN)

TARGET    = mapreduce
TEST      = wc
SRC       = $(TEST).cc $(TARGET).cc

RM        = rm -fv
DIFF      = diff -s
INPUT     = input.txt
OUTPUT    = output.txt
REFERENCE = desired.txt
GRIND     = valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes
WARN      = clang++ -Weverything -Wno-c++98-compat
TIDY      = clang-tidy -extra-arg=$(CXX_STD)
FORMAT    = clang-format -style="{BasedOnStyle: google, IndentWidth: 4}" -i --verbose
LINT      = cpplint --filter=-legal/copyright


all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXX_FLAGS) -o $(TARGET) $^

clean:
	$(RM) $(TEST) $(TARGET)

# the following two targets run the test and detect leaks
sanitize: $(SRC)
	$(CXX) $(CXX_NOSAN) -o $(TEST) $^
	./$(TEST) $(INPUT) > $(OUTPUT)
	sort $(OUTPUT) | $(DIFF) $(REFERENCE) -

grind: $(SRC)
	$(CXX) $(CXX_NOSAN) -o $(TEST) $^
	$(GRIND) ./$(TEST) $(INPUT) > $(OUTPUT)
	sort $(OUTPUT) | $(DIFF) $(REFERENCE) -

# this one gives lots of warnings
warn: $(SRC)
	$(WARN) -o $(TEST) $^

# this one gives even more bonus warnings
tidy: $(SRC)
	$(TIDY) $^

# formats the code for consistent style
format: $(SRC)
	$(FORMAT) $^

# more warnings
lint: $(SRC)
	$(LINT) $^
