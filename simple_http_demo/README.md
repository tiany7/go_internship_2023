# HTTP 服务
警告，这期内容会稍微提高点难度，看起来不如前几期有趣，因为这些内容大概比较重要，所以请大家务必耐心看完！

这大概是整个后台最重要的概念了，它是整个后台的入口，所有的请求都会经过它，它会根据请求的不同，调用不同的控制器来处理请求。

通常一场面试，面试官会问你，你的项目中有哪些是 HTTP 服务，它们的作用是什么，你是如何实现它们的。并且伴随而来的有很多八股文问题。

HTTP和OS是面试的两大杀手问题

在笔者有限的为数不多的几十场面试里面，大部分失败的面试都是挂在了这个问题及衍生问题上面。因为HTTP服务绝不是一个只靠死记硬背就能去
理解的。有些问题说得再多，没有实操也很难去理解，所以本次我们就来搞一搞。HTTP Webserver是后端经典项目，开发周期大概为1-2月，所以这里我们不会涉及怎么去实现一个http webserver，这个问题留给大家。

但我希望明年春招之前，各位都能自己去试着实现一遍webserver，然后把这个项目写在简历上，这样基本能秒杀大部分面试了

本次我们来体验一下HTTP服务，之后基本就能和我前面教你们的串上了

想要真正理解，只有实操也是不太够的，所以后面会布置一些八股问题，大家找到所有答案之后把可以把答案发给甜酱，第一个发的有
10元小额红包作为奖励

## 关于本次lab

本次lab将使用Httplib开源框架作为Webserver的实现，之所以选用这个是因为它的使用非常简单，只有一个头文件，
我们只需要include它就好。

注意，httplib采用了SELECT作为IO多路复用方式，这不是一种很高效的实现。所以大家在实际项目中基本碰不到这种实现方式，但是它的使用非常简单，所以我们就用它来体验一下HTTP服务吧

## Acknowledgment

本次lab的代码来自于 [HTTPLIB](https://github.com/yhirose/cpp-httplib) 的example，感谢作者的开源精神！

## 1. Hello World

我们来实现一个简单的hello world服务

### 1.1 配置依赖

如上文所说，httplib只需要配置一个头文件

![image](https://user-images.githubusercontent.com/46698520/202537714-63626a59-8aba-41eb-82b4-a12bcf016bc0.png)

比如这里，为了方便起见，我把它放进了我们的comm_github_mysql_cpp_connector的git_repositories里面了，大家只需要在dep里面加上这一行就好了

![image](https://user-images.githubusercontent.com/46698520/202540231-46791c76-9f59-44fb-903a-c59c95b03139.png)

然后我们来搭建一个简单的服务

### 1.2 实现服务

首先我们先写cpp file

```cpp

#include "httplib.h"
using namespace httplib;

int main(void) {
    Server svr;

    svr.Get("/hi", [](const Request & /*req*/, Response &res) {
    res.set_content("Hello World!", "text/plain");
});

svr.listen("0.0.0.0", 8080);
}
```

这个代码的意思就是在8080端口上监听，当有人访问/hi的时候，返回一个Hello World!的字符串

这个text/plain是告诉对面我们返回来的是文字，这个地方规定了浏览器如何渲染返回信息的方式

### 1.3 编译运行

我们来配置下BAZEL

```python
cc_binary(
    name = "hello_server",
    srcs = ["http_server.cpp"],
    defines = ["CPPHTTPLIB_OPENSSL_SUPPORT"],
    visibility = ["//visibility:public"],
    deps = ["@comm_github_mysql_cpp_connector//:httplib",
            "@com_github_grpc_grpc//:grpc++",
            "@com_github_grpc_grpc//:grpc++_reflection",
            "//simple_calculator_demo/simple_adder:adder_cc_grpc"],
    linkopts = ["-lpthread"],
)
```

无奖竞猜，为什么要带上我们之前写的adder_cc_grpc？

答：因为grpc里面自带了openssl，include了grpc就相当于配置好了openssl，这样我们就不用多花时间去配置openssl了

然后我们来编译运行

```bash
bazel build //http_server:hello_server
#切换到bazel-bin目录
./hello_server
```

### 1.4 测试
我们可以在本地测试一下，在命令行输入
    
```bash
curl localhost:8080/hi
```

你会收到对方的回应

![image](https://user-images.githubusercontent.com/46698520/202551071-b16649b8-5a0c-484f-a919-76cece266ec4.png)

大家现在应该对http有个初步的认识了吧？ 我们使用的微信，B站中，客户端和前端会大量调用后端提供的HTTP接口获取信息，这些信息都是通过HTTP协议传输的。

还有个点，为什么我们要通过curl 8080端口来访问服务，一般我们访问百度都是比如 www.baidu.com/xx 的这种？

实际上，我们在浏览器中输入 www.baidu.com/xx 的时候，浏览器会自动帮我们把这个请求转换成了<目标机器公共IP>:8080/xx的形式，然后再去访问我们的服务，这个过程叫做DNS解析。

DNS解析的过程，递归和迭代方式的区别，这是道很经典的八股题，课后作业++

然后我们如何在外网或者浏览器访问这个呢？

云服务器设置的有安全组，默认所有端口都是不能被inbound流量访问的，这样会带来安全问题。不过我们可以手动开启。

在自己的云服务页面，打开安全组设置，添加你需要开放的端口，比如8080，然后保存，这样就能被外网访问了。

我们可以通过浏览器（<公共ip>:port/hi）访问我们的服务

![image](https://user-images.githubusercontent.com/46698520/202557088-3b3fc9a1-f822-4d7c-8aa9-a7071d329576.png)

也可以通过jetbrain自带的http工具访问

打开tools->http client

![image](https://user-images.githubusercontent.com/46698520/202557212-085df24f-8306-4e05-904f-85f096e40854.png)

然后

![image](https://user-images.githubusercontent.com/46698520/202557339-2f7f86da-4da5-4e7d-890c-955c6a8c4997.png)

这样就可以看到结果了，往后的http服务测试我们主要用后者


## 2. A + B Problem Again

我们知道了如何搭建一个简单的HTTP服务，那么我们就可以来实现我们的A + B Problem了

以前我们的A+B Problem是这样的, GRPC客户端去调用服务端，服务端返回结果，客户端展示

但是这里有个问题哈，在生产环境里，我们不能让客户接触到代码，更不可能让他们通过客户端代码去获取信息，那么怎么办呢？

于是乎，我们可以在RPC服务端外面套一层HTTP服务，这样客户端就可以通过HTTP的方式去访问我们的服务了，RPC只用作内部服务之间通信。这就是后端的雏形了。

有没有感觉对后端清晰很多？

### 2.1 服务端

然后我们来看看怎么实现这个服务端，adder我们实现好了，这里面只需要把adder的的客户端代码搬过来就行了，然后在HTTP服务里面加一个
parse参数的步骤，httplib提供的有get_param_value函数，传回来的是string值，自行解析即可，然后解析完毕，调用adder的客户端代码就行了，返回去就行了。

那么http是如何绑定参数的

一个带参数的http请求长这个样子


```bash
GET <public IP>:<端口>/<标签>?key1=value1&key2=value2... HTTP/1.1
Host: 
User-Agent: curl/7.64.1
Accept: */*
```

大概是这种形式来进行交互，Accept的意思是接收什么类型的数据，这里我们用*表示接收所有类型的数据，有些接口只接受text/plain，有些接受application/json，有些接受application/xml，这个是根据接口的设计来的。

这里不赘述

我们可以开始实现我们的A + B problem了

### 2.2 客户端

我们把adder client去掉main搬过来，然后加入http的解析，然后调用adder的客户端代码，这里服务这样写

```cpp

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
```

然后我们分别开两个窗口，先跑add服务，然后再跑新写的http服务，去curl一下这个服务，看看效果。

```bash
cd bazel-bin/simple_calculator_demo/simple_adder
./adder_server
cd bazel-bin/simple_http_demo/
./hello_server
```

然后我们去curl一下

```bash
curl http://localhost:8080/add?a=111&b=222
```

可以看到结果了

![image](https://user-images.githubusercontent.com/46698520/202569799-15bcd910-b50b-4f01-9dae-1398186c701b.png)

是不是很简单呢？

好了，你已经见识了http的基本服务流程，这里我们只是简单的实现了一个加法，如果你想要实现更多的功能，可以自己动手

# 作业题

## 编程题

实现一个grpc + http服务，http服务path叫solve, 服务内容是算法题 [Almost Prime](https://codeforces.com/problemset/problem/26/A) ，输入一个整数，然后返回答案。

在这里你需要输入数据，然后经过http - rpc返回答案。

## 八股

1. http的基本流程吗？
2. HTTP建立链接的过程是怎么样的？详细描述下》
3. 为什么说HTTP是无状态(state less)的？
4. http都有哪几种请求方法？
5. POST和GET方法都有什么区别？(至少6条)
6. 什么时候用GET？什么时候用POST，请举出具体例子？
7. HTTP位于哪一层？TCP位于哪一层？
8. HTTP的状态码有哪些？给出三个常见的状态码的含义。
9. cookie和session机制是什么？有什么区别？
10. HTTP1.1的新特性？(三个)
