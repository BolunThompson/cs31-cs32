CXX:=clang++
CXXFLAGS:=-std=c++14 -Wall -Wextra -Werror -Wpedantic

mkfile_path:=$(abspath $(lastword $(MAKEFILE_LIST)))
current_dir:=$(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

zip_files=original.cpp logic_error.cpp compile_error.cpp report.txt
project_zip:=${current_dir}.zip

bin_files:=original logic_error

.PHONY: all
all: ${bin_files}

.PHONY: zip
zip:
	zip ${project_zip} ${zip_files}
	zip -sf ${project_zip}

.PHONY: clean
clean: 
	rm -f ${bin_files} ${project_zip}

# using implicit rules for compiling specific files
# (although I don't know if it's good practice)
