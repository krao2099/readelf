all:
	gcc -o readelf readelf.c

debug:
	gcc -o readelf -DDEBUG readelf.c