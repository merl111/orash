TARGET_EXEC ?= orash

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src
ifndef ORACLE_HOME
      ORACLE_HOME=/usr
endif
CCLIB=-L$(ORACLE_HOME)/lib -lclntsh

SRCS := $(shell find $(SRC_DIRS) ! -path */demo/* ! -path */proj/* -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS ?= $(INC_FLAGS) -MMD -MP -g

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(CCLIB) $(OBJS) -o $@ $(LDFLAGS)

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) $(CCLIB) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
