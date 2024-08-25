all: build/progBuild


build/progBuild: src/main.c
	gcc -o build/progBuild src/main.c -lcjson
