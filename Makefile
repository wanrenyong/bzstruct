ifdef TEST
TARGET_APP ?= demo
else
TARGET_LIB ?= libbzstruct.a
endif


TOP_DIR = $(shell pwd)

#external definition
CJSON_INC_DIR ?= $(TOP_DIR)/thirdparty

#BUILD_THIRDPARTY flag
#BUILD_THIRDPARTY +=

#Extra flags
LOCAL_C_FLAGS += $(EXTRA_C_FLAGS)
LOCAL_LD_FLAGS += $(EXTRA_LD_FLAGS)
LOCAL_LD_LIBS += $(EXTRA_LD_LIBS)

#default is $PWD/build
ifdef BUILD_DIR
LOCAL_BUILD_DIR := $(BUILD_DIR)
endif

#default is $LOCAL_BUILD_DIR/obj
ifdef BUILD_OBJ_DIR
LOCAL_BUILD_OBJ_DIR := $(BUILD_OBJ_DIR)
endif

#default is $LOCAL_BUILD_DIR/lib
ifdef INSTALL_LIB_DIR
LOCAL_INSTALL_LIB_DIR := $(INSTALL_LIB_DIR)
endif

#default is $LOCAL_BUILD_DIR/bin
ifdef INSTALL_BIN_DIR
LOCAL_INSTALL_BIN_DIR := $(INSTALL_BIN_DIR)
endif


ifdef TEST
LOCAL_C_FLAGS += -g
BUILD_THIRDPARTY ?= yes
LOCAL_C_FLAGS += -Ddemo_main=main
endif

# it's not neccessary to define LOCAL_SRC_FILES, default is $PWD/*.c
#LOCAL_SRC_FILES += 

ifdef BUILD_THIRDPARTY
LOCAL_EXTRA_SRC_FILES += $(wildcard thirdparty/*.c)
endif

LOCAL_C_FLAGS += -I$(CJSON_INC_DIR)

include build.mk

help:
	@echo "Usage: make [Options]"
	@echo "  Options:"
	@echo "     TEST=<1>                 - Build a demo program"
	@echo "     BUILD_DIR=<build_dir>    - Define Build dir, default is ./build"
	@echo "     EXTRA_C_FLAGS=<flag>     - Define extra c compling flags"
	@echo "     INSTALL_LIB_DIR=<dir>    - Define where lib will be installed, default is BUILD_DIR/lib"
	@echo "     clean                    - Clean BUILD_DIR"
