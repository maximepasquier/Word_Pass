.DEFAULT_GOAL := compile

compile:
	g++ -o ./bin/Main ./src/Main.cpp ./src/functions.cpp -lcrypto

run:
	./bin/Main

clear_bin:
	rm ./bin/*

clear_data:
	rm ./data/*