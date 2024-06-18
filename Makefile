ifeq (,$(TOOLCHAIN_PREFIX))
$(error TOOLCHAIN_PREFIX is not set)
endif

ifeq (,$(CC))
$(error CC is not set)
endif

ifeq (,$(CXX))
$(error CXX is not set)
endif

ifeq (,$(CFLAGS))
$(error CFLAGS is not set)
endif

ifeq (,$(CXXFLAGS))
$(error CXXFLAGS is not set)
endif

ifeq (,$(LDFLAGS))
$(error LDFLAGS is not set)
endif

####### FLAGS #######
WARNINGS		:= -Wall -Wshadow -Wundef -Wmissing-prototypes -Wno-discarded-qualifiers -Wall -Wextra -Wno-unused-function -Wno-error=strict-prototypes -Wpointer-arith \
					-fno-strict-aliasing -Wno-error=cpp -Wuninitialized -Wmaybe-uninitialized -Wno-unused-parameter -Wno-missing-field-initializers -Wtype-limits -Wsizeof-pointer-memaccess \
					-Wno-format-nonliteral -Wno-cast-qual -Wunreachable-code -Wno-switch-default -Wreturn-type -Wmultichar -Wformat-security -Wno-ignored-qualifiers -Wno-error=pedantic \
					-Wno-sign-compare -Wno-error=missing-prototypes -Wdouble-promotion -Wclobbered -Wdeprecated -Wempty-body -Wtype-limits -Wshift-negative-value -Wstack-usage=2048 \
					-Wno-unused-value -Wno-unused-parameter -Wno-missing-field-initializers -Wuninitialized -Wmaybe-uninitialized -Wall -Wextra -Wno-unused-parameter \
					-Wno-missing-field-initializers -Wtype-limits -Wsizeof-pointer-memaccess -Wno-format-nonliteral -Wpointer-arith -Wno-cast-qual -Wmissing-prototypes \
					-Wunreachable-code -Wno-switch-default -Wreturn-type -Wmultichar -Wno-discarded-qualifiers -Wformat-security -Wno-ignored-qualifiers -Wno-sign-compare

CFLAGS 			+= -I$(SYSROOT)/usr/include
CFLAGS 			+= -D_POSIX_C_SOURCE=199309L
CFLAGS 			+= -DLOG_USE_COLOR
CFLAGS			+= $(WARNINGS)
CFLAGS			+= -Dtimegm=mktime

CXXFLAGS		+= -I$(SYSROOT)/usr/include
CXXFLAGS		+= -std=c++20
CXXGLAGS		+= $(WARNINGS)
CXXFLAGS		+= -Dtimegm=mktime

LDFLAGS         += -L$(SYSROOT)/lib
LDFLAGS 		+= -L$(SYSROOT)/usr/lib
LDFLAGS 		+= -lwiringx

####### DIRS #######
BIN 			= race-computer

BUILD_DIR 		= ./build
BUILD_OBJ_DIR 	= $(BUILD_DIR)/obj
BUILD_BIN_DIR 	= $(BUILD_DIR)/bin

prefix 			?= /usr
bindir 			?= $(prefix)/bin

####### SOURCES #######
ASRCS			= $(shell find ./src -type f -name '*.S')
CSRCS			= $(shell find ./src -type f -name '*.c')
CXXSRCS			= $(shell find ./src -type f -name '*.cpp')

############### minmea ###############
CSRCS += ./minmea/minmea.c
############### minmea ###############

############### LVGL ###############
# Partially copied from lvgl/lvgl.mk: Removed demos and examples compilation
LVGL_PATH = ./lvgl

ASRCS += $(shell find $(LVGL_PATH)/src -type f -name '*.S')
CSRCS += $(shell find $(LVGL_PATH)/src -type f -name '*.c')

CXXEXT := .cpp
CXXSRCS += $(shell find $(LVGL_PATH)/src -type f -name '*${CXXEXT}')

AFLAGS += "-I$(LVGL_PATH)"
CFLAGS += "-I$(LVGL_PATH)"
CXXFLAGS += "-I$(LVGL_PATH)"
############### LVGL ###############

####### OBJECTS #######
AOBJS			= $(ASRCS:.S=.o)
COBJS			= $(CSRCS:.c=.o)
CXXOBJS 		= $(CXXSRCS:.cpp=.o)

OBJS 			= $(AOBJS) $(COBJS) $(CXXOBJS)
TARGET 			= $(addprefix $(BUILD_OBJ_DIR)/, $(patsubst ./%, %, $(OBJS)))

#$(info ASRCS = $(ASRCS))
#$(info CSRCS = $(CSRCS))
#$(info CXXSRCS = $(CXXSRCS))
#$(info AOBJS = $(AOBJS))
#$(info COBJS = $(COBJS))
#$(info CXXOBJS = $(CXXOBJS))
#$(info OBJS = $(OBJS))
#$(info TARGET = $(TARGET))

####### TASKS #######
all: default

$(BUILD_OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC)  $(CFLAGS) -c $< -o $@
	@echo "CC $<"

$(BUILD_OBJ_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	@$(CC) $(AFLAGS) -c $< -o $@
	@echo "AS $<"

$(BUILD_OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "CPP $<"

default: $(TARGET)
	@echo "Linking object files to create the binary..."
	@mkdir -p $(dir $(BUILD_BIN_DIR)/)
	@$(CXX) -o $(BUILD_BIN_DIR)/$(BIN) $(TARGET) $(CXXFLAGS) $(LDFLAGS)
	@echo "Binary created: $(BUILD_BIN_DIR)/$(BIN)"

clean: 
	rm -rf $(BUILD_DIR)