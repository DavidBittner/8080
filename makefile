C = clang++

SRC = $(wildcard ./src/*.cc)
INC = $(wildcard ./src/*.hh)

OBJS = $(SRC:.cc=.o)
DEPFILES = $(SRC:.cc=.d)

BIN = run.out

FLAGS = -Wall -Werror -g -Iinc/ -I/usr/include/spdlog -std=c++14 -MMD
LFLAGS = -pthread

all: $(OBJS)
	@echo "COMPILER INFO"
	@echo "------------------------------------"
	@$(C) --version
	@$(C) $(FLAGS) $(LFLAGS) $(OBJS) -o $(BIN)
	@echo ""
	@echo "$(BIN) generated successfully."
	@echo "------------------------------------"

-include $(DEPFILES)

%.o: %.cc
	@echo "Building $<"
	@$(C) $(FLAGS) -c $< -o $@
	@echo "Built $< to $@."

clean:
	@rm -f $(OBJS)
	@rm -f $(BIN)
	@rm -f $(DEPFILES)
	@echo Cleaned.

force:
	@touch $(SRC) $(INC)
	@make all
