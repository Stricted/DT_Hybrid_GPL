
DESTDIR=/usr/local
PREFIX=polarssl_

.SILENT:

all:
	cd library  && make all && cd ..
	cd programs && make all && cd ..

clean:
	cd library  && make clean && cd ..
	cd programs && make clean && cd ..
