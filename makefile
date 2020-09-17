 # All Targets
all: Assignment1

# Tool invocations
Assignment1: bin/Main.o bin/Session.o bin/Watchable.o bin/Action.o bin/User.o
	@echo 'Building target: Assignment1'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/splflix bin/Main.o bin/Session.o bin/Watchable.o bin/Action.o bin/User.o
	@echo 'Finished building target: Assignment1'
	@echo ' '

# Depends on the source and header files
bin/Main.o: src/Main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp

# Depends on the source and header files 
bin/Session.o: src/Session.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Session.o src/Session.cpp
	
bin/Watchable.o: src/Watchable.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Watchable.o src/Watchable.cpp
	
bin/User.o: src/User.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/User.o src/User.cpp

bin/Action.o: src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

#Clean the build directory
clean: 
	rm -f bin/*
