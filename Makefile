LIBPATH += -L"/System/Library/Frameworks/OpenGL.framework/Libraries" 

FRAMEWORK = -framework GLUT
FRAMEWORK += -framework OpenGL

OUTPATH = build/
OBJS = util.o cmd.o config.o ticket.o redmine.o 
COMPILERFLAGS = -Wall
CC = g++
CFLAGS = $(COMPILERFLAGS)
LIBRARIES = -lGL -lGLU -lm -lobjc -lstdc++

ALL: git-ticket

git-ticket: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(OUTPATH)$@ main.c
	$(OUTPATH)$@

-include $(OBJS:.o=.d)

%.o: %.c
	$(CC) -c $(CFLAGS) $*.c -o $*.o 
	$(CC) -MM $(CFLAGS) $*.c > $*.d 

example1-2: example1-2.o example1-2.c
	$(CC) $(FRAMEWORK) $(CFLAGS) -o $@.exe $(LIBPATH) $@.c $(LIBRARIES)

clean: 
	rm -rf $(OUTPATH)*.o $(OUTPATH)git-ticket $(OUTPATH)in.* *.o *.d in.*
