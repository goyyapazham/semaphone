all : 
	gcc control.c -o control
	gcc client.c -o client

clean : 
	rm client
	rm control
	rm *~
