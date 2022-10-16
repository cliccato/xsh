install:
	sudo mv bin/xsh $PATH

compile:
	gcc src/*.c -o bin/xsh --no-warnings

