CXX:=clang++
CXXFLAGS:=-std=c++17 \
          -Wall -Wextra -Wpedantic -Wno-sign-compare -Werror=return-type \
          -Wno-unknown-pragmas -Werror=vla \
          -Wfloat-equal -Wcast-align -Wstrict-overflow
DEBUG:=-g -O0 -fstandalone-debug -fsanitize=address -fno-omit-frame-pointer


mkfile_path:=$(abspath $(lastword $(MAKEFILE_LIST)))
current_dir:=$(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

zip_files=array.cpp report.txt
project_zip:=${current_dir}.zip

bin_files:=array

.PHONY: debug
debug: CXXFLAGS+=${DEBUG}
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


# using implicit rules for compiling specific files
# (although I don't know if it's good practice)
