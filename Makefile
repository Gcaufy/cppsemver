dist = bin/semver_test
carg = -std=c++11 -stdlib=libc++ -Weverything -Wno-c++98-compat -Wno-padded -g

build :
	mkdir -p bin && g++ $(carg) -o $(dist) semver_test.cpp
test : build
	./bin/semver_test && make clean
debug : build
	gdb ./bin/semver_test && make clean
clean : 
	rm -rf bin 