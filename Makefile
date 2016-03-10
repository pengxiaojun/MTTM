CC=gcc
CFLAGS=-Wl,-rpath, -lrt -lc -lpthread

mttpm: work.c
	$(CC) -o mttpm work.c control.c queue.c $(CFLAGS)
	@echo ""
	@echo "mttpm build completed"
	@echo ""

clean: mttpm
	rm -f mttpm
