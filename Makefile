NAME = prog1

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++20 -g -Iincludes -MMD -MP

SRCS = srcs/main.cpp \

OBJS_DIR = objs

DEPS = $(OBJS:.o=.d) $(OBJS2:.o=.d)

OBJS =	$(SRCS:%.cpp=$(OBJS_DIR)/%.o)

all : $(NAME) $(NAME2)

$(OBJS_DIR)/%.o:%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean :
	rm -rf $(OBJS_DIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

-include $(DEPS)

.PHONY : all clean fclean re
