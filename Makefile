
NAME = ./ircserv

SRCS_FOLDER = srcs
OBJECTSDIR = objects
HEADERS_FOLDER = headers

FLAGS = -Wall -Wextra -Werror
CPP_STANDARD = -std=c++98

SRC_FILES = Server.cpp \
# User.cpp \
# Channel.cpp \
# Request.cpp \


HEADERS_FILES = $(HEADERS_FOLDER)/User.hpp \
$(HEADERS_FOLDER)/Server.hpp \
$(HEADERS_FOLDER)/Channel.hpp \
$(HEADERS_FOLDER)/Request.hpp \

MAIN = main.cpp

OBJS := $(addprefix $(OBJECTSDIR)/, $(SRC_FILES:.cpp=.o))
MAIN_OBJ = $(addprefix $(OBJECTSDIR)/, $(MAIN:.cpp=.o))

all: $(NAME)

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
