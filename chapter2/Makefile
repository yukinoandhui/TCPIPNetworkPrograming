CXX = g++
SRC = $(wildcard *.c )
SRCPP = $(wildcard *.cpp)
OBJ_C = $(patsubst %.c,%.o,$(SRC))
OBJ_CPP = $(patsubst %.cpp,%.o,$(SRCPP)) 
CXXFLAGS = -Wall -c
Target =all

$(Target): $(OBJ_C) $(OBJ_CPP)
	
$(OBJ_C): %.o:%.c
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_CPP): %.o:%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@
ifeq ("x","y")
# here's all your 'commented' makefile content...
# 这里只是为了方便注释，所以写了假if
%.o: %.c 
	$(CXX) $(CXXFLAGS) $< -o $@
%.o: %.cpp 
	$(CXX) $(CXXFLAGS) $< -o $@
endif

.PHONY:clean
clean:
	rm -f *.o *.out