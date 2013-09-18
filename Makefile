LIBPATH += -L"/System/Library/Frameworks/OpenGL.framework/Libraries" 

FRAMEWORK = -framework GLUT
FRAMEWORK += -framework OpenGL

OUTPATH = build/
OBJS = util.c cmd.c config.c ticket.c redmine.c 
COMPILERFLAGS = -Wall
CC = g++
CFLAGS = -g -Wall -Ilibgit2/include -Ilibgit2/src -Wextra -Wmissing-prototypes -Wno-missing-field-initializers
LFLAGS = -Llibgit2/build -lgit2 -lz
LIBRARIES = -lGL -lGLU -lm -lobjc -lstdc++
APPS = gitticket util-test config-test

all: $(APPS)

% : %.c $(OBJS)
	$(CC) $(OBJS) -o $(OUTPATH)$@ $(CFLAGS) $< $(LFLAGS);
	$(OUTPATH)$@

clean: 
	$(RM) $(OUTPATH)m$(APPS)
	$(RM) -rf $(OUTPATH)*
	$(RM) -rf in.*
