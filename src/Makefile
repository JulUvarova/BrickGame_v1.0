SHELL:=/bin/bash
CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11 -pedantic -g
CFLAGS_BIN=$(subst -c ,,$(CFLAGS))
TESTFLAGS=-fprofile-arcs -ftest-coverage
LDFLAGS=
BACK_SOURCES=$(wildcard brick_game/tetris/*.c)
BACK_OBJECTS=$(BACK_SOURCES:.c=.o)
BACK_LIBNAME=tetris_backend.a
FRONT_SOURCES=$(wildcard gui/cli/*.c)
FRONT_OBJECTS=$(FRONT_SOURCES:.c=.o)
FRONT_LIBNAME=frontend.a
TEST_SOURCES=$(wildcard brick_game/tetris/tests/*.c)
S21_DEBUG?=false
ifeq ($(S21_DEBUG),true)   
	DEBUG_FLAGS=-fsanitize=leak -fsanitize=address -fsanitize=undefined -fsanitize=unreachable -fno-sanitize-recover -fstack-protector
else
	DEBUG_FLAGS=
endif

.PHONY: all
all:  install
	chmod +x ../build/s21_tetris
	../build/s21_tetris

.PHONY: build_back
build_back:  $(BACK_OBJECTS)
	ar rcs $(BACK_LIBNAME) $(BACK_OBJECTS) 
	rm -f brick_game/tetris/*.o


.PHONY: build_front
build_front:  $(FRONT_OBJECTS)
	ar rcs $(FRONT_LIBNAME) $(FRONT_OBJECTS) -lncurses
	rm -f gui/cli/*.o

.PHONY: install
install: clean uninstall build_back build_front 
	$(CC) $(CFLAGS_BIN) tetris.c *.a -o ../build/s21_tetris -lncurses 
	rm -f *.a

.PHONY: uninstall
uninstall: 
	rm -f ../build/s21_tetris

.PHONY: clean
clean:
	rm -f {.,brick_game/tetris, gui/cli, tests}/*.o {.,tests}/*.gcda {.,tests}/*.gcno
	rm -rf report
	rm -f tests/{s21_test.info,s21_test,tests}
	rm -f RESULT_VALGRIND.txt
	rm -f *.a
	rm -f tetris.install.tar.gz

.PHONY: dvi
dvi: 
	open ./README.md

.PHONY: dist
dist: 
	tar -czf tetris.install.tar.gz ./* 

.PHONY: test
test:  CFLAGS_BIN += $(TESTFLAGS)
test: clean uninstall build_back build_front 
	$(CC) $(CFLAGS_BIN) $(DEBUG_FLAGS) $(TEST_SOURCES) *.a -o brick_game/tetris/tests/tests -lcheck -lm -lsubunit
ifeq ($(S21_DEBUG),true)   
	./tests/tests
else
	valgrind -q -s --leak-check=full --trace-children=yes --track-origins=yes --log-file=RESULT_VALGRIND.txt ./tests/tests
endif

.PHONY: gcov_report
gcov_report: test
	$(CC) --coverage $(CFLAGS_BIN) $(DEBUG_FLAGS) $(SOURCES) $(TEST_SOURCES) -o tests/s21_test -lcheck -lm -lsubunit -lgcov
	./tests/s21_test
	lcov -t "s21_test" -o tests/s21_test.info -c -d .
	genhtml -o report tests/s21_test.info

.PHONY: check
check:
	cppcheck --enable=all --suppress=missingIncludeSystem ./
	clang-format -n ../brick_game/tetris/*.c
	clang-format -n ../brick_game/tetris/*.h
	clang-format -n ../brick_game/tetris/tests/*.c
	clang-format -n ../brick_game/tetris/tests/*.h
	clang-format -n ../gui/cli/*.c
	clang-format -n ../gui/cli/*.h

%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $< -o $@	



