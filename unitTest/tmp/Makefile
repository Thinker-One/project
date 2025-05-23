# 设置变量
PWD  		:= 	$(shell pwd)
TARGET		:= 	main
SRC_DIR		:= 	src
OBJ_DIR		:= 	obj
SRCS		:= 	$(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*.cc $(SRC_DIR)/*.cxx $(SRC_DIR)/*.cpp)
OBJS		:= 	$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)  
OBJS            :=      $(OBJS:$(SRC_DIR)/%.cc=$(OBJ_DIR)/%.o)
OBJS            :=      $(OBJS:$(SRC_DIR)/%.cxx=$(OBJ_DIR)/%.o)
OBJS            :=      $(OBJS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)


# 指定编译器和编译选项
CC		=	gcc
CXX		=	g++
CFLAGS		=	-Wall -Wextra -g -O0
CXXFLAGS	=	-Wall -Wextra -g -O0 -std=c++17

# 动态/静态库
LIB		=	-ludev

# 编译
.PHONY: all
all: printVars clean $(TARGET)


.PHONY: printVars
printVars:
	@echo "\033[0;32m信息参考：\033[0m"
	@echo "          EXEC: ${TARGET}"
	@echo "          PWD: $(PWD)"
	@echo "          SRCS: $(SRCS)"
	@echo "          OBJS: $(OBJS)"
	@echo "          OBJ_DIR: $(OBJ_DIR)"
	@echo "          FILE LIST: $(shell ls)"
	
help:
	@echo "\033[0;32m使用说明：\033[0m"
	@echo "          * 使用此 Makefile 前，先创建 src 目录，并将源码放入 src 目录"
	@echo "          * 编译出的默认可执行文件名为 test，可自行修改 Makefile 中 TARGET 变量"
	@echo "\033[0;32m可用选项：\033[0m"
	@echo "          make                   上下文信息 && 编译"
	@echo "          make all               和 make 等价"
	@echo "          make run               运行目标程序（暂时不支持带参，可自行修改）"
	@echo "          make debug             调试目标程序"
	@echo "          make clean             清理文件"
	@echo "          make help              你现在看到的"


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "\033[0;32mOBJ  (C)：\033[0m" && echo -n "          "
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@echo "\033[0;32mOBJ (CC)：\033[0m" && echo -n "          "
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cxx
	@echo "\033[0;32mOBJ (CC)：\033[0m" && echo -n "          "
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "\033[0;32mOBJ (CC)：\033[0m" && echo -n "          "
	$(CXX) -c $< -o $@ $(CXXFLAGS)

.PHONY: $(TARGET)
$(TARGET): $(OBJS)
	@echo "\033[0;32m目标文件：\033[0m" && echo -n "          "
	$(CXX) $^ -o $@ $(CFLAGS) $(LIB)

	
.PHONY: clean
clean:
	@echo "\033[0;32m文件清理：\033[0m" && echo -n "          "
	rm -f $(OBJ_DIR)/*.o $(TARGET)


.PHONY: run
run:
	./${TARGET}

.PHONY: debug
debug:
	gdb ${TARGET}




# 注解

# wildcard:	wildcard 是一个函数，用于匹配文件名模式并返回符合条件的文件列表。通常，wildcard 函数的语法是：$(wildcard PATTERN) 	\
		其中 PATTERN 是一个文件名模式，可以包含通配符 * 和 ?。SRCS := $(wildcard *.c) 这将会将当前目录中所有以 .c 结尾的文	\
		件添加到 SRCS 变量中。


# $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o):	这行代码是一个模式替换规则，用于将源文件路径转换为目标文件路径。			\
  $(SRCS)：					是源文件路径列表									\
  $(SRC_DIR)/%.c：				是一个模式，表示 $(SRC_DIR) 目录下的所有 .c 文件。					\
  $(OBJ_DIR)/%.o：				是替换的模式，表示目标文件的路径格式，例如 obj/main.o obj/utils.o。

# $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS)))						\
  是先将 SRC_DIR 目录下的 .cpp 文件路径替换为 .o 文件路径，这时 .c 文件路径没变，再讲此替换后的文件路径列表作为参数，将 .c 文件路	\
  径替换																\
  $(patsubst PATTERN, REPLACEMENT, TEXT)												\
  PATTERN：是要匹配的模式，可以包含通配符 %，表示任意字符序列。										\
  REPLACEMENT：是替换模式，用于替换匹配到的模式。											\
  TEXT：是要进行模式替换的文本或者文本列表。


# $(CC) $(CFLAGS) -c $< -o $@:		使用指定的编译器和编译选项，将源文件编译成目标文件。$< 表示输入文件（源文件），$@ 表示输出	\
					文件（目标文件）。										\
  $(CC)：				是一个变量，表示编译器的名称，例如 gcc。							\
  $(CFLAGS)：				是一个变量，表示编译选项，例如 -Wall -Wextra -g。						\
  -c：					是一个编译器选项，表示编译成目标文件而不是可执行文件。						\
  $<：					是一个自动变量，表示规则中的第一个依赖文件，即源文件。						\
  -o $@：				是一个编译器选项，表示输出文件的名称，其中 $@ 是一个自动变量，表示规则中的目标文件。		


# $^: 		是一个自动变量，用于表示规则的所有依赖文件列表。在一个规则中，如果你想要编译多个源文件并链接它们成一个可执行文件，	\
		你可以使用 $^ 来表示所有的源文件。

# =:		使用 = 定义的变量是简单的延迟赋值变量。这意味着，当你在 Makefile 中使用这个变量时，其值会在使用时被展开。如果变量	\
		的值是另一个变量，那么在使用时，会再次展开，这可能导致一些意外的结果。
# :=:		使用 := 定义的变量是立即赋值变量。这意味着，变量的值在定义时就会被展开，并在后续的使用中保持不变。这通常用于需要保	\
		持稳定性的变量，比如命令的路径等。

#		一般来说，如果项目只包含了 C 文件或只包含了 C++ 文件，那么使用相应的编译器变量即可。如果项目中既包含了 C 文件	\
		又包含了 C++ 文件，那么建议使用 $(CXX) 变量，因为它是 C 和 C++ 通用的编译器变量，可以适应各种情况。
