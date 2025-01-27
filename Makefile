SCANNER = scanner
PARSER  = parser
CC      = g++ #gcc
CFLAGS  = -Iinclude -Wall -std=c++11 -g -MMD #gnu11
LEX     = flex
YACC    = bison
LIBS    = -lfl

EXEC    = $(PARSER)
SRCS    = $(PARSER) $(SCANNER) $(basename $(notdir $(wildcard lib/*.cpp))) ast
DEPS := $(SRCS:=.d)
OBJS := $(SRCS:%=obj/%.o)
OBJDIR  = obj
LIBDIR  = lib

all: $(OBJDIR) $(EXEC)

$(OBJDIR)/$(SCANNER).c: $(OBJDIR)/%.c: %.l
	$(LEX) -o $@ $<

$(OBJDIR)/$(PARSER).c: $(OBJDIR)/%.c: %.y
	$(YACC) -o $@ --defines=$(OBJDIR)/parser.h -v $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJS): $(OBJDIR)

$(OBJDIR)/%.o: $(LIBDIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<


$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

prepare:
	# we have installed the package meet the basic needs
	# you can prepare the extra package you need here, e.g.
	#apt install clang
	#apt install g++

jasmin.jar:
	@which jasmin 2>&1 || (wget https://github.com/davidar/jasmin/raw/master/jasmin.jar)

test: all jasmin.jar
	make test -C test/

bonus: all jasmin.jar
	make test -C bonus/

example: all jasmin.jar
	make test -C example/

pack:
	make clean
	zip -r icd22-hw4.zip . -x ".*" -x "*.zip" 

.PHONY: clean

clean:
	make clean -C test/
	make clean -C example/
	make clean -C bonus/
	rm -rf $(SCANNER) $(SCANNER:=.c) $(PARSER:=.c) $(PARSER:=.h) $(DEPS) $(PARSER:=.output) $(OBJS) $(EXEC) $(OBJDIR)

DOCKERHUB_ACCOUNT=plaslab
IMAGE_NAME = compiler-f20-hw4
DOCKER_IMG = ${DOCKERHUB_ACCOUNT}/${IMAGE_NAME}:latest

pull:
	docker pull ${DOCKER_IMG}
