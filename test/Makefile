include ../colors.mk

export PATH := $(HOME)/local/bin:$(PATH)

NAME			:=	test

CXX				:=	clang++

CXXFLAGS		:=	-Wall -Wextra -Werror -std=c++20

MAKEFLAGS		:=	--no-print-directory

IRC_DIR			:=	../srcs
IRC_INCLUDE_DIR	:=	../include

SRC_DIR			:=	srcs
INCLUDE_DIR		:=	include
BUILD_DIR		:=	.build

INC				:= 	-I$(INCLUDE_DIR) -I$(IRC_INCLUDE_DIR)

SRCS_FILES		:= 	test.cc\
					testPassCommand.cc\
					ReplyAssert.cc\
					ServerRunner.cc\

IRC_SRCS_FILES	:=	commands/CommandFactory.cc\
					commands/ACommand.cc\
					commands/NickCommand.cc\
					commands/PassCommand.cc\
					Client.cc\
					ClientConnection.cc\
					CommandParser.cc\
					IRCException.cc\
					Logger.cc\
					ReplyFactory.cc\
					Server.cc\
					utils.cc\

EXT				:= .cc

TEST_SRCS		:=	$(addprefix $(SRC_DIR)/, $(SRCS_FILES))
IRC_SRCS 		:=	$(addprefix $(IRC_DIR)/, $(IRC_SRCS_FILES))
SRCS			:=	$(TEST_SRCS) $(IRC_SRCS)

OBJS			:=	$(patsubst %.cc,$(BUILD_DIR)/%.o,$(notdir $(SRCS)))
DIRS			:=	$(sort $(shell dirname $(OBJS)))

vpath %.cc $(SRC_DIR) $(IRC_DIR) $(IRC_DIR)/commands

#================== Targets

all: $(NAME)

$(DIRS):
	@mkdir -p $@

$(NAME): $(OBJS)
	@echo "\n$(GREEN)Create binaries$(NOC)"
	@$(CXX) $(OBJS) -o $@

$(BUILD_DIR)/%.o: %.cc | $(DIRS)
	@mkdir -p $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) $(INC) $< -c -o $@

debug: CXXFLAGS += -g -DDEB=1
debug: all

clean:
	@echo "$(RED)Remove objects$(NOC)"
	@rm -rf $(BUILD_DIR)


fclean: clean
	@echo "$(RED)Remove binary$(NOC)"
	@rm -f $(NAME)

re: fclean
	@make

.PHONY: all clean fclean re $(DIRS)