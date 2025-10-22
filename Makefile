game: gameboard.c bitmanipulation.c main.c
	gcc -o game gameboard.c bitmanipulation.c main.c
	
clean: 
	rm -f game