#include <iostream>
#include <string>

#include <grpcpp/grpcpp.h>
#include "hello.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::CompletionQueue;

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

        //the producer-consumer queue we use to communicate asynchronously with the gRPC runtime.
        CompletionQueue cq;

        Status status;
        // stub_->PrepareAsyncsayHello() creates an RPC object, returning
        // an instance to store in "call" but does not actually start the RPC
        // because we are using the async API, we need to hold onto the "call"
        // instance in order to get updates on the ongoing RPC
        std::unique_ptr<ClientAsyncResponseReader<GreetingReply> > rpc(
            stub_->PrepareAsyncsayHello(&context, request, &cq));
        )

        rpc->StartCall();

        // Request that, upon completion of the RPC, "reply" be updated with the
        // server's response; "status" with the indication of whether the operation was successful
        /// Tag the request with integer one
        rpc->Finish(&reply, &status, (void*)1);
        void* got_tag;

        bool ok = false;

        // Block until the next result is available in the completion queue "cq".
        // The return value of Next should always be checked. This return value
        // tells us whether there is any kind of event or the cq_ is shutting down.
        GPR_ASSERT(cq.Next(&got_tag, &ok));

        // Verify that the result from "cq" corresponds, by its tag, our previous
        // request.
        GPR_ASSERT(got_tag == (void*)1);
        // ... and that the request was completed successfully. Note that "ok"
        // corresponds solely to the request for updates introduced by Finish().
        GPR_ASSERT(ok);

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