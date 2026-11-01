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

LDLIBS_BASE     =
LDLIBS_RELEASE  = -flto
LDLIBS_DEBUG    = -fsanitize=address,undefined

ifeq ($(OS),Windows_NT)

    PKG_CONFIG   := $(shell where pkg-config >NUL 2>&1 && echo yes || echo no)
    MKDIR        = if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
    CLEAN        = if exist $(TARGET).exe del /f $(TARGET).exe & if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)

else

    CFLAGS_DEBUG += -fsanitize-address-use-after-scope

    PKG_CONFIG   := $(shell command -v pkg-config >/dev/null 2>&1 && echo yes || echo no)
    MKDIR        = mkdir -p $(BUILD_DIR)
    CLEAN        = $(RM) $(TARGET) && $(RM) -r $(BUILD_DIR)

endif

ifeq ($(PKG_CONFIG),yes)

    SDL_PACKAGES = sdl3 sdl3-image sdl3-ttf

    SDL_CFLAGS = $(shell pkg-config --cflags $(SDL_PACKAGES) 2>/dev/null)
    SDL_LIBS   = $(shell pkg-config --libs $(SDL_PACKAGES) 2>/dev/null)

    ifeq ($(strip $(SDL_LIBS)),)
        $(error SDL3 dependencies not found. Install SDL3, SDL3_image and SDL3_ttf.)
    endif

else

    $(error pkg-config is not available. Please install pkg-config.)

endif

CFLAGS_BASE += $(SDL_CFLAGS)
LDLIBS_BASE += $(SDL_LIBS)

CFLAGS       ?= $(CFLAGS_BASE)
LDLIBS       ?= $(LDLIBS_BASE)

SRCS          = $(wildcard $(SRC_DIR)/*.c)
OBJS          = $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:.c=.o)))
DEPS          = $(OBJS:.o=.d)

$(BUILD_DIR):
	$(MKDIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

.PHONY: all clean run rebuild debug release

all: $(TARGET)

debug: CFLAGS = $(CFLAGS_BASE) $(CFLAGS_STRICT) $(CFLAGS_DEBUG)
debug: LDLIBS = $(LDLIBS_BASE) $(LDLIBS_DEBUG)
debug: clean all

release: CFLAGS = $(CFLAGS_BASE) $(CFLAGS_STRICT) $(CFLAGS_RELEASE)
release: LDLIBS = $(LDLIBS_BASE) $(LDLIBS_RELEASE)
release: clean all

run: $(TARGET)
	./$(TARGET)

clean:
	$(CLEAN)

rebuild: clean all

-include $(DEPS)
