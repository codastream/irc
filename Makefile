include colors.mk

export PATH := $(HOME)/local/bin:$(PATH)

NAME			:=	irc

CXX				:=	clang++

CXXFLAGS		:=	-Wall -Wextra -Werror -std=c++98

BUILD_TYPE		?=	release
ifeq ($(BUILD_TYPE), debug)
	CXXFLAGS += -g -DDEBUG=1 -O0
else
	CXXFLAGS += -DNDEBUG=1 -O2
endif

MAKEFLAGS		:=	--no-print-directory

SRC_DIR			:=	srcs
INCLUDE_DIR		:=	include
BUILD_DIR		:=	.build

SRCS_FILES		:= 	main.cc\
					commands/CommandFactory.cc\
					commands/ACommand.cc\
					commands/NickCommand.cc\
					commands/PassCommand.cc\
					Client.cc\
					ClientConnection.cc\
					CommandParser.cc\
					IRCException.cc\
					ReplyFactory.cc\
					Server.cc\
					utils.cc\

EXT				:= .cc
# SRCSEXT			:= $(addsuffix $(EXT), $(SRCS_FILES))
SRCS			:= $(addprefix $(SRC_DIR)/, $(SRCS_FILES))

OBJS			:=	${SRCS:$(SRC_DIR)/%.cc=$(BUILD_DIR)/%.o}
INC				:=	-I $(INCLUDE_DIR) -I $(INCLUDE_DIR)/commands
HEADERS			:=	$(wildcard $(INCLUDE_DIR)/*.h)
DIRS			:=	$(sort $(shell dirname $(OBJS)))

FILES_TO_FORMAT	:=	$(SRCS) $(HEADERS)

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

debug: CXXFLAGS += -g -DDEBUG=1
debug: clean all

lint:
	@echo "$(YELLOW)Running static analysis$(NOC)"
	@cppcheck --enable=all -I include -I srcs --std=c++98 $(SRC_DIR)

format:
	@echo "$(YELLOW)Checking code format$(NOC)"
	@clang-format --dry-run --Werror $(FILES_TO_FORMAT)

fclean: clean
	@echo "$(RED)Remove binary$(NOC)"
	@rm -f $(NAME)

re: fclean
	@make

.PHONY: all clean fclean re $(DIRS)
