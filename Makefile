all : derpscript.cpp repl/derp.cpp
	g++ -g -Iinclude -Wall $^ -o derp_97
	g++ -std=c++11 -g -Iinclude -Wall $^ -o derp_11

clean :
	- rm derp


