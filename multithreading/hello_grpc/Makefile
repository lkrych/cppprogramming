LDFLAGS = -L/usr/local/lib `pkg-config --libs protobuf grpc++`

CXX = g++
CPPFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++11

GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

all: clean client server async_client async_server

client: hello.pb.o hello.grpc.pb.o hello_grpc_client.o
	$(CXX) $^ $(LDFLAGS) -o $@

server: hello.pb.o hello.grpc.pb.o hello_grpc_server.o
	$(CXX) $^ $(LDFLAGS) -o $@

async_client: hello.pb.o hello.grpc.pb.o hello_async_client.o
	$(CXX) $^ $(LDFLAGS) -o $@

async_server: hello.pb.o hello.grpc.pb.o hello_async_server.o
	$(CXX) $^ $(LDFLAGS) -o $@

%.grpc.pb.cc: %.proto
	protoc --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

%.pb.cc: %.proto
	protoc --cpp_out=. $<

clean:
	rm -f *.o *.pb.cc *.pb.h client server