Name = Webserv

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror

SRC = src/SERVER_FILES/main.cpp src/SERVER_FILES/server.cpp src/SERVER_FILES/client.cpp src/parsing/REQUEST/request.cpp src/parsing/Parse_config/server.cpp src/parsing/Parse_config/location.cpp

SRC_HEADERS = inc/server.hpp inc/client.hpp inc/request.hpp

OBJ = $(SRC:.cpp=.o)

all : $(Name)

$(Name) : $(OBJ) $(SRC_HEADERS)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(Name)

re : fclean all

.PHONY : all fclean re clean