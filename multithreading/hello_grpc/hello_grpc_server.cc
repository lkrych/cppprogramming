#include <iostream>
#include <string>
#include <sstream>

#include <grpcpp/grpcpp.h>
#include "hello.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using hello::HelloService;
using hello::Greeting;
using hello::GreetingResponse;

class HelloServiceImplementation final : public HelloService::Service {
    Status sayHello(
        ServerContext* context,
        const Greeting* greeting,
        GreetingResponse* reply
    ) override {
        std::string name = greeting->name();
        std::stringstream ss;
        ss << "Hello " << name << std::endl;
        
        reply->set_message(ss.str());

        return Status::OK;
    }
};

void Run() {
    std::string address("0.0.0.0:5000");
    HelloServiceImplementation service;

    ServerBuilder builder;

    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on port: " << address << std::endl;

    server->Wait();
};

int main(int argc, char* argv[]) {
    Run();
    return 0;
};

