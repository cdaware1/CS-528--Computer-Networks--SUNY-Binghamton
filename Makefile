all : 
	g++ -o server server.cpp
	g++ -o client client.cpp	

clean :
	rm -rf client server