
###########################
# directories
###########################
ROOT = .

SRCDIR = ${ROOT}/src
OUTDIR = ${ROOT}

###########################
# filenames 
###########################
OUTFILE = rtosdemo.out

SRCS += $(shell find ${SRCDIR} -type f -name *.c)
SRCS := $(sort ${SRCS})

###########################
# include 
###########################
VPATH := $(shell find $(SRCDIR) -type d)
INC_PATH = $(addprefix -I,${VPATH})

###########################
# compiler aliases 
###########################
CC = gcc 
COMPILER = ${CC} ${INC_PATH}

###########################
# targets 
###########################
.PHONY: all
all: outfile

.PHONY: outfile
outfile: ${SRCS}
	gcc ${SRCS} -o ${OUTFILE}
