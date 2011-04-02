################################################################################
# Arras, a BSD licensed 'big array'
################################################################################

DEBUG := YES

CC := g++

DEBUG_FLAGS   := -Wall -pedantic -Wno-format -g
RELEASE_FLAGS := -Wall -pedantic -Wno-unknown-pramas -Wno-format -O3

ifeq (YES, ${DEBUG})
	CFLAGS	:= ${DEBUG_FLAGS}
else
	CFLAGS	:= ${RELEASE_FLAGS}
endif


################################################################################
# Dependencies
################################################################################
LIBRNG_DIR := ../extern/librng/

################################################################################
# Arras
################################################################################
BUILD_DIR  := build
ARRAS_SRCS := skip.c
ARRAS_OBJS := $(addsuffix .o, $(basename ${ARRAS_SRCS}))

################################################################################
# Tasks
################################################################################
all: LIBRNG ${ARRAS_OBJS}
	${CC} ${ARRAS_SRCS} -c
LIBRNG:
	ln -s ../extern/librng/src librng
	make -C ../extern/librng/
	cp ../extern/librng/librng.so .

TESTS := test.c

test: ${ARRAS_OBJS}
	${CC} -o arras_tests ${CFLAGS} ${TESTS} ${ARRAS_OBJS} librng.so

clean:
	make -C ../extern/librng/ clean
	rm librng
	rm -f ${ARRAS_OBJS}

%.o : ${LIBRNG} %.c
	${CC} -c ${CFLAGS} $< -o $@
