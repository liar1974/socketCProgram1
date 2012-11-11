project:
	gcc -o Library1 Library1.c -lsocket -lnsl -lresolv
	gcc -o Library2 Library2.c -lsocket -lnsl -lresolv
	gcc -o Library3 Library3.c -lsocket -lnsl -lresolv
	gcc -o Database Database.c -lsocket -lnsl -lresolv
	gcc -o User1 User1.c -lsocket -lnsl -lresolv
	gcc -o User2 User2.c -lsocket -lnsl -lresolv