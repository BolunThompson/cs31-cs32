CXX:=clang++
CXXFLAGS:=-std=c++17 \
          -Wall -Wextra -Wpedantic -Wno-sign-compare -Werror=return-type \
          -Wno-unknown-pragmas -Werror=vla \
          -Wfloat-equal -Wcast-align -Wstrict-overflow
DEBUG:=-fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer \
       -fsanitize=bounds -g -O0


mkfile_path:=$(abspath $(lastword $(MAKEFILE_LIST)))
current_dir:=$(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

zip_files=rental.cpp hw.txt report.txt
project_zip:=${current_dir}.zip

bin_files:=rental

.PHONY: debug
debug: CXXFLAGS+=${SAN} -g
debug: ${bin_files}

.PHONY: fast
fast: CXXFLAGS+=-O2
fast: ${bin_files}

.PHONY: zip
zip: ${zip_files}
	zip ${project_zip} ${zip_files}
	zip -sf ${project_zip}

.PHONY: clean
clean: 
	rm -f ${bin_files} ${project_zip}

.PHONY: test
test:
	python3 ./test.py

# using implicit rules for compiling specific files
# (although I don't know if it's good practice)
