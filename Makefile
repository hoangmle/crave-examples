CRAVE_HOME = ../../build/root
include $(CRAVE_HOME)/share/crave/crave.makefile

SYSTEMC_INCLUDE = $(SYSTEMC_HOME)/include
SYSTEMC_LIB = $(SYSTEMC_HOME)/lib-linux/libsystemc.a
ifeq (${shell getconf LONG_BIT}, 64)
  SYSTEMC_LIB = $(SYSTEMC_HOME)/lib-linux64/libsystemc.a
endif

TLM_INCLUDE = $(TLM_HOME)/include/tlm

INCLUDES = -I $(crave_INCLUDES)
LIBS = $(crave_LIBRARIES)

SRC = main.cpp
TARGET = test

all: $(SRC)
	g++ $(SRC) $(INCLUDES) $(LIBS) -o $(TARGET) -g -O0

clean:
	rm -f $(TARGET)

