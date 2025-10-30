NAME = webserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
DFLAGS = -MMD -MP

BUILD_DIR = build/
INC_DIR = include/

LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)libft.a
LIBFT_INC = $(LIBFT_DIR)includes/

SRC = src/main.cpp \
      src/server/Server.cpp \
      src/server/Client.cpp \
      src/server/PollManager.cpp \
      src/http/HttpRequest.cpp \
      src/http/HttpResponse.cpp \
      src/http/MultipartParser.cpp \
      src/http/ErrorPage.cpp \
      src/http/Autoindex.cpp \
      src/cgi/CgiHandler.cpp \
      src/config/ConfigParser.cpp \
      src/config/LocationConfig.cpp \
      src/config/ServerConfig.cpp

OBJ = $(patsubst src/%.cpp, $(BUILD_DIR)%.o, $(SRC))
DEPS = $(OBJ:.o=.d)

all: $(LIBFT) $(NAME)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)server
	@mkdir -p $(BUILD_DIR)http
	@mkdir -p $(BUILD_DIR)cgi
	@mkdir -p $(BUILD_DIR)config

$(BUILD_DIR)%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(DFLAGS) -I$(INC_DIR) -I$(LIBFT_INC) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJ) $(LIBFT)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ) $(LIBFT)

clean:
	rm -rf $(BUILD_DIR)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
