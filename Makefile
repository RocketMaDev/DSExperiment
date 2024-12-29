SRC_DIR = src
TEST_DIR = test
BUILD_DIR = build
TARGET_LINKED = linked
TARGET_BOOK = book
TARGET_ARRAY = array
TARGET_QS = qs
TARGET_STR = str
TARGET_TREE = tree
TARGET_HUFFMAN = huffman
TARGET_MATRIX = MatrixGraph
TARGET_LINKER = LinkedGraph
TARGET_SEARCH = search
TARGET_MST = mst
TARGETS := $(TARGET_BOOK) $(TARGET_ARRAY) $(TARGET_LINKED) $(TARGET_QS) $(TARGET_STR)
TARGETS += $(TARGET_TREE) $(TARGET_HUFFMAN) $(TARGET_MATRIX) $(TARGET_LINKER)
TARGETS += $(TARGET_SEARCH) $(TARGET_MST)

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

CC = gcc
CFLAGS = -Isrc -Itest -Wall -Wextra
ifdef DEBUG
    CFLAGS += -g -DDEBUG
else
    CFLAGS += -O2
endif

ifndef TARGET
	ERROR = err
else ifeq ($(TARGET), $(TARGET_LINKED))
	DEPS = linkedlist.c listoperation.c
	TEST = testlinked.c
else ifeq ($(TARGET), $(TARGET_BOOK))
	DEPS = linkedlist.c
	TEST = testbook.c
	CFLAGS += -include $(TEST_DIR)/testbook.h -DOVERRIDE_CMP
else ifeq ($(TARGET), $(TARGET_ARRAY))
	DEPS = arraylist.c arrayoperation.c
	TEST = testarray.c
else ifeq ($(TARGET), $(TARGET_QS))
	DEPS = arraylist.c stack.c queue.c qsoperation.c
	TEST = testqs.c
else ifeq ($(TARGET), $(TARGET_STR))
	DEPS = arraylist.c str.c
	TEST = teststr.c
	CFLAGS += -include $(TEST_DIR)/definech.h
else ifeq ($(TARGET), $(TARGET_TREE))
	DEPS = arraylist.c tree.c stack.c
	TEST = testtree.c
else ifeq ($(TARGET), $(TARGET_HUFFMAN))
	DEPS = arraylist.c tree.c stack.c huffman.c
	TEST = testhuffman.c
else ifeq ($(TARGET), $(TARGET_MATRIX))
	DEPS = arraylist.c matrixgraph.c
	TEST = testmatrix.c
	CFLAGS += -include $(TEST_DIR)/definelong.h
else ifeq ($(TARGET), $(TARGET_LINKER))
	DEPS = arraylist.c linkedgraph.c
	TEST = testlinker.c
	CFLAGS += -include $(TEST_DIR)/definelong.h
else ifeq ($(TARGET), $(TARGET_SEARCH))
	DEPS = arraylist.c linkedgraph.c matrixgraph.c graphops.c queue.c
	TEST = testtraverse.c
	CFLAGS += -include $(TEST_DIR)/definelong.h
else ifeq ($(TARGET), $(TARGET_MST))
	DEPS = arraylist.c linkedgraph.c matrixgraph.c graphops.c queue.c
	TEST = testmst.c
	CFLAGS += -include $(TEST_DIR)/definelong.h
else
	ERROR = err
endif
DEP_SRCS =  $(patsubst %.c, $(SRC_DIR)/%.c, $(DEPS))
DEP_SRCS += $(patsubst %.c, $(TEST_DIR)/%.c, $(TEST))
DEP_OBJS =  $(patsubst %.c, $(BUILD_DIR)/%.o, $(DEPS))
DEP_OBJS += $(patsubst %.c, $(BUILD_DIR)/%.o, $(TEST))

all: $(BUILD_DIR) $(ERROR) $(TARGET)

$(BUILD_DIR):
	mkdir -p build

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(DEP_OBJS)
	$(CC) $(DEP_OBJS) -o $@

err:
	@echo 'make' "TARGET=($(TARGETS))"
	@echo 'make clean_deps may needed'
	@exit 1

clean:
	rm -f $(BUILD_DIR)/*.o $(TARGETS)

clean_deps:
	rm -f $(BUILD_DIR)/*.o

.PHONY: all clean clean_deps err

