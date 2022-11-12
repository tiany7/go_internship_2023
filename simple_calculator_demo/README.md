# 一个简单的微服务demo

## 前情提要
这些布置的任务并不是独立的，会在之后的某些时刻串起来，微服务也是这样，实际是由很多的服务组成的，里面涉及到很多层的设计。
刚才说的hello world项目只是练手，让大家知道有这么一个东西。具体怎么用，大家之前在学校学的东西又以什么样的形式实现，后面会有所介绍的

但同时，动手的过程本身也是一个学习的过程，我不想过多地干预，代替大家思考。让大家自己去摸索，这样才能真正的学会。
我的职责是作为一个忠实的问答机器，帮助大家解决问题，理解和学习，

### 1. 书接上回

让我们重新认识一下上次的proto文件，其实刚开始学，我们不必对着这些内容去追求每一个细节，我们只需要知道这些东西是如何映射到我们的代码中的就可以了。

所以首先我们需要看一下我们的proto文件是如何映射到client和server代码中去的。

我们来看下helloworld.proto文件

```proto
syntax = "proto3";

package helloworld;

// The greeting service definition.
service Greeter {
  // Sends a greeting
  rpc SayHello (HelloRequest) returns (HelloReply) {}
}

// The request message containing the user's name.
message HelloRequest {
  string name = 1;
}

// The response message containing the greetings
message HelloReply {
  string message = 1;
}
```

#### package

首先package就是我们的的包名，对应到cpp就是命名空间

#### service

service就是我们的服务，这里定义的service是指，我有一个叫做Greeter的服务，这个服务有一个叫做SayHello的方法，这个方法有一个参数HelloRequest，返回值是HelloReply

我实现SayHello的方式就是进来的请求是HelloRequest，我返回的是HelloReply
我们从HelloRequest获取信息，然后处理完信息，返回到HelloReply中去,如此依赖，我们就实现了一个服务。

#### Tips
因为我们从proto文件到cpp文件的编译是自动的，那么其中必然包含了我们可以指定的部分和自动生成的部分

比如在下面一个类的定义里面

```cpp
GreeterServiceImpl
```

翻看proto文件容易得知，我们的服务名是Greeter，那么我们的实现类就是GreeterServiceImpl

那么 Greeter是我们可以指定的信息，而ServiceImpl是自动生成的信息。

按照这个逻辑，我们可以推断出，有哪些东西是需要换的，有哪些东西是不需要换的

### 2. A + B Problem

我们现在来写一个简单的计算器，实现两个数的加法和减法。这里会给出教程，大家可以跟着走一遍。

课后作业是写一个多个数的加法。

#### 2.1 proto文件

首先我们需要定义我们的proto文件

服务名字：

Adder

方法名字：

AddTwoNumbers

参数：

AddTwoNumbersRequest

返回值：

AddTwoNumbersReply


```proto    
syntax = "proto3";

package adder;

service Adder {
  rpc AddTwoNumbers (AddTwoNumbersRequest) returns (AddTwoNumbersReply) {}
}

message AddTwoNumbersRequest {
  int32 a = 1;
  int32 b = 2;
}

message AddTwoNumbersReply {
  int32 result = 1;
}
```

为了简单起见我把命名空间给移除了，但注意这在工程里并不算是个好习惯

#### 2.2 生成cpp文件

我们把根据Hello World的教程，写出来server的代码，如下图所示

```cpp


#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#ifdef BAZEL_BUILD
#include "simple_calculator_demo/simple_adder/adder.grpc.pb.h"
#else
#include "adder.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;


// Logic and data behind the server's behavior.
class AdderServiceImpl final : public Adder::Service {
    Status AddTwoNumbers(ServerContext* context, const AddTwoNumbersRequest* request,
                         AddTwoNumbersReply* reply) override {
        int a = request->a();
        int b = request->b();
        int sum = a + b;
        reply->set_result(sum);
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    AdderServiceImpl service;

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();

    return 0;
}
```

#### 2.3 生成客户端

我们依葫芦画瓢，写出来客户端代码

```cpp


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

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    int AddTwoNumbers(int a, int b) {
        // Data we are sending to the server.
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
    b = atoi(argv[2]); //从命令行读取数据
    auto res = adder.AddTwoNumbers(a,b);
    std::cout<<a<<" + "<<b<<" = "<<res<<std::endl;

    return 0;
}
```

然后写一下我们的BUILD文件

```python
package(default_visibility = ["//visibility:public"])

load("@rules_proto//proto:defs.bzl", "proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")


proto_library(
    name = "adder_proto",
    srcs = ["adder.proto"],
)

cc_proto_library(
    name = "adder_cc_proto",
    deps = [":adder_proto"],
)

cc_grpc_library(
    name = "adder_cc_grpc",
    srcs = [":adder_proto"],
    grpc_only = True,
    deps = [":adder_cc_proto"],
)

cc_binary(
    name = "adder_client",
    srcs = ["adder_client.cpp"],
    defines = ["BAZEL_BUILD"],
    deps = [":adder_cc_grpc",
            "@com_github_grpc_grpc//:grpc++",],
)

cc_binary(
    name = "adder_server",
    srcs = ["adder_service.cpp"],
    defines = ["BAZEL_BUILD"],
    deps = [":adder_cc_grpc",
            "@com_github_grpc_grpc//:grpc++",
            "@com_github_grpc_grpc//:grpc++_reflection",],
)
```

那么我们现在就可以到/bazel-bin 里顺序执行adder和adder_client了

![image](https://user-images.githubusercontent.com/46698520/201495886-a94ac39e-5a40-4b2b-9874-7b3a96b23729.png)

我们可以看到结果了，如此依赖我们就实现了一个简单的RPC服务

那么在工作中，我们的服务是由成百上千个这种微服务单元组成的，比如别的组需要计算a + b, 我们会事先约定接口，对方
调我们的AddTwoNumbers接口，我们返回结果，这样就实现了一个简单功能

### 3. 关于GRPC

message里面有很多支持的类型，包括常见的vector，map，bool都可以在message里面定义。
如果对类型有疑问，可以参考这个网站

[protobuf类型支持大全](https://developers.google.com/protocol-buffers/docs/proto3)

### 4. repeated类型

repeated类型本质是一个动态数组，和vector类似

在proto3之前的proto2版本中，其实在每一个message中，每一个属性是分为required，optional和repeated类型

比如我们实现一个学生的proto message，在proto3中是这样

```protobuf
syntax = "proto3";
message Student {
    string name = 1;
    int32 age = 2;
    string address = 3;
    repeated string courses = 4;
}
```

在proto2中是这样

```protobuf
syntax = "proto2";
message Student {
    required string name = 1;
    optional int32 age = 2;
    optional string address = 3;
    repeated string courses = 4;
}
```

repeated类型用法类似于vector，操作也类似，比如上面的例子，我们可以这样操作

```cpp
Student stu;
stu.add_courses("math");
stu.add_courses("english");
stu.add_courses("chinese");
```

这样就实现了一个简单的push_back

那么我们读取repeated类型信息的时候，怎么知道其中有多少门课呢？
```cpp
Student stu;
stu.course_size(); //这就是查看大小的方法
```

那么我们怎么读取其中的每一门课呢？
```cpp
Student stu;
for(int i = 0; i < stu.course_size(); i++){
    std::cout<<stu.courses(i)<<std::endl;
}
//又或者
for(auto course : stu.courses()){
    std::cout<<course<<std::endl; //只读
}

//又或者
for(auto& course : stu.courses()){
    course = "math"; //读写
}

读取raw pointer

for(const auto & course : *stu.mutable_courses()){
    std::cout<<course<<std::endl;
}
```
有没有很神奇？

# 作业

1. 请在同一份文件中实现一个简单的RPC服务，实现一个实现多个数字的加法的计算器，比如输入1,2,3,4,5,6,7,8,9,10，返回55

方法名：AddNumbers

request: AddNumbersRequest

response: AddNumbersResponse


2. 请在同一份文件中实现一个简单的RPC服务，批量处理AddTwoNumbersRequest，比如输入(1, 1), (2, 2), (3, 3)，返回(2, 4, 6)

方法名：AddTwoNumbersBatch

request: AddTwoNumbersBatchRequest

response: AddTwoNumbersBatchResponse

Client和Service文件可以用原来

然后我给大家留一些时间吧，暂时不放答案了，大家尽量自己先想，遇到不会的问题可以百度或者问我，
下周四之前我们再来讨论一下，我会放出答案。

学习嘛，就是这个样子。说的记的其实不太能记住，但是亲手解决问题，一辈子都不会忘记。

加油！！

大家可以背八股了。

# 前情提要
下周我们学习一下http相关的东西，这是面试的重中之重了。

然后到下下周我们开始学习云服务和搭建docker，k8s和部署数据库，这些都是后台的基本功。之后内容应该就能串起来了！
