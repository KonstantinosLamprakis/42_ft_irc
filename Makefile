
NAME = ./ircserv

SRCS_FOLDER = srcs
OBJECTSDIR = objects
HEADERS_FOLDER = headers

FLAGS = -Wall -Wextra -Werror -g
DEBUG = -fsanitize=address -static-libsan
CPP_STANDARD = -std=c++17

SRC_FILES = Server.cpp \
Request.cpp \
Exceptions.cpp \
Helper.cpp \
User.cpp \
# Channel.cpp


HEADERS_FILES = $(HEADERS_FOLDER)/User.hpp \
$(HEADERS_FOLDER)/Server.hpp \
$(HEADERS_FOLDER)/Channel.hpp \
$(HEADERS_FOLDER)/Request.hpp \
$(HEADERS_FOLDER)/Exceptions.hpp \
$(HEADERS_FOLDER)/Helper.hpp

MAIN = main.cpp

OBJS := $(addprefix $(OBJECTSDIR)/, $(SRC_FILES:.cpp=.o))
MAIN_OBJ = $(addprefix $(OBJECTSDIR)/, $(MAIN:.cpp=.o))

all: $(NAME)

debug: $(OBJS) $(MAIN_OBJ) $(HEADERS_FILES)
	@c++ $(CPP_STANDARD) $(OBJECTSDIR)/main.o $(OBJS) $(DEBUG) -o $@ -g

$(NAME): $(OBJS) $(MAIN_OBJ) $(HEADERS_FILES)
	@c++ $(CPP_STANDARD) $(OBJECTSDIR)/main.o $(OBJS) -o $@ -g

$(OBJECTSDIR)/%.o: $(SRCS_FOLDER)/%.cpp $(HEADERS_FILES)
	@mkdir -p $(dir $@)
	@c++ $(CPP_STANDARD) $(FLAGS) -o $@ -c $< -g

$(OBJECTSDIR)/%.o: main.cpp $(HEADERS_FILES)
	@c++ $(CPP_STANDARD) $(FLAGS) -o $@ -c $< -g

clean:
	@echo "deleting: " $(OBJECTSDIR)
	@rm -rf $(OBJECTSDIR)

fclean: clean
	@echo "deleting: " $(NAME)
	@rm -rf $(NAME)

re: fclean all
