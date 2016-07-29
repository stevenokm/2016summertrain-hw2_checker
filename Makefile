LEX=flex
#YACC=bison -y
YACC=bison -y -t
LDFLAGS+=-lfl
#CFLAGS+=-g -pg -p
CFLAGS+=-O3
CXXFLAGS+=-Wno-deprecated

all: hw2_checker

hw2_checker: blif_grammar.o blif_token.o
	$(CXX) $(CXXFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) -o $@ -c $^

blif_token.cpp: blif_token.l
	$(LEX) $(LFLAGS) -o $@ $^

blif_grammar.cpp: blif_grammar.y hw2_checker.hpp hw2_checker.cpp
	$(YACC) -d -o $@ $<

clean:
	$(RM) *.o blif_token.cpp blif_grammar.cpp hw2_checker
