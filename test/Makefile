NAME = Qalgo

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++20 -g -Iincludes -MMD -MP

SRCS =	srcs/main.cpp \
		srcs/Board.cpp \
		srcs/Snake.cpp \
		srcs/Interpreter.cpp \
		srcs/Agent.cpp
		
OBJS_DIR = objs

DEPS = $(OBJS:.o=.d)

OBJS =	$(SRCS:%.cpp=$(OBJS_DIR)/%.o)

all : $(NAME)

$(OBJS_DIR)/%.o:%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME) : $(OBJS) $(MLX)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS) $(MLXFLAGS)

clean :
	rm -rf $(OBJS_DIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

-include $(DEPS)

.PHONY : all clean fclean re
