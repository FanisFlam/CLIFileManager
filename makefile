CXX = g++
CXXFLAGS = -Iinclude -static-libstdc++ -static-libgcc

TARGET = cli_file_manager

SRCS = src/main.cpp src/commands.cpp src/utilities.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS) $(TARGET)