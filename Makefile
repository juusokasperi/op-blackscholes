NAME = bs_sweep

SRCS =	main.cpp
SRCS_DIR = srcs/

OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))
OBJS_DIR = objs/

DEPS_DIR = deps/
DEPS = $(addprefix $(DEPS_DIR), $(SRCS:.cpp=.d))

BIN_DIR = bin/

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -MMD -MP $(INCS)
#CXXFLAGS += -O2 -DNDEBUG -flto -fomit-frame-pointer -march=native mtune=native

INCS = -I./incs

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	@mkdir -p $(dir $@) $(dir $(DEPS_DIR)$*)
	@$(CXX) $(CXXFLAGS) -c $< -o $@ -MF $(DEPS_DIR)$*.d

all: $(NAME)

$(NAME): $(OBJS)
	@mkdir -p $(BIN_DIR)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(BIN_DIR)$(NAME)
	@echo "Compiled, executable ./$(BIN_DIR)$(NAME)"

-include $(DEPS)

clean:
	@rm -rf $(OBJS_DIR) $(DEPS_DIR)
	@echo "Clean OK."

fclean:
	@rm -rf $(OBJS_DIR) $(DEPS_DIR) $(BIN_DIR)
	@echo "FClean OK."

re: fclean all

.PHONY: all clean fclean re
