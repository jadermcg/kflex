include Makevars

all: kflex main

main: main.cpp utils.cpp utils.h smt.h smt.cpp kdive.cpp kdive.h dump.h dump.cpp khmap.h khmap.cpp
	$(CXX) $(CXXFLAGS) -o main main.cpp utils.cpp smt.cpp kdive.cpp dump.cpp khmap.cpp $(LDFLAGS) $(LIBS)

kflex: run.cpp smt.cpp smt.h utils.cpp utils.h ksearch.cpp ksearch.h dump.h dump.cpp dsearch.cpp dsearch.h kdive.cpp kdive.h khmap.cpp khmap.h
	$(CXX) $(CXXFLAGS) -o kflex run.cpp utils.cpp smt.cpp ksearch.cpp dump.cpp dsearch.cpp kdive.cpp khmap.cpp $(LDFLAGS) $(LIBS)
	

clean:
	rm -f kflex main *.o
