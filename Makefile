#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
PROG_NAME=tabfix
PROG_VERSION=0.0.8
PROG_URL=https://github.com/progman/tabfix
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
CXX=g++
LN=g++
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
#-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
CFLAGS_x32DBG=-m32 -ggdb -pg -O0 -pedantic -std=c++11 -Wall -Wextra -Wlong-long -Wunused
CFLAGS_x32REL=-m32           -O3 -pedantic -std=c++11 -Wall -Wextra -Wlong-long -Wunused
CFLAGS_x64DBG=-m64 -ggdb -pg -O0 -pedantic -std=c++11 -Wall -Wextra -Wlong-long -Wunused
CFLAGS_x64REL=-m64           -O3 -pedantic -std=c++11 -Wall -Wextra -Wlong-long -Wunused
#--analyze
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
LFLAGS_x32DBG=-m32 -ggdb
LFLAGS_x32REL=-m32 -s
LFLAGS_x64DBG=-m64 -ggdb
LFLAGS_x64REL=-m64 -s
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
help:
	@echo "use make [x32 | x64 | x32dbg | x64dbg | clean]";
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
x32dbg: tabfix.cpp Makefile
	@if [ ! -e bin ]; then    (mkdir bin;)     fi
	@$(CXX) submodule/lib_cpp/lib_cpp.cpp -o bin/lib_cpp.o -c -D'PROG_NAME="$(PROG_NAME)"' -D'PROG_VERSION="$(PROG_VERSION)"' -D'PROG_TARGET="$(@)"' -D'PROG_FULL_NAME="$(PROG_NAME)-$(@)-$(PROG_VERSION)"' -D'PROG_URL="$(PROG_URL)"' $(CFLAGS_x32DBG)
	@$(CXX) tabfix.cpp -o bin/tabfix.o -c -D'PROG_NAME="$(PROG_NAME)"' -D'PROG_VERSION="$(PROG_VERSION)"' -D'PROG_TARGET="$(@)"' -D'PROG_FULL_NAME="$(PROG_NAME)-$(@)-$(PROG_VERSION)"' -D'PROG_URL="$(PROG_URL)"' $(CFLAGS_x32DBG)
	@$(LN) bin/lib_cpp.o bin/tabfix.o -o bin/$(PROG_NAME)-$(@)-$(PROG_VERSION) $(LFLAGS_x32DBG)
	@ln -sf $(PROG_NAME)-$(@)-$(PROG_VERSION) bin/$(PROG_NAME)
	@objdump -Dslx bin/$(PROG_NAME)-$(@)-$(PROG_VERSION) > bin/$(PROG_NAME)-$(@)-$(PROG_VERSION).dump;
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
x32:    tabfix.cpp Makefile
	@if [ ! -e bin ]; then    (mkdir bin;)     fi
	@$(CXX) submodule/lib_cpp/lib_cpp.cpp -o bin/lib_cpp.o -c -D'PROG_NAME="$(PROG_NAME)"' -D'PROG_VERSION="$(PROG_VERSION)"' -D'PROG_TARGET="$(@)"' -D'PROG_FULL_NAME="$(PROG_NAME)-$(@)-$(PROG_VERSION)"' -D'PROG_URL="$(PROG_URL)"' $(CFLAGS_x32REL)
	@$(CXX) tabfix.cpp -o bin/tabfix.o -c -D'PROG_NAME="$(PROG_NAME)"' -D'PROG_VERSION="$(PROG_VERSION)"' -D'PROG_TARGET="$(@)"' -D'PROG_FULL_NAME="$(PROG_NAME)-$(@)-$(PROG_VERSION)"' -D'PROG_URL="$(PROG_URL)"' $(CFLAGS_x32REL)
	@$(LN) bin/lib_cpp.o bin/tabfix.o -o bin/$(PROG_NAME)-$(@)-$(PROG_VERSION) $(LFLAGS_x32REL)
	@ln -sf $(PROG_NAME)-$(@)-$(PROG_VERSION) bin/$(PROG_NAME)
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
x64dbg: tabfix.cpp Makefile
	@if [ ! -e bin ]; then    (mkdir bin;)     fi
	@$(CXX) submodule/lib_cpp/lib_cpp.cpp -o bin/lib_cpp.o -c -D'PROG_NAME="$(PROG_NAME)"' -D'PROG_VERSION="$(PROG_VERSION)"' -D'PROG_TARGET="$(@)"' -D'PROG_FULL_NAME="$(PROG_NAME)-$(@)-$(PROG_VERSION)"' -D'PROG_URL="$(PROG_URL)"' $(CFLAGS_x64DBG)
	@$(CXX) tabfix.cpp -o bin/tabfix.o -c -D'PROG_NAME="$(PROG_NAME)"' -D'PROG_VERSION="$(PROG_VERSION)"' -D'PROG_TARGET="$(@)"' -D'PROG_FULL_NAME="$(PROG_NAME)-$(@)-$(PROG_VERSION)"' -D'PROG_URL="$(PROG_URL)"' $(CFLAGS_x64DBG)
	@$(LN) bin/lib_cpp.o bin/tabfix.o -o bin/$(PROG_NAME)-$(@)-$(PROG_VERSION) $(LFLAGS_x64DBG)
	@ln -sf $(PROG_NAME)-$(@)-$(PROG_VERSION) bin/$(PROG_NAME)
	@objdump -Dslx bin/$(PROG_NAME)-$(@)-$(PROG_VERSION) > bin/$(PROG_NAME)-$(@)-$(PROG_VERSION).dump;
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
x64:    tabfix.cpp Makefile
	@if [ ! -e bin ]; then    (mkdir bin;)     fi
	@$(CXX) submodule/lib_cpp/lib_cpp.cpp -o bin/lib_cpp.o -c -D'PROG_NAME="$(PROG_NAME)"' -D'PROG_VERSION="$(PROG_VERSION)"' -D'PROG_TARGET="$(@)"' -D'PROG_FULL_NAME="$(PROG_NAME)-$(@)-$(PROG_VERSION)"' -D'PROG_URL="$(PROG_URL)"' $(CFLAGS_x64REL)
	@$(CXX) tabfix.cpp -o bin/tabfix.o -c -D'PROG_NAME="$(PROG_NAME)"' -D'PROG_VERSION="$(PROG_VERSION)"' -D'PROG_TARGET="$(@)"' -D'PROG_FULL_NAME="$(PROG_NAME)-$(@)-$(PROG_VERSION)"' -D'PROG_URL="$(PROG_URL)"' $(CFLAGS_x64REL)
	@$(LN) bin/lib_cpp.o bin/tabfix.o -o bin/$(PROG_NAME)-$(@)-$(PROG_VERSION) $(LFLAGS_x64REL)
	@ln -sf $(PROG_NAME)-$(@)-$(PROG_VERSION) bin/$(PROG_NAME)
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
clean:
	@if [ -e bin ]; then    rm -rf bin;    fi
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
