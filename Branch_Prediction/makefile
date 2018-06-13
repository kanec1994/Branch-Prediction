all: predictors

predictors: predictors.o
	g++ -std=c++11 predictors.o -o predictors

predictors.o: predictors.cpp
	g++ -g -c -std=c++11 predictors.cpp -o predictors.o

clean:
	rm -f predictors
