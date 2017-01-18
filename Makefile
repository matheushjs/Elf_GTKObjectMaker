all:
	gcc main_UI.c -o prog -Wall -Wno-unused-result `pkg-config --cflags --libs gtk+-3.0`

clean:
	find -type f -name "*~" -execdir rm -vf '{}' \;
