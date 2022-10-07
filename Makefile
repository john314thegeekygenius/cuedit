
OBJ_FILES := obj/cuedit.o obj/cudriver.o obj/cumenu.o obj/cufile.o

all: release debug clean

cuedit:
	mkdir -p obj
	make cuedit.o cudriver.o cumenu.o cufile.o

release: cuedit
	g++ $(OBJ_FILES) -o bin/cuedit

debug: cuedit
	g++ -g $(OBJ_FILES) -o bin/cuedit_debug

cuedit.o:
	g++ -std=c++20 -c src/cuedit.cpp -o obj/cuedit.o
cudriver.o:
	g++ -std=c++20 -c src/cudriver.cpp -o obj/cudriver.o
cumenu.o:
	g++ -std=c++20 -c src/cumenu.cpp -o obj/cumenu.o
cufile.o:
	g++ -std=c++20 -c src/cufile.cpp -o obj/cufile.o


clean:
	rm -f obj/*.o

github:
	git add src/*.cpp
	git add src/*.h
	git add *.code-workspace
	git add *.md
	git add Makefile
	git add bin
	@read -p "Please enter a commit message:" msg; \
	git commit -m "$$msg"
	git push

