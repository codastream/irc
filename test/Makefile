include ../colors.mk

export PATH := $(HOME)/local/bin:$(PATH)

NAME			:=	test

CXX				:=	clang++

CXXFLAGS		:=	-Wall -Wextra -Werror -std=c++20

SRC_DIR			:=	srcs
INCLUDE_DIR		:=	include
IRC_INCLUDE_DIR	:=	../include
BUILD_DIR		:=	.build

SRCS_FILES		:= 	test.cc\
					testPassCommand.cc\
					ReplyAssert.cc\
					ServerRunner.cc\

EXT				:= .cc

SRCS			:=	$(addprefix $(SRC_DIR)/, $(SRCS_FILES))

OBJS			:=	${SRCS:$(SRC_DIR)/%.cc=$(BUILD_DIR)/%.o}
INC				:=	-I $(INCLUDE_DIR) -I$(IRC_INCLUDE_DIR) -I $(IRC_INCLUDE_DIR)/commands
HEADERS			:=	$(wildcard $(INCLUDE_DIR)/*.h)
DIRS			:=	$(sort $(shell dirname $(OBJS)))

#================== Loader vars
NB_COMP	:=	1

ifndef RECURSION
TO_COMP :=	$(shell make RECURSION=1 -n | grep Compiling | wc -l)
else
TO_COMP	:=	1
endif

PERCENT	:= 0

#================== Targets

all: $(NAME)

$(DIRS):
	@mkdir -p $@

$(NAME): $(OBJS)
	@echo "\n$(GREEN)Create binaries$(NOC)"
	@$(CXX) $(OBJS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc | $(DIRS)
	@mkdir -p $(BUILD_DIR)
	@if [ $(NB_COMP) -eq 1 ]; then echo "$(BOLD)Compilation of source files :$(NOC)";fi
	$(eval PERCENT=$(shell if [ $(TO_COMP) -eq 0 ]; then echo 100; else expr $(NB_COMP)00 "/" $(TO_COMP); fi))
	@if [ $(PERCENT) -le 30 ]; then echo -n "$(RED)"; elif [ $(PERCENT) -le 66 ]; then echo -n "$(YELLOW)"; elif [ $(PERCENT) -gt 66 ]; then echo -n "$(GREEN)"; fi
	@echo -n "\r"; for i in $$(seq 1 $$(/usr/bin/tput cols)); do echo -n " "; done
	@echo -n "\r"; for i in $$(seq 1 25); do if [ $$(expr $$i "*" 4) -le $(PERCENT) ]; then echo -n "█"; else echo -n " "; fi; done; echo -n "";
	@printf " $(NB_COMP)/$(TO_COMP) - Compiling $<"
	@echo -n "$(NOC)"
	@$(CXX) $(CXXFLAGS) $(INC) $< -c -o $@
	$(eval NB_COMP=$(shell expr $(NB_COMP) + 1))

clean:
	@echo "$(RED)Remove objects$(NOC)"
	@rm -rf $(BUILD_DIR)


fclean: clean
	@echo "$(RED)Remove binary$(NOC)"
	@rm -f $(NAME)

re: fclean
	@make

.PHONY: all clean fclean re $(DIRS)