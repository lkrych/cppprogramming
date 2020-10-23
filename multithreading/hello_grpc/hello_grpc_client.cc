#include <iostream>
#include <string>

#include <grpcpp/grpcpp.h>
#include "hello.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using hello::HelloService;
using hello::Greeting;
using hello::GreetingResponse;

class HelloServiceClient {
    public:
        HelloServiceClient(std::shared_ptr<Channel> channel) : stub_(HelloService::NewStub(channel)) {}
    
    std::string sendRequest(std::string name) {
        Greeting request;
        
        request.set_name(name);

        GreetingResponse reply;

        ClientContext context;

        Status status = stub_->sayHello(&context, request, &reply);

        if (status.ok()) {
            return reply.message();
        } else {
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            return " :( There was an error sending your greeting";
        }
    }

    private:
        std::unique_ptr<HelloService::Stub> stub_;
};

void Run(std::string name) {
    std::string address("0.0.0.0:5000");
    HelloServiceClient client(
        grpc::CreateChannel(
            address,
            grpc::InsecureChannelCredentials()
        )
    );

    std::string response;
    
    response = client.sendRequest(name);
    std::cout << response << std::endl;
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: ./client Friend" << std::endl;
        exit(1);
    }

    std::string name = argv[1];
    Run(name);

    return 0;
}