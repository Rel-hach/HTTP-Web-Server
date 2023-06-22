Name = Webserv

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror

SRC = main.cpp Parse_config/parse_config.cpp SERVER_FILES/serv_monitor.cpp REQUEST/request.cpp SERVER_FILES/ListeningSocket.cpp

SRC_HEADERS = INCLUDES/parse_config.hpp INCLUDES/serv_monitor.hpp INCLUDES/ListeningSocket.hpp INCLUDES/request.hpp

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