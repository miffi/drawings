CC ?= gcc
LD = $(CC)

SRCDIR := src
BUILDPREFIX ?= build

SRC := $(wildcard $(SRCDIR)/*.c)
OBJ := $(subst $(SRCDIR), $(BUILDPREFIX), $(SRC:.c=.o))
TARGET ?= mandelbrot

CFLAGS += -std=c11
LDFLAGS += -lGL -lglfw -lm -lcglm
CPPFLAGS += -MD -MP

BUILDTYPE ?= debug

ifeq ($(BUILDTYPE), release)
	CFLAGS += -O2
	CPPFLAGS += -DNDEBUG
else
	CFLAGS += -O0 -g
endif

ifeq ($(CC), clang)
	CFLAGS += -Weverything
else
	CFLAGS += -Wall -Wextra -pedantic
endif

.PHONY: all clean

all: $(BUILDPREFIX) $(TARGET)

-include $(OBJ:.o=.d)

$(TARGET): $(OBJ)
	$(LD) -o $(BUILDPREFIX)/$@ $^ $(LDFLAGS)

$(BUILDPREFIX)/%.o: $(SRCDIR)/%.c
	$(CC) -o $@ $< $(CFLAGS) $(CPPFLAGS) -c

$(BUILDPREFIX):
	mkdir -p $(BUILDPREFIX)

clean:
	$(RM) -r $(BUILDPREFIX)
