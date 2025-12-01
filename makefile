GCC = g++

GCCFLAGS1 = -lboost_system -lpthread

GCCFLAGS2 = -Irapidjson/include/ -lboost_system -lpthread

TARGET1 = launcher

TARGET2 = parser

SOURCE1 = boost_child.cpp 
SOURCE2 = slurm_parser.cpp 

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(SOURCE1)
	$(GCC) -o $(TARGET1) $(SOURCE1) $(GCCFLAGS1)

$(TARGET2): $(SOURCE2)
	$(GCC) -o $(TARGET2) $(SOURCE2) $(GCCFLAGS2)

clean:
	rm -f $(TARGET1) $(TARGET2)

.PHONY: all clean
