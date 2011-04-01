################################################################################
# Arras, a BSD licensed 'big array'
################################################################################

DEBUG := YES

CC := g++

DEBUG_FLAGS	:=  -Wall -Wno-format -g -DDEBUG
RELEASE_FLAGS	:=	-Wall -Wno-unknown-pramas -Wno-format -O3

ifeq (YES, ${DEBUG})
	CFLAGS	:= ${DEBUG_FLAGS}
else
	CFLAGS	:= ${RELEASE_FLAGS}
endif

DEPENDENCIES := ${LIBRNG}

BUILD_DIR  := build
ARRAS_SRCS := skip.c
ARRAS_OBJS := $(addsuffix .o, $(basename ${ARRAS_SRCS}))

TESTS := test.c

test: ${ARRAS_OBJS}
	${CC} ${CFLAGS} ${TESTS} ${ARRAS_OBJS}

clean:
	-rm -f ${ARRAS_OBJS}

%.o : %.c
	${CC} -c ${CFLAGS} $< -o $@
