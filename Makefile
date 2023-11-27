run: enduro

enduro: bibutil.o
	g++ enduro.c bibutil.o -framework OpenGL -framework GLUT -lX11 -lm -lpthread -ljpeg -w && ./a.out
bibutil.o: bibutil.cpp
	g++ -c -g -Wall bibutil.cpp -w
clean: 
	rm *.o