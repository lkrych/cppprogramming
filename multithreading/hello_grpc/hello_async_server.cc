#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <thread>

#include <grpcpp/grpcpp.h>
#include <grpc/support/log.h>
#include "hello.grpc.pb.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::Status;

using hello::HelloService;
using hello::Greeting;
using hello::GreetingResponse;

class AsyncServer final {
    public:
        //destructor
        ~AsyncServer() {
            server_->Shutdown();
            //always shutdown the completion queue after the server;
            cq_->Shutdown();
        }
    
    void Run() {
        std::string server_address("0.0.0.0:5000");

        ServerBuilder builder;

        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        
        //Register service_ as the instance we'll communicate with the clients
        builder.RegisterService(&service_);

        //Get ahold of the completion queue used for Async communication with
        // the grpc runtime
        cq_ = builder.AddCompletionQueue();
        // Assemble the server
        server_ = builder.BuildAndStart();
        std::cout << "Server listening on " << server_address << std::endl;

        // Proceed to server's main loop.
        HandleRpcs();
    }
    private:
        class CallData {
            public:
            // Take im the "service" instance (the async server) and the completion queue
            CallData(HelloService::AsyncService* service, ServerCompletionQueue* cq) :
            service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
                //Invoke the serving logic 
                Proceed();
            }

            void Proceed() {
                if (status_ == CREATE) {
                    status_ = PROCESS;
                    // As part of the initial CREATE state, we request that the system 
                    // start processing SayHello requests. In this request, "this" acts as 
                    // the taq uniquely identifying the request (so that different CallData instances
                    // can serve different requests concurrently), in this case the tag is the memory address
                    // of the CallData instance
                    service_->RequestsayHello(&ctx_, &request_, &responder_, cq_, cq_, this);
                } else if (status_ == PROCESS) {
                    // Spawn a new CallData instance to serve new clients while we process
                    // the one for this CallData. The instance will deallocate itself
                    new CallData(service_, cq_);

                    //the actual processing!
                    std::string prefix("Hello ");
                    reply_.set_message(prefix + request_.name());

                    // let the gRPC runtime know we've finished using the memory
                    // address of this instance as the tag for the event
                    status_ = FINISH;
                    responder_.Finish(reply_, Status::OK, this);
                } else {
                    GPR_ASSERT(status_ == FINISH);
                    // Once in the FINISH state, deallocate CallData
                    delete this;
                }
            }

            private:
                HelloService::AsyncService* service_;

                ServerCompletionQueue* cq_;

                ServerContext ctx_;

                Greeting request_;

                GreetingResponse reply_;

                ServerAsyncResponseWriter<GreetingResponse> responder_;

                enum CallStatus { CREATE, PROCESS, FINISH };
                CallStatus status_;
        };

    // this can be run in multiple threads if needed/
    void HandleRpcs() {
        //spawn a new CallData instance to serve new clients.
        new CallData(&service_, cq_.get());
        void* tag; //uniquely identifies a request 
        bool ok;
        while (true) {
            // Block waiting to read the next event from the completion queue.
            // The event is uniquely identified by its tag, which in this case is the 
            // memory address of the CallData instance.
            // The return value of Next should always be checked. This value tells us whether
            // there is any kind of event or cq_ is shutting down
            GPR_ASSERT(cq_->Next(&tag, &ok));
            GPR_ASSERT(ok);
            static_cast<CallData*>(tag)->Proceed();
        }
    }

    std::unique_ptr<ServerCompletionQueue> cq_;
    HelloService::AsyncService service_;
    std::unique_ptr<Server> server_;
};


int main(int argc, char* argv[]) {
    AsyncServer server;
    server.Run();

    return 0;
};

