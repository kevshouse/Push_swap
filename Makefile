# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kevin-anderson <kevin-anderson@student.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/14 19:14:32 by kevin-ander       #+#    #+#              #
#    Updated: 2025/05/14 21:00:31 by kevin-ander      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ========================
# 1. COMPILER CONFIGURATION
# ========================
CC          := cc
CFLAGS      := -Wall -Wextra
DEBUG_FLAG  ?= -DDEBUG=0  # Default to debug off
INCLUDES    := -I./includes -I./tests/test_utils
LDFLAGS     := -fprofile-arcs -ftest-coverage

# ========================
# 2. LIBRARIES & TOOLS
# ========================
CRITERION   := -lcriterion
COV_FLAGS   := -fprofile-arcs -ftest-coverage
GCOV        := gcov
LCOV        := lcov
GENHTML     := genhtml
LCOV_IGNORE := --ignore-errors empty,inconsistent,source
GENHTML_IGNORE := --ignore-errors source

# ========================
# 3. PROJECT STRUCTURE
# ========================
NAME        := push_swap
TEST_NAME   := test_stack
SRC_DIR     := src
TEST_DIR    := tests
OBJ_DIR     := obj
COV_DIR     := coverage

# ========================
# 4. SOURCE FILES
# ========================
CORE_FILES  := p_s_machine.c operations.c utils.c
MAIN_FILE   := push_swap.c
TEST_FILES  := test_stack_ops.c test_utils/test_helpers.c

# ========================
# 5. GENERATED PATHS
# ========================
SRCS        := $(addprefix $(SRC_DIR)/,$(CORE_FILES) $(MAIN_FILE))
OBJS        := $(addprefix $(OBJ_DIR)/,$(CORE_FILES:.c=.o) $(MAIN_FILE:.c=.o))
TEST_SRCS   := $(addprefix $(TEST_DIR)/,$(TEST_FILES))
TEST_OBJS   := $(addprefix $(OBJ_DIR)/,$(TEST_FILES:.c=.o))

# ========================
# 6. OUTPUT COLORS
# ========================
GREEN       := \033[0;32m
RED         := \033[0;31m
YELLOW      := \033[0;33m
NC          := \033[0m

# ========================
# 7. MAIN BUILD RULES
# ========================
all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(YELLOW)Linking $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) $(DEBUG) $(OBJS) -o $(NAME) $(LDFLAGS)
	@echo "$(GREEN)$(NAME) compiled successfully!$(NC)"

# ========================
# 8. COMPILATION RULES
# ========================
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(@D)
	@echo "$(YELLOW)Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) $(DEBUG_FLAG) $(INCLUDES) -c $< -o $@ $(COV_FLAGS)

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(@D)
	@echo "$(YELLOW)Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) $(DEBUG_FLAG) $(INCLUDES) -c $< -o $@ $(COV_FLAGS)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# ========================
# 9. TESTING RULES
# ========================
test: $(TEST_NAME)
	@./$(TEST_NAME)

$(TEST_NAME): $(filter-out $(OBJ_DIR)/$(MAIN_FILE:.c=.o),$(OBJS)) $(TEST_OBJS)
	@echo "$(YELLOW)Building tests...$(NC)"
	@$(CC) $(CFLAGS) $(DEBUG) $(INCLUDES) $^ -o $(TEST_NAME) $(CRITERION) $(COV_FLAGS)
	@echo "$(GREEN)Tests compiled successfully!$(NC)"

# ========================
# 10. COVERAGE REPORT
# ========================
coverage: test
	@echo "$(YELLOW)Generating coverage report...$(NC)"
	@mkdir -p $(COV_DIR)
	@# First reset counters
	@$(LCOV) --zerocounters --directory $(OBJ_DIR)
	@# Run tests to generate fresh coverage data
	@./$(TEST_NAME) > /dev/null 2>&1
	@# Capture coverage with all necessary ignores
	@if ! $(LCOV) --capture \
		--directory $(OBJ_DIR) \
		--output-file $(COV_DIR)/coverage.info \
		--no-external \
		--exclude '*/tests/*' \
		--exclude '*/usr/include/*' \
		--ignore-errors unused \
		--ignore-errors empty \
		--ignore-errors inconsistent; then \
		echo "$(YELLOW)Coverage generation completed with warnings$(NC)"; \
	fi
	@# Generate HTML report
	@if ! $(GENHTML) $(COV_DIR)/coverage.info \
		--output-directory $(COV_DIR) \
		--ignore-errors source; then \
		echo "$(YELLOW)HTML generation completed with warnings$(NC)"; \
	fi
	@echo "$(GREEN)Coverage report generated in $(COV_DIR)/$(NC)"
	@xdg-open $(COV_DIR)/index.html 2>/dev/null || \
	 open $(COV_DIR)/index.html 2>/dev/null || \
	 echo "Open $(COV_DIR)/index.html in your browser"

# ========================
# 11. CLEANING RULES
# ========================
clean:
	@find . -name "*.gcda" -delete
	@find . -name "*.gcno" -delete
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)Object files and coverage data removed$(NC)"

fclean: clean
	@rm -f $(NAME) $(TEST_NAME)
	@rm -rf $(COV_DIR)
	@echo "$(RED)Executables and coverage reports removed$(NC)"

re: fclean all

.PHONY: all clean fclean re test coverage
