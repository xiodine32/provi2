build: punctaje_fix server
	./punctaje_fix
server: main.cpp rules.cpp server.cpp
	g++ -o server main.cpp rules.cpp server.cpp -O2 -lpthread -g

punctaje_fix: rules_fix.cpp punctaje_fix.cpp
	g++ -o punctaje_fix rules_fix.cpp punctaje_fix.cpp -O2 -lpthread -g --std=c++11