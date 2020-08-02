#Do not edit the contents of this file.
warnings_BAD: *.c
	gcc -Werror -Wall -g -std=gnu99 -o test *.c -lrt -lpthread
	
warnings_OK: *.c
	gcc -Wall -g -std=gnu99 -o test *.c -lrt -lpthread
	
clean: *.c
	rm test
