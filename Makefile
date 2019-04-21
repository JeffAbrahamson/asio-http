###########################################################################
## Handle automatic dependency calculation.  Note that this depends on
## the include line at the end of this makefile.
## Cf. http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/

DEPDIR := .d
$(shell mkdir -p $(DEPDIR)/src >/dev/null)
$(shell mkdir -p $(DEPDIR)/analysis >/dev/null)
DEPFLAGS = -MT $@ -MD -MP -MF $(DEPDIR)/$*.Td

SITE = -std=c++14 -Wall -Wextra -DBOOST_LOG_DYN_LINK
INCLUDE_DIR = 
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDE_DIR) $(TARGET_ARCH) $(SITE) -c
LINK.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $(SITE)
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

%.o : %.cc $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

###########################################################################
## Variables needed for build and install.

# GTEST = -L src/gtest -lgtest -lgtest_main -lpthread

# Unless requested, compile with debug information and no
# optimization.  Deployment scripts may override this.
CXXFLAGS ?= -ggdb3
CFLAGS ?= -ggdb3

SRC_FILES =			\
	connection.cc 		\
	connection_manager.cc 	\
	main.cc 		\
	mime_types.cc 		\
	reply.cc 		\
	request_handler.cc 	\
	request_parser.cc 	\
	server.cc 		\

SRC = $(SRC_FILES:%.cc=src/%.cc)
OBJ = $(SRC:%.cc=%.o)
BIN = http

###########################################################################
## Targets intended for users.

all: $(BIN) TAGS

clean:
	rm -f src/*.o
	rm -rf http

###########################################################################
## Build http

BOOST_LIBS =					\
	-lboost_program_options			\
	-lpthread				\

$(BIN): Makefile $(OBJ)
	$(LINK.cc) -o $@ $(OBJ) $(BOOST_LIBS)

TAGS:
	etags src/*h src/*cc > src/TAGS

###########################################################################
## Include dependency information computed at top of makefile.

include $(shell find $(DEPDIR) -type f)
