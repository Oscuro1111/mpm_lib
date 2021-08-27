GCC=cc

LIBS=../lib/lib.a

SHARED_FILES=$(wildcard ../lib/shared/*)

SHARED_LIB=$(SHARED_FILES:../lib/shared/%=./lib/%)

SRCDIR=../src

MPM_LIB_DIR:=../lib

LIBNAME=mpm-server.so.0.1

OBJSDIR=.

FLAGS=-fPIC -c -O2

#utils src build
UTILS_SRC_DIR:=../src/utils

UTILS_SRC=$(wildcard $(UTILS_SRC_DIR)/*.c)

UTILS_OBJS:=$(UTILS_SRC:$(UTILS_SRC_DIR)/%.c=$(OBJSDIR)/%.o)

$(UTILS_OBJS):%.o:$(UTILS_SRC_DIR)/%.c
	 $(GCC) $(FLAGS)  $^ -o $@


#http build
HTTP_SRC_DIR:=../src/http

HTTP_SRCS=$(wildcard $(HTTP_SRC_DIR)/*.c)

HTTP_OBJS:=$(HTTP_SRCS:$(HTTP_SRC_DIR)/%.c=$(OBJSDIR)/%.o)

$(HTTP_OBJS):%.o:$(HTTP_SRC_DIR)/%.c
	 $(GCC) $(FLAGS)  $^ -o $@

#-------------------------------------------------------
SRCSFILES=$(wildcard $(SRCDIR)/*.c)

SRC_OBJS:=$(SRCSFILES:$(SRCDIR)/%.c=$(OBJSDIR)/%.o)

#SRC DIR BUILD
$(SRC_OBJS):%.o:$(SRCDIR)/%.c
	 $(GCC) $(FLAGS)  $^ -o $@

#---------------------------------------------------------------------------------------------------
ALLOBJS:=$(SRC_OBJS) $(HTTP_OBJS) $(UTILS_OBJS)

all:$(ALLOBJS)

ALLSRCS:=$(SRCFILES) $(HTTP_SRCS) $(UTILS_SRC)

obj_build:all
	@echo "::Linking object files"
	$(GCC) -shared -o $(MPM_LIB_DIR)/$(LIBNAME) $(ALLOBJS)