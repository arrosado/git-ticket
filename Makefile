.PHONY: all

LIBPATH += -L"/System/Library/Frameworks/OpenGL.framework/Libraries" 
FRAMEWORK = -framework GLUT
FRAMEWORK += -framework OpenGL
LIBRARIES = -lGL -lGLU -lm -lobjc -lstdc++

ifeq ($(OS), LINUX)
POSTBUILD = ldconfig
else
POSTBUILD = 
endif

PUBLIC = public/
PRIVATE = private/
OUTPATH = build/
OBJS = $(PRIVATE)util.c $(PRIVATE)cmd.c $(PRIVATE)config.c $(PRIVATE)ticket.c $(PRIVATE)redmine.c 

CC = gcc
CFLAGS = -g -Wall -Ilibgit2/include -I$(PUBLIC) -Ilibgit2/src -Wextra -Wmissing-prototypes -Wno-missing-field-initializers
LFLAGS = -Llibgit2/build -lgit2 -lz
APPS = git-ticket util-test config-test

all: $(APPS)
	$(POSTBUILD)

% : $(PRIVATE)%.c $(OBJS)
	$(CC) $(OBJS) -o $(OUTPATH)$@ $(CFLAGS) $< $(LFLAGS);
	$(OUTPATH)$@

clean: 
	$(RM) $(OUTPATH)m$(APPS)
	$(RM) -rf $(OUTPATH)*
	$(RM) -rf in.*
