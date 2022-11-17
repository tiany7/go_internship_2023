
#include <grpcpp/grpcpp.h>

#include "httplib.h"
#include "simple_calculator_demo/simple_adder/adder.grpc.pb.h"

using namespace httplib;

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

int main(void) {
    Server svr;

    //建立链接
    AdderClient adder(
            grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    svr.Get("/add", [&](const Request & req, Response &res) {
        auto a = req.get_param_value("a");
        auto b = req.get_param_value("b");
        int result = adder.AddTwoNumbers(std::stoi(a), std::stoi(b));
        res.set_content("The added result is " + std::to_string(result), "text/plain");
    });

    svr.listen("0.0.0.0", 8080);
}