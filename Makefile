
all: release debug clean

cuedit:
	mkdir -p obj
	g++ -c src/cuedit.cpp -o obj/cuedit.o
	g++ -c src/cudriver.cpp -o obj/cudriver.o

release: cuedit
	g++ obj/cuedit.o obj/cudriver.o -o bin/cuedit

debug: cuedit
	g++ -g obj/cuedit.o obj/cudriver.o -o bin/cuedit_debug

clean:
	rm -f obj/*.o

github:
	@read -p "Please enter a commit message:" msg

	git add src/*.cpp
	git add src/*.h
	git add Makefile
	git commit -m $msg

