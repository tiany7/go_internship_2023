# Docker浅入门

## 什么是Docker?

在实际开发中，有一类问题最令人讨厌，那就是环境问题。

大家的操作系统，软件版本，配置等等都不一样，
这些都会导致环境问题的出现。这种问题没有一个统一的表现形式，千奇百怪的错误信息可能会让你感到无从下手，而且这种问题往往会在你最不想遇到的时候出现。
处理这些问题往往要花上几天，一个月，甚至更长的时间，否则你的代码跑不通，后续开发无从谈起。
要想让很多人去开发同一份代码，解决环境问题是必不可少的。

那么有没有一种方法可以解决这个问题呢？

首先我们考虑用一下，每个人都用完全一样的虚拟机开发可不可以？
答案显而易见是不可行的，因为虚拟机太大了，而且启动速度太慢了。而且这个沟通的过程太难协调了，比如xx软件和yy软件可能需要的版本都不一样，
zz软件可能和在用的软件都不兼容，我们可能就需要同时启动两台甚至更多机器，这样非常笨拙，而且利用率不高。大家可能也能感觉到，虚拟机是非常昂贵的，这么开下去，公司迟早要破产。

或者我们也可以想到一个方法，约定一个方式，使得每个人通过固定的查找方法，可以访问同一个环境，这样就可以避免环境问题了。

于是乎我们就有了Docker

大家可以把Docker理解为小型的虚拟机，我们只需要把我们的开发环境打包成一个镜像，比如你现在正在用的Ubuntu系统，实际就是一个大号的docker
只不过运行在这台虚拟机的硬件上面，那么docker就是在这个虚拟机里再建一个虚拟机。这个虚拟机里面的环境就是我们的开发环境，比如我们需要的软件，配置等等。
相比于多开虚拟机，这个方法显然更容易接受，代价更小。而且到后面大家会发现，docker平台具备高度可移植性。

### NOTICE
本次的讲解内容有点大

## Docker的安装

### 安装依赖

```bash
sudo apt-get install apt-transport-https ca-certificates curl gnupg-agent software-properties-common -y
```

### 添加GPG key

```bash
sudo curl –fsSL https://download.docker.com/linux/ubuntu/gpg | apt-key add -
```

### 添加Docker fingerprint

```bash
apt-key fingerprint 0EBFCD88

add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"
```

### 安装Docker

```bash
sudo apt-get install docker-ce docker-ce-cli  containerd.io -y
```

### 测试Docker

这一步我们已经完成了Docker的安装，我们可以通过下面的命令来测试一下

```bash
docker --version
```

如果出现了版本信息，则docker安装成功

## Hello World

我们可以通过下面的命令来运行一个hello world的docker
    
```bash
docker pull hello-world
```

然后我们查看一下docker的镜像

```bash
docker images
```

可以看到我们刚刚下载的hello-world镜像

然后我们运行一下这个镜像

```bash
docker run hello-world
```

然后虚拟机console会出现下面的信息

```bash
Hello from Docker!
This message shows that your installation appears to be working correctly.
```

大家刚开始看可能有点疑惑，你先慢点，这个怎么就hello-world了？这个东西是怎么跑起来的呢？

这个的意思是，我们通过docker pull命令，从docker hub上面拉取了一个hello-world的镜像（微型操作系统环境）到本地，然后
我们跑了一下这个操作系统，操作系统里面的程序就是hello world，所以打印输出了控制台内容。

我知道这里有些难理解，但是大家不要着急，后面我们会慢慢的理解这个过程。我们不妨先把hello-world代码替换成我们自己要跑的代码。
那么这个image（镜像）的作用就是装好一切我们需要的环境，包括程序本身，然后我们跑这个代码也就是安装程序 + 运行程序的过程了。
你只要装了这么一个docker的控制台，甭管你是在windows还是linux还是mac，你都可以跑这个程序了。

当然，这个只是应用层面的，具体的docker镜像是怎么构建的，我们下面就来讲。
## 手动写一个hello-world文档

Dockerfile是一个文本文件，里面包含了一条条的指令，这些指令就是用来构建docker镜像的。我们可以通过下面的命令来创建一个Dockerfile

比如上面的hello world，我们可以通过下面的命令来创建一个Dockerfile

比如要创建一个ubuntu的镜像，然后在里面安装g++，我们可以通过下面的命令来创建一个Dockerfile

```bash
touch Dockerfile
```

输出hello-world有很多方式，我们选取一个比较简单的方式，就是通过python print来输出hello world

我们写一个简单python脚本，注意这个脚本的文件名必须是hello.py，
然后在Dockerfile同级目录下面

```python3
print("hello world")
```


```bash
FROM ubuntu:18.04 # 基础镜像
RUN apt-get install python3-pip -y # 安装python3
RUN pip3 install --upgrade pip # 更新pip
RUN apt-get install -y python3 # 安装python3
COPY ./echo.py  /app/echo.py # 将当前目录下的echo.py拷贝到镜像的/app/echo.py
CMD ["python3", "/app/echo.py"] # 运行python3 /app/echo.py
```




这个Dockerfile的意思是，我们要从ubuntu:18.04这个镜像开始，然后把当前目录下的echo.py拷贝到镜像的/app/echo.py，
然后运行python3 /app/echo.py，这样就实现了一个最简单的hello world

然后我们通过下面的命令来构建这个镜像

```bash
docker build -t my_image .
```

这个命令的意思是，my_image的镜像，然后我们把当前目录下的Dockerfile作为构建的依据。
这个.可以替换为其他路径，如果你的dockerfile不叫Dockerfile，那么你可以通过-f来指定dockerfile的名字

```bash
docker build -t my_image -f my_dockerfile .
```


然后我们通过下面的命令来查看一下我们构建的镜像

```bash
docker images
```

可以看到我们刚刚构建的ubuntu-g++镜像
![image](https://user-images.githubusercontent.com/46698520/206888071-5f5cabdb-5dbc-4c2d-8533-83f464c67a06.png)

然后我们通过下面的命令来运行这个镜像

```bash
docker run my_image
```
然后我们可以看到输出了hello world

![image](https://user-images.githubusercontent.com/46698520/206888105-a38ce22d-de9e-4b93-846b-896b242c0da6.png)

这个镜像在跑，但是由于我们没有指定具体的命令，所以它会直接退出，我们可以通过下面的命令来查看一下这个镜像的状态

```bash
docker ps -a
```

我们看到了这个

![image](https://user-images.githubusercontent.com/46698520/206888133-4ae305c2-fe62-497a-8100-3e445f4ca9e9.png)

Exited，很怪，因为我们不是刚run过么？怎么exit了？

其实是因为docker在执行完具体的命令之后，和cpp的二进制文件一样，会自己退出，一般的服务是会阻塞的，然而这些简单任务就会执行完毕，就会自动退出

因为镜像很占空间，所以我们可以通过下面的命令来删除这个镜像

```bash
docker rmi my_image
```


![image](https://user-images.githubusercontent.com/46698520/206887223-3d3e6c19-028c-41a5-8b38-0678853f5852.png)

然后大家发现了没有，这个ubuntu18:04, 其实也是个镜像。。。。

所以我们就可以套娃，在别人的镜像上面安装别的东西，然后再构建镜像，然后再安装别的东西。。。。。

这么一来，我们就避免了重复造轮子的问题。

# 思考作业

本章节的思考作业是，你能不能构建一个镜像，这个镜像首先用g++编译一个c++文件，
这个c++文件从**标准输入**读入两个数字A和B，打印出来A+B的结果

参考文献：[Docker文档](https://docs.docker.com/)

答案谜底会在下一章节公布

主要是为了锻炼下大家读文档的能力，这个能力在工作中至关重要，保持一个良好的消息接受渠道，
是任何一个工程师的基本素养

加油！