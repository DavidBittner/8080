C = clang

SRC = $(wildcard ./src/*.c)
INC = $(wildcard ./src/*.h)

OBJS = $(SRC:.c=.o)

BIN = run.out

FLAGS = -Wall -Werror -g -I./inc/
LFLAGS = 

all: $(OBJS)
	@echo "COMPILER INFO"
	@echo "------------------------------------"
	@$(C) --version
	@$(C) $(LFLAGS) $(OBJS) -o $(BIN)
	@echo ""
	@echo "$(BIN) generated successfully."
	@echo "------------------------------------"

%.o: %.c
	@echo "Compiling $< with dependencies $^..."
	$(C) $(FLAGS) -c $< -o $@
	@echo "Compiled to $@"

clean:
	@rm $(OBJS)
	@rm $(BIN)
	@echo Cleaned.

force:
	@touch $(SRC) $(INC)
	@make all
