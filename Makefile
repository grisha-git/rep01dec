CPPFLAGS += -Wall -Wextra -std=c++11
SRC = $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=%.o)
paint: $(OBJ)
	$(CXX) $^ -o $@

clean:
	$(RM) -rf $(OBJ) paint