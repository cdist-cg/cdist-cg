AM_CFLAGS = -std=c89 -D_FORTIFY_SOURCE=2 -D_POSIX_C_SOURCE=1
AM_CFLAGS += -Wall

if DEBUG
AM_CFLAGS += -O0 -g
else
AM_CFLAGS += -O2
endif

bin_PROGRAMS = cdist-cg
cdist_cg_SOURCES = src/main.c \
	src/commands/banner.c src/commands/config.c src/commands/info.c \
	src/commands/inventory.c
