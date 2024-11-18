CC = cc
CFLAGS = -std=c11 -Wall -Wextra -Werror

.PHONY: clean pack run

figsearch: figsearch.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -Rf *.o figsearch odevzdani.tar.gz

pack:
	tar -czvf odevzdani.tar.gz ./*

run: tnine
	./figsearch