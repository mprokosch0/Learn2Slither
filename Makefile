NAME = Learn2Slither

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++20 -g -Iincludes -Iminilibx-linux -MMD -MP

MLX_PATH = ./minilibx-linux
MLX = $(MLX_PATH)/libmlx.a

MLXFLAGS = -lmlx -L$(MLX_PATH) -lXext -lX11 -lm 

SRCS =	srcs/main.cpp \
		srcs/classes/Engine.cpp \
		srcs/classes/Image.cpp \
		srcs/classes/Text.cpp \
		srcs/classes/Rectangle.cpp \
		srcs/classes/Button.cpp \
		srcs/classes/screens/IScreen.cpp \
		srcs/classes/screens/TitleScreen.cpp \
		srcs/classes/screens/SelectScreen.cpp \
		srcs/classes/game/Snake.cpp \
		srcs/classes/game/Board.cpp \
		srcs/classes/agent/Agent.cpp \
		srcs/classes/agent/Interpreter.cpp

OBJS_DIR = objs

DEPS = $(OBJS:.o=.d)

OBJS =	$(SRCS:%.cpp=$(OBJS_DIR)/%.o)

all : $(NAME)

$(OBJS_DIR)/%.o:%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME) : $(OBJS) $(MLX)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS) $(MLXFLAGS)

$(MLX):
	@make -sC $(MLX_PATH)

clean :
	@make -sC $(MLX_PATH) clean
	rm -rf $(OBJS_DIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

-include $(DEPS)

.PHONY : all clean fclean re
