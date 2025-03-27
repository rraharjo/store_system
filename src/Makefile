#compiler
CXX = g++
CXXFLAGS = -g
#include
INCL = header_files
INCLJSON = /home/raharjo/external_lib/json/include
INCLPSQL = -lpq


TARGET = main.exe
SRCS = $(wildcard source_files/accounting/accounting_transaction/*.cpp) $(wildcard source_files/accounting/accounts/*.cpp) $(wildcard source_files/accounting/*.cpp) $(wildcard source_files/inventory/assets/*.cpp) $(wildcard source_files/inventory/transaction/*.cpp) $(wildcard source_files/inventory/*.cpp) $(wildcard source_files/store/transaction/*.cpp) $(wildcard source_files/store/*.cpp) $(wildcard source_files/util/*.cpp) $(wildcard source_files/util/class/*.cpp) $(wildcard source_files/util/database/*.cpp) $(wildcard source_files/util/enums/date/*.cpp) $(wildcard source_files/util/enums/accounting/*.cpp) $(wildcard source_files/util/enums/database/*.cpp) $(wildcard source_files/util/factory/*.cpp) source_files/util/network/inbound_message.cpp source_files/util/network/message.cpp source_files/util/network/outbound_message.cpp source_files/util/network/unix_server.cpp source_files/driver/executor.cpp source_files/driver/unix_driver.cpp
SRCSHELL = source_files/.env/private.cpp $(shell find source_files -type f -name "*.cpp" ! -name "private.cpp")
OBJS = $(SRCS:.cpp:.o)
MAINSRCS = main.cpp $(SRCSHELL)
MAINOBJS = main.o $(OBJS)
CLIENTSRCS = sample_client.cpp source_files/util/network/inbound_message.cpp source_files/util/network/outbound_message.cpp source_files/util/network/message.cpp

# create object
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I $(INCL) -I $(INCLJSON) -c $< -o $@

# create exe
$(TARGET): $(MAINOBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(MAINOBJS)

#clean
clean:
	rm -f $(OBJS) $(TARGET)

main:
	$(CXX) $(CXXFLAGS) -o $(TARGET) -I $(INCL) -I $(INCLJSON) $(MAINSRCS) $(INCLPSQL)

client:
	$(CXX) $(CXXFLAGS) -o sample_client.exe -I $(INCL) $(CLIENTSRCS)