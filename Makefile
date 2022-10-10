
OBJ_FILES := obj/cuedit.o obj/cudriver.o obj/cumenu.o obj/cufile.o obj/cuproject.o

CXX_FLAGS := -std=c++17
#-O3


all: release clean

cuedit:
	mkdir -p obj
	make cuedit.o cudriver.o cumenu.o cufile.o cuproject.o

release: cuedit
	g++ $(OBJ_FILES) -o bin/cuedit

cuedit.o:
	g++ $(CXX_FLAGS) -c src/cuedit.cpp -o obj/cuedit.o
cudriver.o:
	g++ $(CXX_FLAGS) -c src/cudriver.cpp -o obj/cudriver.o
cumenu.o:
	g++ $(CXX_FLAGS) -c src/cumenu.cpp -o obj/cumenu.o
cufile.o:
	g++ $(CXX_FLAGS) -c src/cufile.cpp -o obj/cufile.o
cuproject.o:
	g++ $(CXX_FLAGS) -c src/cuproject.cpp -o obj/cuproject.o


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

