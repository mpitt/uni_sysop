CC = gcc
CFLAGS = -lpthread -Wall --pedantic

OBJ = raxor.o threads.o tr.o te.o td.o tw.o queue.o log.o

prefix = /usr/local
binprefix = $(prefix)/bin

.PHONY: all
all: raxor

raxor: $(OBJ)
	$(CC) $(CFLAGS) -o raxor $(OBJ)

raxor.o: raxor.h threads.h queue.h log.h
threads.o: threads.h
tr.o: threads.h tr.h
te.o: threads.h te.h
td.o: threads.h td.h
tw.o: threads.h tw.h
queue.o: queue.h
log.o: log.h

.PHONY: clean
clean:
	-rm -f $(OBJ) raxor

.PHONY: install uninstall
install: all
	install raxor $(binprefix)
	install -m 777 -d /var/log/threads/
	install rsyslog.conf /etc/rsyslog.d/raxor.conf
	service rsyslog restart

uninstall:
	-rm -f $(binprefix)/raxor
	-rm -rf /var/log/threads/
	-rm -f /etc/rsyslog.d/raxor.conf
	service rsyslog restart
