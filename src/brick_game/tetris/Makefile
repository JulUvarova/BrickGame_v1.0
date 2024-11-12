SHELL:=/bin/bash
CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11 -pedantic
CFLAGS_BIN=$(subst -c ,,$(CFLAGS))
TESTFLAGS=-fprofile-arcs -ftest-coverage
LDFLAGS=
SOURCES=$(wildcard *.c)
TEST_SOURCES=$(wildcard tests/*.c)
OBJECTS=$(SOURCES:.c=.o)
LIBNAME=s21_tetris.a
S21_DEBUG?=false
# all, install, uninstall, clean, dvi, dist, test, gcov_report
ifeq ($(S21_DEBUG),true)   
	DEBUG_FLAGS=-fsanitize=leak -fsanitize=address -fsanitize=undefined -fsanitize=unreachable -fno-sanitize-recover -fstack-protector
else
	DEBUG_FLAGS=
endif

.PHONY: all
# all: $(LIBNAME)
all: cleanall
	$(CC) $(CFLAGS_BIN) *.c -o s21_tetris -lcheck -lm -lsubunit -lncurses 

.PHONY: check
check:
	cppcheck --enable=all --suppress=missingIncludeSystem ./
	clang-format -n *.c
	clang-format -n *.h
	clang-format -n tests/*.c
	clang-format -n tests/*.h

.PHONY: $(LIBNAME)
$(LIBNAME): build clean

.PHONY: build
build: $(OBJECTS)
	ar rcs $(LIBNAME) $(OBJECTS)  -lncurses

.PHONY: test
test: CFLAGS_BIN += $(TESTFLAGS)
test: build clean
	$(CC) $(CFLAGS_BIN) $(DEBUG_FLAGS) $(TEST_SOURCES) $(LIBNAME) -o tests/tests -lcheck -lm -lsubunit 
ifeq ($(S21_DEBUG),true)   
	./tests/tests
else
	valgrind -q -s --leak-check=full --trace-children=yes --track-origins=yes --log-file=RESULT_VALGRIND.txt ./tests/tests
endif

.PHONY: clean
clean:
	rm -f {.,tests}/*.o {.,tests}/*.gcda {.,tests}/*.gcno
	rm -rf report
	rm -f tests/{s21_test.info,s21_test,tests}
	rm -f RESULT_VALGRIND.txt

.PHONY: cleanall
cleanall:
	rm -f {.,tests}/*.o {.,tests}/*.gcda {.,tests}/*.gcno *.a
	rm -rf report
	rm -f tests/{s21_test.info,s21_test,tests}
	# rm s21_tetris
	rm -f RESULT_VALGRIND.txt

.PHONY: gcov_report
gcov_report: test
	$(CC) --coverage $(CFLAGS_BIN) $(DEBUG_FLAGS) $(SOURCES) $(TEST_SOURCES) -o tests/s21_test -lcheck -lm -lsubunit -lgcov
	./tests/s21_test
	lcov -t "s21_test" -o tests/s21_test.info -c -d .
	genhtml -o report tests/s21_test.info


%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $< -o $@