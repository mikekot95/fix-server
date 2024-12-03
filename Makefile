CXX = g++
CXXFLAGS = -I include -std=c++17
SRCDIR = src
INCDIR = include
BINDIR = bin
TARGET_SERVER = $(BINDIR)/server
TARGET_CLIENT = $(BINDIR)/client
QUICKFIX_PATH = ~

SOURCES = $(wildcard $(SRCDIR)/*.cpp) server.cpp
OBJECTS = $(SOURCES:.cpp=.o)

CLIENT_SOURCES = $(QUICKFIX_PATH)/quickfix/examples/tradeclient/tradeclient.cpp $(QUICKFIX_PATH)/quickfix/examples/tradeclient/Application.cpp
CLIENT_OBJECTS = $(CLIENT_SOURCES:.cpp=.o)

all: $(TARGET_SERVER) $(TARGET_CLIENT)

$(TARGET_SERVER): $(OBJECTS) | $(BINDIR)
	$(CXX) -o $@ $^ $(QUICKFIX_PATH)/quickfix/lib/libquickfix.so

$(TARGET_CLIENT): $(CLIENT_OBJECTS) | $(BINDIR)
	$(CXX) -o $@ $^ $(QUICKFIX_PATH)/quickfix/lib/libquickfix.so

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp $(INCDIR)/%.h 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(QUICKFIX_PATH)/quickfix/examples/tradeclient/%.o: $(QUICKFIX_PATH)/quickfix/examples/tradeclient/%.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -f $(OBJECTS) $(CLIENT_OBJECTS) $(TARGET_SERVER) $(TARGET_CLIENT)

