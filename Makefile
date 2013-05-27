CFLAGS	:= -lpthread -Wall --pedantic

.PHONY: clean

all: code queue log

code: code.c queue.o log.o
	gcc code.c queue.o log.o -o code $(CFLAGS)

debug: code.c queue.o log.o
	gcc code.c queue.o log.o -o code $(CFLAGS) -ggdb

queue: queue.c
	gcc -c queue.c -Wall

log: log.c
	gcc -c log.c -Wall

clean:
	@rm -f *.o code

install:
	mkdir -p /var/log/threads/
	chmod 777 -R /var/log/threads/

uninstall:
	rm -rf /var/log/threads/
