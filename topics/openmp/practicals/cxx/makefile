all: dot.exe normalize.exe hello_world.exe sum_threads.exe pi.exe first_touch.exe

pi.exe : pi.cpp
	CC -O3 pi.cpp -fopenmp -o pi.exe

normalize.exe : normalize.cpp
	CC -O3 normalize.cpp -fopenmp -o normalize.exe

hello_world.exe : hello_world.cpp
	CC -O3 hello_world.cpp -fopenmp -o hello_world.exe

dot.exe : dot.cpp
	CC -O3 dot.cpp -fopenmp -o dot.exe

sum_threads.exe : sum_threads.cpp
	CC -O3 sum_threads.cpp -fopenmp -o sum_threads.exe

first_touch.exe : first_touch.cpp
	CC -O3 first_touch.cpp -fopenmp -o first_touch.exe

clean :
	rm -f *.exe
