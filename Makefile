all:
	$(CC) -Wall -Wextra tctd.h tctd.c example.c -o example

test:
	 ./example || exit 1