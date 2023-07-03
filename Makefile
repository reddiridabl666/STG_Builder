GEN = "Unix Makefiles"
CC = c++
BUILD = _build
DEPS = ${BUILD}/_deps
SRC_DIRS = app pkg

run: build
	./${BUILD}/app/stg_builder_engine

build:
	cmake --build ${BUILD}

rebuild: clear build

clear:
	rm -r ${BUILD}/*

lint:
	cppcheck --suppress=missingInclude --enable=all --error-exitcode=-1 --check-config --inline-suppr --inconclusive ${SRC_DIRS}
	clang-tidy-16 -p ${BUILD} $$(find ${SRC_DIRS} -name *.cpp)
