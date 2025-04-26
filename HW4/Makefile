all: main

main: main.cpp
	g++ -Wall -Wpedantic -Wextra -std=c++23 $^

test: main
	cat test.txt
	cat test.txt | ./a.out

test2: main
	cat test2.txt
	cat test2.txt | ./a.out

clean:
	rm ./a.out
