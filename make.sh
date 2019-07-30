gcc -g -O -c uttf.c -o uttf.o -Iexternal/freetype-2.10.1/include/ -I/usr/local/include/SDL2/  -ldsl2
ar rcs libuttf.a uttf.o

gcc libuttf.a showfont.c -lsdl2 -I/usr/local/include/SDL2/ external/freetype-2.10.1/objs/.libs/libfreetype.a
