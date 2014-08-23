OBJS   = src/big-text.o src/print.o src/riff.o
PROG = riff
CXX = csc 

# top-level rule to create the program.
all: $(PROG)

# compiling other source files.
%.o: %.scm 
	$(CXX) -c $<

# linking the program.
$(PROG): $(OBJS)
	$(CXX) $(OBJS) -o $(PROG)

# cleaning everything that can be automatically recreated with "make".
clean:
	rm $(PROG) src/*.o

.PHONY: all clean debug
