
OBJ_FILES := obj/cuedit.o obj/cudriver.o obj/cumenu.o

all: release debug clean

cuedit:
	mkdir -p obj
	g++ -std=c++20 -c src/cuedit.cpp -o obj/cuedit.o
	g++ -std=c++20 -c src/cudriver.cpp -o obj/cudriver.o
	g++ -std=c++20 -c src/cumenu.cpp -o obj/cumenu.o

release: cuedit
	g++ $(OBJ_FILES) -o bin/cuedit

debug: cuedit
	g++ -g $(OBJ_FILES) -o bin/cuedit_debug

clean:
	rm -f obj/*.o

github:
	git add src/*.cpp
	git add src/*.h
	git add Makefile
	@read -p "Please enter a commit message:" msg; \
	git commit -m "$$msg"
	git push

