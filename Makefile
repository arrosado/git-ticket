.PHONY: all

LIBPATH += -L"/System/Library/Frameworks/OpenGL.framework/Libraries" 
FRAMEWORK = -framework GLUT
FRAMEWORK += -framework OpenGL
LIBRARIES = -lGL -lGLU -lm -lobjc -lstdc++

OUTPATH = build/
OBJS = util.c cmd.c config.c ticket.c redmine.c 

CC = gcc
CFLAGS = -g -Wall -Ilibgit2/include -Ilibgit2/src -Wextra -Wmissing-prototypes -Wno-missing-field-initializers
LFLAGS = -Llibgit2/build -lgit2 -lz
APPS = git-ticket util-test config-test

all: $(APPS)

% : %.c $(OBJS)
	$(CC) $(OBJS) -o $(OUTPATH)$@ $(CFLAGS) $< $(LFLAGS);
	$(OUTPATH)$@

clean: 
	$(RM) $(OUTPATH)m$(APPS)
	$(RM) -rf $(OUTPATH)*
	$(RM) -rf in.*
