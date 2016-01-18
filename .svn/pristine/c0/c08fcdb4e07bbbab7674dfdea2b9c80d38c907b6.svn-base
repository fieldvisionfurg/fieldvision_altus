CXX      = icpc
CXXFLAGS = -g -V
LIBS     =
OBJS     = example.o
SRCS     = main.cc 
TARGET   = data

$(TARGET):	
	icpc main.cc util_bin.cc util_csv.cc -o example

$(OBJS):
	$(CXX) -c $(CCFLAGS) $(SRCS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
