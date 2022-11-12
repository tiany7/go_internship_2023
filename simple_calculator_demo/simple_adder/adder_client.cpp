

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "simple_calculator_demo/simple_adder/adder.grpc.pb.h"
#else
#include "adder.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class AdderClient {
public:
    AdderClient(std::shared_ptr<Channel> channel)
            : stub_(Adder::NewStub(channel)) {}
            
    int AddTwoNumbers(int a, int b) {
        AddTwoNumbersRequest request;
        request.set_a(a);
        request.set_b(b);

        AddTwoNumbersReply reply;

        ClientContext context;

        Status status = stub_->AddTwoNumbers(&context, request, &reply);

        if (status.ok()) {
            return reply.result(); //返回结果
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return -1;
        }
    }

private:
    std::unique_ptr<Adder::Stub> stub_;
};

int main(int argc, char** argv) {


    AdderClient adder(
            grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    int a,b;
    a = atoi(argv[1]);
    b = atoi(argv[2]);
    auto res = adder.AddTwoNumbers(a,b);
    std::cout<<a<<" + "<<b<<" = "<<res<<std::endl;

    return 0;
}