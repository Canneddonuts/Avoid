clang :
	clang src/Main.c src/Title.c src/Credits.c src/Gameover.c src/Gameplay.c -std=c99 -Wall -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o Avoid

tcc :
	tcc src/Main.c src/Title.c src/Credits.c src/Gameover.c src/Gameplay.c -std=c99 -Wall -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o Avoid

gcc :
	gcc src/Main.c src/Title.c src/Credits.c src/Gameover.c src/Gameplay.c -std=c99 -Wall -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o Avoid
