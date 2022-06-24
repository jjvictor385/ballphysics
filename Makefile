jumpball: ball.bmp
	gcc main.c -o jumpball -lSDL2 -lSDL2main

ball.bmp:
	[ -f ball.bmp ]
