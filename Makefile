derp : derpscript.cpp repl/derp.cpp
	g++ -Iinclude -Wall $^ -o $@

clean :
	- rm derp


