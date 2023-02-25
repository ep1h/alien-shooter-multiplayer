# SETTINGS #####################################################################
APP_NAME := asmp
BUILD_DIR := build
SRC_DIRS := src
TEST_SRC_DIRS := test/
CC:=i686-w64-mingw32-gcc
CXX:=i686-w64-mingw32-g++
LINKER := $(CXX)
STATIC_LIBS := -lSynchronization
# RELEASE
CC_CMP_FLAGS_RELEASE := -O3 -Wall -Wextra -Werror
CXX_CMP_FLAGS_RELEASE := $(CC_CMP_FLAGS_RELEASE)
LINK_FLAGS_RELEASE := -static -shared $(STATIC_LIBS)
# DEBUG
CC_CMP_FLAGS_DEBUG := -O0 -g -D DEBUG_
CXX_CMP_FLAGS_DEBUG := $(CC_CMP_FLAGS_DEBUG)
LINK_FLAGS_DEBUG := $(LINK_FLAGS_RELEASE)

# IMPLEMENTATION ###############################################################
.DEFAULT_GOAL := help
MKDIR_P := mkdir -p
RM_R := rm -fr

CFG ?= release
ifeq ($(CFG),debug)
    CC_CMP_FLAGS = $(CC_CMP_FLAGS_DEBUG)
    CXX_CMP_FLAGS = $(CXX_CMP_FLAGS_DEBUG)
    LINK_FLAGS := $(LINK_FLAGS_DEBUG)
    APP_NAME := $(APP_NAME)_dbg.dll
    _BUILD_DIR := debug
else
    CC_CMP_FLAGS = $(CC_CMP_FLAGS_RELEASE)
    CXX_CMP_FLAGS = $(CXX_CMP_FLAGS_RELEASE)
    LINK_FLAGS := $(LINK_FLAGS_RELEASE)
    APP_NAME := $(APP_NAME).dll
    _BUILD_DIR := release
endif
ifneq (,$(findstring build_tests,$(MAKECMDGOALS)))
    $(eval LINK_FLAGS := $(subst -shared,,$(LINK_FLAGS)))
    CC_CMP_FLAGS +=-D EZTEST -D FILENAME=$(basename $(notdir $<))
    CXX_CMP_FLAGS +=-D EZTEST -D FILENAME=$(basename $(notdir $<))
    LINK_FLAGS += -Wl,--entry=_$(ENTRY)
    SRC_DIRS += $(TEST_SRC_DIRS)
    _BUILD_DIR := test_$(_BUILD_DIR)
    APP_NAME := test_$(APP_NAME)_$(ENTRY).exe
endif

OBJ_DIR := $(BUILD_DIR)/$(_BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)
rwc=$(sort $(wildcard $1$2)) $(foreach d,$(wildcard $1*),$(call rwc,$d/,$2))

C_FILES:=$(foreach d,$(SRC_DIRS),$(call rwc,$(d),*.c))
CPP_FILES:=$(foreach d,$(SRC_DIRS),$(call rwc,$(d),*.cpp))
OBJ_FILES = $(C_FILES:%=$(OBJ_DIR)/%.o) $(CPP_FILES:%=$(OBJ_DIR)/%.o)
H_FILES:=$(foreach d,$(SRC_DIRS),$(call rwc,$(d),*.h))
HPP_FILES:=$(foreach d,$(SRC_DIRS),$(call rwc,$(d),*.hpp))
H_DIRS:=$(sort $(dir $(H_FILES)))
HPP_DIRS:=$(sort $(dir $(HPP_FILES)))
H_INCLUDES:=$(addprefix -I, $(H_DIRS))
HPP_INCLUDES:=$(addprefix -I, $(HPP_DIRS))
INCLUDES:=$(addprefix -I, $(SRC_DIRS)) $(H_INCLUDES) $(HPP_INCLUDES)

# compile .c files
$(OBJ_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) -c $(CC_CMP_FLAGS) -o $@ $(INCLUDES) $<

# compile .cpp files
$(OBJ_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) -c $(CXX_CMP_FLAGS) -o $@ $(INCLUDES) $<

# # link
$(BIN_DIR)/$(APP_NAME): $(OBJ_FILES)
	$(MKDIR_P) $(dir $@)
	$(LINKER) $^ $(LINK_FLAGS) -o $@

build: $(BIN_DIR)/$(APP_NAME)
build_tests: $(BIN_DIR)/$(APP_NAME)
clean:
	$(RM_R) $(BUILD_DIR)
.PHONY: help
help:
	@echo "Usage: make [TARGET] [VARIABLES]"
	@echo ""
	@echo "Targets:"
	@echo "  build       - build the project"
	@echo "  build_tests - build tests"
	@echo "  clean       - remove $(BUILD_DIR)"
	@echo "  help        - display this help message (default)"
	@echo ""
	@echo "Variables:"
	@echo "  CFG         - build configuration(release(default)/debug)"
	@echo "  ENTRY       - entry point for tests (ignored for non-test builds)"
	@echo ""
	@echo "Examples:"
	@echo ""
	@echo "  make                   - display this help message"
	@echo "  make build             - build the project"
	@echo "  make build CFG=debug   - build the project without optimizations"
	@echo "                           and include debugging information"
	@echo "  make build_tests ENTRY=memory_tests - build tests for the project."
	@echo "    Function 'memory_tests' (which calls all tests) must exist"
