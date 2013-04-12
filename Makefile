CC_FLAGS	:= -lpthread -Wall --pedantic

all: code queue log

code: code.c queue.o log.o
	gcc code.c queue.o log.o -o code $(CC_FLAGS)

queue: queue.c
	gcc -c queue.c -Wall

log: log.c
	gcc -c log.c -Wall

clean: clean_code clean_queue clean_log

clean_code:
	rm code

clean_queue:
	rm queue.o

clean_log:
	rm log.o

install:
	mkdir -p /var/log/threads/
	chmod 777 -R /var/log/threads/
