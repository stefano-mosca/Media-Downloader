SHELL           := /bin/sh

TARGET          = media_downloader
BUILD_DIR       = .build
SRC_DIR         ?= src
CC              ?= gcc

CFLAGS_BASE     = -std=c23 -Iinclude

CFLAGS_STRICT   = -Wall -Wextra -Werror -Wpedantic \
                  -Wconversion -Wshadow -Wformat=2 \
                  -Wstrict-prototypes -Wmissing-prototypes \
                  -Wmissing-declarations -Wundef \
                  -Wcast-align -Wwrite-strings \
                  -Wswitch-default -Wimplicit-fallthrough \
                  -Wmisleading-indentation \
                  -Wstrict-aliasing=2

CFLAGS_RELEASE  = -O3 -march=native -flto=auto \
                  -fno-plt -fomit-frame-pointer

CFLAGS_DEBUG    = -O0 -g3 -ggdb3 \
                  -fno-strict-aliasing \
                  -fstack-protector-strong \
                  -fno-omit-frame-pointer \
                  -fsanitize=address,undefined \
                  -DDEBUG

LDLIBS_RELEASE  = -flto
LDLIBS_DEBUG    = -fsanitize=address,undefined

ifeq ($(OS),Windows_NT)
    # Windows (w64devkit)
    EXE_EXT      := .exe
    RUN_CMD      = ./$(TARGET)$(EXE_EXT)

    # My path of w64devkit
    W64DEVKIT    ?= C:/Program Files/C and C++ compilers/w64devkit

    CFLAGS_BASE  += -I"$(W64DEVKIT)/include"
    LDLIBS_BASE  = -L"$(W64DEVKIT)/lib" \
                   -lSDL3_image \
                   -lSDL3_ttf \
                   -lSDL3
else
    # Linux
    EXE_EXT      :=
    RUN_CMD      = ./$(TARGET)

    SDL_PACKAGES = sdl3 sdl3-image sdl3-ttf
    SDL_CFLAGS   = $(shell pkg-config --cflags $(SDL_PACKAGES) 2>/dev/null)
    SDL_LIBS     = $(shell pkg-config --libs   $(SDL_PACKAGES) 2>/dev/null)

    ifeq ($(strip $(SDL_LIBS)),)
        $(error SDL3 not found.
    endif

    CFLAGS_BASE  += $(SDL_CFLAGS)
    LDLIBS_BASE  = $(SDL_LIBS)

    CFLAGS_DEBUG += -fsanitize-address-use-after-scope
endif

MKDIR           = mkdir -p $(BUILD_DIR)
CLEAN           = rm -f $(TARGET)$(EXE_EXT) && rm -rf $(BUILD_DIR)

override CFLAGS := $(CFLAGS_BASE)
override LDLIBS := $(LDLIBS_BASE)

SRCS          = $(wildcard $(SRC_DIR)/*.c)
OBJS          = $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:.c=.o)))
DEPS          = $(OBJS:.o=.d)

$(BUILD_DIR):
	$(MKDIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(TARGET)$(EXE_EXT): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

.PHONY: all clean run rebuild debug release

all: $(TARGET)$(EXE_EXT)

debug: CFLAGS = $(CFLAGS_BASE) $(CFLAGS_STRICT) $(CFLAGS_DEBUG)
debug: LDLIBS = $(LDLIBS_BASE) $(LDLIBS_DEBUG)
debug: clean all

release: CFLAGS = $(CFLAGS_BASE) $(CFLAGS_STRICT) $(CFLAGS_RELEASE)
release: LDLIBS = $(LDLIBS_BASE) $(LDLIBS_RELEASE)
release: clean all

run: $(TARGET)$(EXE_EXT)
	$(RUN_CMD)

clean:
	$(CLEAN)

rebuild: clean all

-include $(DEPS)
