all: dot.exe normalize.exe hello_world.exe sum_threads.exe pi.exe first_touch.exe laplace.exe dispersion.exe histogram.exe

pi.exe : pi.cpp
	CC -dynamic -O3 pi.cpp -fopenmp -o pi.exe

normalize.exe : normalize.cpp
	CC -dynamic -O3 normalize.cpp -fopenmp -o normalize.exe

hello_world.exe : hello_world.cpp
	CC -dynamic -O3 hello_world.cpp -fopenmp -o hello_world.exe

dot.exe : dot.cpp
	CC -dynamic -O3 dot.cpp -fopenmp -o dot.exe

sum_threads.exe : sum_threads.cpp
	CC -dynamic -O3 sum_threads.cpp -fopenmp -o sum_threads.exe

first_touch.exe : first_touch.cpp
	CC -dynamic -O3 first_touch.cpp -fopenmp -o first_touch.exe

histogram.exe : histogram.cpp
	CC -dynamic -O3 histogram.cpp -fopenmp -o histogram.exe

dispersion.exe : dispersion.c
	cc -dynamic -fopenmp -std=c99 -O3 -Wall dispersion.c -o dispersion.exe

laplace.exe : laplace.c
	cc -dynamic -fopenmp -std=c99 -O3 -Wall dispersion.c -o laplace.exe

clean :
	rm -f *.exe
