CFLAGS	:= -g -lpthread -Wall --pedantic

all: code queue log

code: code.c queue.o log.o
	gcc code.c queue.o log.o -o code $(CFLAGS)

queue: queue.c
	gcc -c queue.c -Wall

log: log.c
	gcc -c log.c -Wall

clean: clean_code clean_queue clean_log

clean_code: code
	@rm code

clean_queue: queue.o
	@rm queue.o

clean_log: log.o
	@rm log.o

install:
	mkdir -p /var/log/threads/
	chmod 777 -R /var/log/threads/

uninstall:
	rm -rf /var/log/threads/
