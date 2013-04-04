all: code

code: code.c
	gcc code.c -o code -lpthread

clean:
	[[ -e code ]] && rm code
