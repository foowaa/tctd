all:
	$(CC) -Wall -Wextra example.cpp -o example

test:
	 ./example || exit 1