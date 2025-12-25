CPPFLAGS += -Wall -Wextra -std=c++11
SRC_ALL = $(wildcard *.cpp)
SRC = $(filter-out cli.cpp, $(SRC_ALL))
OBJ = $(SRC:%.cpp=%.o)
paint: $(OBJ)
	$(CXX) $^ -o $@

cli: cli.o
	$(CXX) $^ -o $@

clean:
	$(RM) -rf $(OBJ) paint cli cli.o