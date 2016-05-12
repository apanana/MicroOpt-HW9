make:
	gcc -O3 -std=c99 -march=native -o driver -Werror -Wall -Wextra -pedantic *.c

run:
	./driver quotes.txt 1000

clean:
	rm driver