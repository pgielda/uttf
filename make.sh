gcc -g -O -c uttf.c -o uttf.o -Iexternal/freetype-2.10.1/include/
ar rcs libuttf.a uttf.o

gcc showfont.c libuttf.a -lSDL2 -I/usr/include/SDL2/ external/freetype-2.10.1/objs/.libs/libfreetype.a
