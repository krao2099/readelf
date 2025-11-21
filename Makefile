all:
	clang -o readelf readelf.c

debug:
	clang -o readelf -DDEBUG readelf.c
clean:
	rm readelf
