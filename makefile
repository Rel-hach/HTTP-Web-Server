Name = Webserv

CC = c++

CFLAGS			= -Wall -Wextra -Werror -std=c++98
INCLUDES 		= -I./inc/
SRCDIR 			= src
OBJDIR 			= build
SRC 		= $(shell find $(SRCDIR) -name "*.cpp")
OBJ 		= $(SRC:%.cpp=$(OBJDIR)/%.o)

all : $(Name)

$(Name) : $(OBJ) #$(SRC_HEADERS)
	echo "Compiling..."
	$(CC) $^ -o $@

$(OBJDIR)/%.o : %.cpp
	echo "Creating object directory..."
	@mkdir -p $(@D)
	echo "building $@..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $^ -o $@

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(Name)

re : fclean all

.PHONY : all fclean re clean
