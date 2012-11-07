#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
PROG_NAME=tabfix
PROG_VERSION=0.0.3
PROG_URL=https://github.com/progman/tabfix
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
CXX=g++
LN=g++
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
#-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
CFLAGS_x32DBG=-m32 -ggdb -pg -pedantic -O0 -std=c++11 -Wall -Wextra -Wlong-long -Wunused
CFLAGS_x32REL=-m32           -pedantic -O3 -std=c++11 -Wall -Wextra -Wlong-long -Wunused
CFLAGS_x64DBG=-m64 -ggdb -pg -pedantic -O0 -std=c++11 -Wall -Wextra -Wlong-long -Wunused
CFLAGS_x64REL=-m64           -pedantic -O3 -std=c++11 -Wall -Wextra -Wlong-long -Wunused
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
	@$(CXX) tabfix.cpp -c -o bin/tabfix.o -D'PROG_NAME="$(PROG_NAME)"' -D'PROG_VERSION="$(PROG_VERSION)"' -D'PROG_TARGET="$(@)"' -D'PROG_FULL_NAME="$(PROG_NAME)-$(@)-$(PROG_VERSION)"' -D'PROG_URL="$(PROG_URL)"' $(CFLAGS_x32DBG)
	@$(LN) bin/tabfix.o -o bin/$(PROG_NAME)-$(@)-$(PROG_VERSION) $(LFLAGS_x32DBG)
	@ln -sf $(PROG_NAME)-$(@)-$(PROG_VERSION) bin/$(PROG_NAME)
	@objdump -Dslx bin/$(PROG_NAME)-$(@)-$(PROG_VERSION) > bin/$(PROG_NAME)-$(@)-$(PROG_VERSION).dump;
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
x32:    tabfix.cpp Makefile
	@if [ ! -e bin ]; then    (mkdir bin;)     fi
	@$(CXX) tabfix.cpp -c -o bin/tabfix.o -D'PROG_NAME="$(PROG_NAME)"' -D'PROG_VERSION="$(PROG_VERSION)"' -D'PROG_TARGET="$(@)"' -D'PROG_FULL_NAME="$(PROG_NAME)-$(@)-$(PROG_VERSION)"' -D'PROG_URL="$(PROG_URL)"' $(CFLAGS_x32REL)
	@$(LN) bin/tabfix.o -o bin/$(PROG_NAME)-$(@)-$(PROG_VERSION) $(LFLAGS_x32REL)
	@ln -sf $(PROG_NAME)-$(@)-$(PROG_VERSION) bin/$(PROG_NAME)
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
x64dbg: tabfix.cpp Makefile
	@if [ ! -e bin ]; then    (mkdir bin;)     fi
	@$(CXX) tabfix.cpp -c -o bin/tabfix.o -D'PROG_NAME="$(PROG_NAME)"' -D'PROG_VERSION="$(PROG_VERSION)"' -D'PROG_TARGET="$(@)"' -D'PROG_FULL_NAME="$(PROG_NAME)-$(@)-$(PROG_VERSION)"' -D'PROG_URL="$(PROG_URL)"' $(CFLAGS_x64DBG)
	@$(LN) bin/tabfix.o -o bin/$(PROG_NAME)-$(@)-$(PROG_VERSION) $(LFLAGS_x64DBG)
	@ln -sf $(PROG_NAME)-$(@)-$(PROG_VERSION) bin/$(PROG_NAME)
	@objdump -Dslx bin/$(PROG_NAME)-$(@)-$(PROG_VERSION) > bin/$(PROG_NAME)-$(@)-$(PROG_VERSION).dump;
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
x64:    tabfix.cpp Makefile
	@if [ ! -e bin ]; then    (mkdir bin;)     fi
	@$(CXX) tabfix.cpp -c -o bin/tabfix.o -D'PROG_NAME="$(PROG_NAME)"' -D'PROG_VERSION="$(PROG_VERSION)"' -D'PROG_TARGET="$(@)"' -D'PROG_FULL_NAME="$(PROG_NAME)-$(@)-$(PROG_VERSION)"' -D'PROG_URL="$(PROG_URL)"' $(CFLAGS_x64REL)
	@$(LN) bin/tabfix.o -o bin/$(PROG_NAME)-$(@)-$(PROG_VERSION) $(LFLAGS_x64REL)
	@ln -sf $(PROG_NAME)-$(@)-$(PROG_VERSION) bin/$(PROG_NAME)
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
clean:
	@if [ -e bin ]; then    rm -rf bin;    fi
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
