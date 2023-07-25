Name = webserv

CC = c++

CFLAGS			= -Wall -Wextra -Werror #-std=c++98
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
#Name = webserv
#
#CXX = c++
#
#CXXFLAGS = -Wall -Wextra -Werror
#
#SRC = src/SERVER_FILES/main.cpp src/SERVER_FILES/server.cpp src/SERVER_FILES/client.cpp src/parsing/REQUEST/request.cpp src/parsing/Parse_config/location.cpp src/parsing/Parse_config/server_data.cpp src/parsing/Parse_config/parse_conf.cpp
#
#SRC_HEADERS = inc/server.hpp inc/client.hpp inc/request.hpp inc/server_data.hpp inc/location.hpp
#
#OBJ = $(SRC:.cpp=.o)
#
#all : $(Name)
#
#$(Name) : $(OBJ) $(SRC_HEADERS)
#	$(CXX) $(CXXFLAGS) $(OBJ) -o $@
#
#clean :
#	rm -f $(OBJ)
#
#fclean : clean
#	rm -f $(Name)
#
#re : fclean all
#
#.PHONY : all fclean re clean

-----------------------------2325150