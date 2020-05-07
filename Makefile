orash: inc/linenoise.h inc/builtin.h src/linenoise.c src/builtin.c

orash: src/linenoise.c src/repl.c src/builtin.c
	mkdir -p bin
	$(CC) -Wall -W -Os -g -o bin/orash src/linenoise.c src/repl.c src/builtin.c

clean:
	rm -f orash
