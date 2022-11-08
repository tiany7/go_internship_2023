## 环境准备

后台开发环境绝大部分时间都是在linux环境下完成的。这就意味着我们将失去图形化界面，只能用命令行和软件进行交互，

所以，本次开发环境的搭建，我们将使用linux系统，为了培养大家摆脱对于图形化界面的依赖，我们将使用远程虚拟机进行开发。

### 虚拟机

本次开发环境如下

- 操作系统：Ubuntu 18.04(Centos 7也可)
- 平台：阿里云， 腾讯云学生套餐(HK区域，否则无法正常安装所需架构)
- 配置：2核 4G内存 10G空间(最低要求)

### 开发工具

- IDE：CLion/Vscode

### 远程连接虚拟机教程

将以为我的虚拟机为例子，其他虚拟机操作类似。我使用的平台为clion，具体操作如下，vscode也可以搞，教程可以百度一下，我不太懂，

#### 前置知识

ssh, smtp, sftp方式建立连接，公钥，私钥， 对称加密 vs 非对称加密

rsa，md5，sha1，sha256，sha512等加密算法在建立可靠连接（https）中的作用

(计算机网络基础，面试大概率会问)


#### 获取公钥

```bash
ssh-keygen -t rsa -C "my_private_key" # 使用RSA算法生成名字教my_private_key的公钥私钥对

cat ~/.ssh/id_rsa.pub # 查看公钥
```

有些服务器使用账号密码登录，这种情况不需要公钥，直接使用账号密码登录即可

有些服务器使用公钥登录，这种情况需要将公钥上传到服务器，然后使用公钥登录，或者有可能直接给你私钥，服务器保留公钥

Anyway，这里我们假设已经获取了私钥，比如在上述命令中生成的id_rsa.pub，会有一个叫做my_private_key的文件，这就是
你的私钥，不要泄露，不要泄露，不要泄露!!!!

#### 建立连接
我用clion链接，所以大致流程如下，记得不要用Community Version，因为Community Version不支持ssh连接
首先找到Tools->Deployment->Configuration，然后点击+号，选择SFTP，然后填写相关信息，如下图

![image](https://user-images.githubusercontent.com/46698520/200650874-78a75d95-928a-4992-a576-00325232537e.png)

![image](https://user-images.githubusercontent.com/46698520/200651232-fabf9df1-3799-4530-8d35-2da19c998a66.png)

然后放进去你的公共ip，端口(一般是22)，用户名(这个你在网站portal一般能够找到)，如果用密码则填入密码，paraphrase为空，如果是私钥则填入私钥在本地
路径，如图所示

![image](https://user-images.githubusercontent.com/46698520/200651443-5c984555-062e-4b46-a100-1615dca988bd.png)

然后点击Test Connection，如果成功则会显示Successfully Connected,如图

![image](https://user-images.githubusercontent.com/46698520/200651767-49d9a937-20cb-4f8b-9291-e5c91e21e712.png)

然后退出来根目录，点击auto detect，就好了，现在就可以开始办公了

![image](https://user-images.githubusercontent.com/46698520/200652246-009cc610-77e1-4900-b988-ecab8a7e143f.png)

#### 建立连接
我们可以开始试着在虚拟机上写代码了，我们首先看命令行，点击tools->deployment->start ssh session，然后选择我们的机器，点击
就可以在Terminal里面看到命令行了，如图

![image](https://user-images.githubusercontent.com/46698520/200653222-0629ac50-2bc7-4486-9e9d-0d750db53fdf.png)

![image](https://user-images.githubusercontent.com/46698520/200653496-1426605b-c84e-4624-9883-beed85e9d658.png)

然后我们可以用图形化界面打开文件，具体方式是Tools->Deployment->Browse Remote Host，然后选择我们的机器，然后就可以看到图形界面了

打开图形界面，我们可以看到我们的文件，如图，注意我们每次修改之后需要保存，然后点击右上角的sync，就可以同步到服务器上了

保存之后，我们可以在命令行里面看到文件的变化，如图

保存前
![image](https://user-images.githubusercontent.com/46698520/200655167-47f8ce35-9e87-4a83-81f2-4c3d58618f9c.png)
保存后
![image](https://user-images.githubusercontent.com/46698520/200655274-3e4c8e7e-d14c-433c-b46d-da3c90024b2d.png)
记得保存哈，vscode不需要每次保存，但是会吃掉机器的内存，各有利弊吧
#### 安装我们需要的前置包
我们需要安装一些包，比如python3，pip3，git，cmake，gcc，g++，这些都是我们需要的，我们可以在命令行里面输入
```
sudo apt-get update && sudo apt-get install -y git g++ make libssl-dev libgflags-dev libprotobuf-dev libprotoc-dev protobuf-compiler libleveldb-dev

sudo apt-get install -y libsnappy-dev

sudo apt-get install -y libgoogle-perftools-dev
```
这些也不是全部，不过暂时够用了，当不够用的时候命令行内会显示出来一些缺少的包，我们再安装就好了

到了现在，就好了，我们可以开始搞了，然后我们搞完之后，来练一练一下基础的命令吧，留作课后作业，这些命令对于工作中排查故障非常重要

### 一些好用的插件
#### OH-MY-ZSH
大家可以安装一下oh-my-zsh，这是一个命令行高亮工具，可以让你的命令行变得更加好看，具体安装方式可以参考这个链接

[安装链接](https://blog.csdn.net/qierkang/article/details/85941316)

安装完他的路径是相对路径，要切换绝对路径，按这么改一下
[切换绝对路径](https://blog.csdn.net/weixin_36429334/article/details/73935272)
#### Z!!!!
这是一个快速切换路径的好东西，你不用再频繁地cd或者cd .. 了，你只需要输入z就可以快速切换到你想要的路径，具体安装方式可以参考这个链接

[安装链接](http://t.zoukankan.com/daodaotest-p-13620965.html)

比如你想要cd /home/xx/yy/zz，你现在只用在/home输入z zz就可以了，非常方便
### 实操lab
先自己尝试完成，如果不会的话可以点开看答案
#### 在~/路径下创建一个名叫work的文档，然后再在里面创建一个名叫test的文档

<details>
<summary>答案</summary>
<pre><code>
mkdir work
cd work
mkdir test
</code></pre>
</details>

#### 在创建好的test文件里创建一个名叫test.txt的文件，然后通过命令行向test.txt追加写入一句话“Hello World”, 再写入"hello world"，然后再通过命令行查看test.txt的内容
<details>
<summary>答案</summary>
<pre><code>
touch test.txt
echo "Hello World" >> test.txt
echo "hello world" >> test.txt
cat test.txt
</code></pre>
</details>

#### 重复上述任务，但要求先grep test.txt里面的Hello, 然后再grep test.txt里面的world
<details>
<summary>答案</summary>
<pre><code>
grep -rn "Hello" | grep "world" 
</code></pre>
</details>

#### 统计一下test.txt里面有多少行
<details>
<summary>答案</summary>
<pre><code>
wc -l test.txt
或者
cat test.txt | wc -l
</code></pre>
</details>

#### 退回到work文件夹，然后要求使用命令行找到包含小写'hello world'的文件内容，要求显示文件名，路径，和所在行数
<details>
<summary>答案</summary>
<pre><code>
grep -rn "hello world" 
</code></pre>
</details>

#### 退回到work文件夹，然后要求使用命令行找到'hello world'(包含大小写)的文件内容，要求显示文件名，路径，和所在行数

<details>
<summary>答案</summary>
<pre><code>
grep -rni "hello world" 
</code></pre>
</details>

#### 从把work文件夹复制一份到~/路径下，然后把复制的文件夹重命名为work2，要求包含所有子文件夹！！
<details>
<summary>答案</summary>
<pre><code>
cp -r work work2
</code></pre>
</details>

#### 删除work文件夹的所有内容
<details>
<summary>答案</summary>
<pre><code>
rm -rf work
</code></pre>
</details>


# 总结
大概到这里就已经完成了基础操作，这些操作需要时间和实践去验证，不过对于本项目来说应该是足够了的。
后续考虑加入奖金机制，比如每周完成度达标则会有小额红包奖励。
