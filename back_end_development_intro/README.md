# 跑一个简单的rpc服务

本lab不涉及任何代码改动，只是想让大家体验一下rpc的调用过程。

我们日常生活中会涉及到大量的询问，比如你问Z聚，这道题怎么写，Z聚会回答blabla
其实RPC就是这样的一个过程，你问Z聚，这道题怎么写，Z聚会回答blabla，只不过计算机
更严谨一点，双方需要约定沟通的方式，然后通过网络建立tcp/http链接，最终问问题的人把
问题发到被问问题的人，然后被问问题的人回答问题，最后把答案发回去。

那么我们不妨通过下面一个简单的grpc例子来体验一下这个过程是如何被计算机执行的

## 1. 安装bazel

bazel和cmake都是我们交互的方式，bazel是google开源的，cmake是我们常用的，这里我们用bazel，
因为bazel比cmake还是要好用一点，不过bazel的安装和调试网络上信息比较少，所以这里如果用cmake也行，
具体可以问devour爷，他比较熟悉。bazel问甜酱，cmake问devour爷。

首先安装bazel，记得安装版本为4.0.0，否则无法正常编译

教程: [安装教程](https://bazel.build/install/ubuntu)

一切按教程走，唯一需要修改的地方就是，我们要指定安装版本，在install的时候

```bash
sudo apt-get install bazel-4.0.0

sudo ln -s /usr/bin/bazel-3.0.0 /usr/bin/bazel #建立软链接，以后用bazel调用的就是4.0.0了
```

## 2. 安装grpc

grpc是google开源的rpc框架，我们这里用它来实现rpc的功能.

根据这篇安装教程安装一下

[安装教程](https://grpc.io/docs/languages/cpp/quickstart/)

注意在make -j的时候，我们不能加-j，因为我们的机器内存不够，加了-j会多线程并行下载，导致内存不够，编译失败

## 3. 编译运行

让我们把视线移到proto上面，这个文件在/grpc/examples/cpp/helloworld/下面，这个文件是我们rpc的协议文件，我们需要约定好这个文件，然后双方都需要遵守这个协议，这样才能正常通信。
回到Z聚的对话问题上，proto文件就是我们问z聚的方式，message是我们说出来的话和Z聚回答我们的格式
service是一个function，说我们说什么样的话，Z聚以怎样的形式回复我们，这个function是通过网络进行通信的
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
我们可以看到，这里定义了一个服务，服务里面有一个rpc函数，这个函数的输入是HelloRequest，输出是HelloReply
定义了一个service，就是我们要say hello，我们希望客户装填hello request内为string的变量，即自己要说的话，
然后服务端会返回一个reply，即用户这句话的内容。

proto文件经过protobuffer编译器编译之后可以生成很多语言版本的文件，所以proto是语言无关的，我们可以用任何语言来实现这个服务，只要我们都遵守这个协议就行了。

每个message是个struct访问的话可以看到，有一个string类型的变量，可以通过xx.message()进行const访问，也可以通过xx.set_message("xx")进行修改

这，就是一个简单的rpc服务，我们可以通过这个服务来实现我们的对话问题。这也就是后端最基础的工作。

package就是生成代码的namespace

## 4. 代码运行

大家可以在grpc/examples/cpp/helloworld/下面找到我们的代码，我们的代码是在greeter_client.cc和greeter_server.cc里面
编译是写在了同文件目录的BUILD文件里面，大家可以看一下，这里就不赘述了, cc_binary指的是我们生成可执行二进制文件，cc_library指的是我们生成的库文件，
库文件不能被直接执行，我们在acm里搞的cpp文件都是以二进制文件形式被测评的

bazel还需要有workspace，在/grpc目录可以看到，这个是什么大家暂时不用了解，但是只需要知道，WORKSPACE定义了每个项目的根目录在哪里

我们一切的引用，比如/根目录/work， 在bazel里面都是//work

然后bazel还支持从github直接拉代码下来到我们的项目里，这个后面会介绍，这里先不说

然后我们可以开始编译greeter_server和greeter_client了

在根目录里

```bash
bazel build //examples/cpp/helloworld:greeter_server
bazel build //examples/cpp/helloworld:greeter_client
```

或者到examples/cpp/helloworld/目录下

```bash
bazel build :greeter_server
bazel build :greeter_client
```

## 5. 执行

我们所有被编译好的代码都会被放到bazel-bin/目录下，我们可以在这个目录下找到我们的可执行文件

```bash
bazel-bin/examples/cpp/helloworld/greeter_server
bazel-bin/examples/cpp/helloworld/greeter_client
```
我们可以看到，我们的服务端和客户端都已经被编译好了，我们可以执行它们了

```bash
bazel-bin/examples/cpp/helloworld/greeter_server
bazel-bin/examples/cpp/helloworld/greeter_client
```
上述代码记得分两个terminal执行，然后我们就可以看道打印出来的结果了

## 6. 代码分析
大家可以学习一下代码的写法，现在的理解可以不用到理论层面，只需要会用就行，后面会有更多的内容来讲解

# 简易安装版

考虑到大家的预算有点紧张，所以有个可以bypass内存限制的方法，就是使用bazel自带的http_archive包管理
和git_repository包管理，这样我们就可以直接从github上拉取代码下来，然后编译，这样本地除了bazel什么都不用安装

文件主要放在了workspace里面，大家可以看一下，具体功能和用法留给大家自己思考。

http_archive和git_repository的主要生效的地方在于，我们在BUILD文件里面的引用，比如我们在BUILD文件里面引用了

假如我们在BUILD文件里面引用了一个文件叫//protos/helloworld.proto的依赖, 我们不能保证这个proto文件在本地，
所以我们可以把这个proto文件放进一个git项目里，只需要填入git的地址，然后在BUILD文件里面引用就可以了

比如之前我们会在dep栏里引用protos/helloworld.proto，现在我们可以直接在BUILD文件里面引用

```python
proto_library(
    name = "helloworld_proto",
    srcs = ["protos/helloworld.proto"],
    visibility = ["//visibility:public"],
)
```

现在我们把proto文件放到github上一个叫my_dep的repo一级目录里面，然后在BUILD文件里面引用，这样我们就可以直接从github上拉取代码下来了

```python
git_repository(
    name = "my_dependency",
    remote = "https://github.com/tiany7/my_dep.git",
    commit = "32094895286f499f997bef4324d68f61ea90a41f",
)
```

现在我们不需要指定文件本地地址，我们只要指定git + 相对路径就好了

```python
proto_library(
    name = "helloworld_proto",
    srcs = ["@my_dependency//:helloworld.proto"], # 这里的@my_dependency就是git_repository里面的name
    visibility = ["//visibility:public"],
)
```

## 操作流程
首先安装好bazel其实什么事情都不用干，然后我们实现上述的hello_world程序

我已经写好了，在./demo里面，大家首先进去demo,然后开始编译
    
```bash
    cd demo
    bazel build //:greeter_server
    bazel build //:greeter_client
```

然后在bazel-bin里运行就好了，无需任何额外操作哈


## 7. 思考作业题
实现一个简单的adder，实现两个数相加，然后返回结果
代码可以在server和client模型上改，试试看如何改，不过这是下次lab的要求，不是这次的要求

