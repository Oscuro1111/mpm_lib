GCC=gcc
SRCDIR=src
OBJSDIR=objs
BINDIR=bin
FLAGS=-c -O2

BINNAME=mpm-server

SRCSFILES=$(wildcard src/*.c)

SRCS=$(SRCSFILES:src/%.c=%)

OBJS:=$(SRCSFILES:src/%.c=$(OBJSDIR)/%.o)

#obj:$(SRCSFILES)
#	$(foreach var,$(SRCS),$(GCC) $(FLAGS) -o $(OBJSDIR)/$(var).o $(SRCDIR)/$(var).c;)

clean_dep:
	@rm -fr .cache 
	@rm -fr  lib
	@echo "Done removing dependencies"

update_dep:
	@./update_dep.sh

clean:
	@rm -fr $(BINDIR)/*
	@echo "::Binaries in ${BINDIR}/ are cleared."
	@rm $(OBJSDIR)/*.o 
	@echo "::Object  files in ${OBJSDIR} is cleared."
	@echo "cleaning done"


buildobj:$(SRCSFILES)
	@./configure.sh
	@echo "::compiling src files"
	@cd $(OBJSDIR)&&make obj_build&&cd ..
	@echo "::Done"

build:buildobj
	@echo "::Build finished Done"

