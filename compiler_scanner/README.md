# 深入理解编译原理
大家平时用cpp比较多。

通常来讲，我们使用的方式就是写进ide，然后编译，运行，做事情+打印结果。

但是大家有没有想过，这个过程在底层是怎么实现的呢？一些奇技淫巧般的操作，编译器究竟是怎么弄清楚你的意思，知道
你要表达什么的呢？

这里我并不是提倡反复造轮子。我们做开发，本就是站在前人的肩膀上，反复造轮子，对源码死扣
往往会耽误很多本可以用在正事上的时间，但是，对于某些语言的新特性
比如cpp17之后推出的模板元编程概念，或者是老掉牙的八股，比如cpp的内存管理，这些东西，从底层出发
无疑会拓宽我们的思路，让我们对语言的理解更加深刻。

当然，从0开始的话是不太可能的，所以我会提供一些模板，最终的目的是大家能够通过模板
那么在这里的话，我的计划是大家可以从底层开始，逐渐给编译器加上一些功能，

那么语言选取上，一般学校会选取C，但是C不够简洁，并且过于自由的内存管理会让我们把大量时间投入到和segfault作斗争上。

我们希望找到一种语言，能够让我们更加专注于语言的本质，而且限制需要比较严格，这样我们的代码一旦可以编译，很大概率是可以正常跑的。

那么OCaml就是一个很好的选择。


## OCaml 语言
可能和大家常用的cpp不一样，cpp，java属于是imperative language，而OCaml是functional language(函数式语言)。

最为显而易见的区别就是，在函数式语言中，只会提供简单的数据结构，比如list，array，tuple，map，set等等。
而去遍历这些数据结构，我们需要使用递归的方式，而不是像imperative language那样，提供for，while，do while等等。

平时写dfs都不是难事，我相信ocaml的递归也不会让大家觉得难以接受。

举个最简单的例子，斐波那契数列，我们只能这样写：

```ocaml
let rec fib n =
    if n <= 1 then 1
    else fib (n - 1) + fib (n - 2)
```

而不能这样写：

这个let表示声明，而rec则表示该函数是递归的。fib n表示函数名为fib，参数为n。

当然，你可以加括号，或者指定类型，但是这些都是不必要的。
    
```ocaml
let rec fib (n: int) =
    if n <= 1 then 1
    else fib (n - 1) + fib (n - 2)
```

那么为什么要这样写呢？因为在函数式语言中，我们不会去改变变量的值，而是通过函数的返回值来改变变量的值。

这就叫做immutability(不可变性), 所以这就引入了一个新的概念，叫做side effect(副作用)。

副作用就是指，函数的返回值不仅仅取决于函数的参数，还取决于函数的执行过程中，对外部变量的影响。如果一个函数本身
影响到了外部变量，那么这个函数就是有副作用的。所以函数式编程很好地规避了这一点。

比如我们如果想在函数外面声明一个外部变量，我们可以使用in关键字
    
```ocaml
let fib n =
    let ans = ref 0 in
    let rec helper n =
        if n <= 1 then 1
        else helper (n - 1) + helper (n - 2)
    in
    helper n
```

这里的ref表示引用，也就是一个指针，ans指向了一个int类型的值，初始值为0。
不过大家放心，这只是为大家展示一下，实际上我们不会这么写，因为这样写的话，可能就有些太难了。

过多的介绍可能会让大家觉得很难，所以我就不多说了，大家可以自己去看看ocaml的[官方文档](https://ocaml.org/learn/tutorials/99problems.html)。

然后我们来写一些题目，看看ocaml的风格。

这里的题目我直接把答案给出来了，大家可以自己去写一下，玩一玩，确保基本操作能够理解。

## 安装OCaml

- 推荐环境: ubuntu 18.04 LTS
- 安装OCaml: sudo apt install ocaml
- 链接: [ocaml 安装教程]（https://www.ocaml.org/docs/up-and-running#Ubuntu-Ubuntu-18-10）

安装完然后输入ocamlbuild或者which ocamlbuild，如果有输出，说明安装成功。

如果没有安装成功，可以试一下添加环境变量
    
```bash
export PATH=$PATH:~/.opam/bin
```


然后我们在这里的话其实不需要了解什么特别高大上的feature，暂时也不需要

我们只需要知道，我们常用的就是let，if，else和match关键字，以及一些常用的容器

### let关键字

let关键字用来声明变量，比如我们想声明一个变量x，那么我们可以这样写

```ocaml
let x = 1
```

如果我们想声明一个函数，那么我们可以这样写

```ocaml
let f x = x + 1
```

我们如何去调用这个函数呢？我们可以这样写

```ocaml
f 1
```

结合起来，我们给一个值y赋值为f(1)，那么y的值就是2了。

```ocaml
let y = f 1
```

这挺简单的

### if else关键字

if else关键字用来做条件判断，比如我们想判断一个数是不是偶数，那么我们可以这样写

```ocaml
let is_even x =
    if x mod 2 = 0 then true
    else false
```

这里的mod是取模运算，比如1 mod 2 = 1，2 mod 2 = 0，3 mod 2 = 1，4 mod 2 = 0
这个不用多说

### match关键字

match关键字用来做元组的匹配

比如我们现在有一个函数f，输入参数是x，但是这个x它可以是一个元组，比如(1,2)，(1,2,3)，(1,2,3,4)等等
我们当然是希望这个函数能够通过某种方式去实现对输入参数的匹配，然后返回一个值

这个时候我们就可以引入我们的match关键字了

比如我们要支持对2-4大小元组的加法，返回一个int，那么我们可以做出类似于cpp的initializer_list的效果

```ocaml
let f x =
    match x with
    | (a, b) -> a + b
    | (a, b, c) -> a + b + c
    | (a, b, c, d) -> a + b + c + d
```

这里的|是用来分割不同的case的，比如上面的代码，我们就是用来分割(a,b)、(a,b,c)、(a,b,c,d)这三种情况的

然后比如我们要实现一个函数，这个函数接收很多类型的参数，根据不同的参数类型，返回不同的值，那么我们可以这样写

```ocaml
let f x =
    match x with
    | 1 -> 1
    | "hello" -> "world"
    | (a, b) -> a + b
    | _ -> 0
```

这样也是允许的，ocaml的match关键字是支持任意类型的。非常强大，但是也带来了一些理解上的困难。

在后续的文章中，我们会进一步讲解这个match关键字，以及它的一些特性。

## 常见容器
我们这里常见的容器有list，tuple，StringMap和Hashtbl

### list
list是函数式语言最为常见的容器，类似于cpp里面的vector。在cpp里面我们可以直接用vector提供的push_back
方法去添加元素，或者用vector提供的size方法去获取元素个数。但是在ocaml里面，我们不可以这样做。

在了解容器之前，我们需要贯彻一个思想，那就是

**万物皆为返回值**

我们在cpp里面，如果想从获得一个vector中所有偶数元素，我们可以这样

```cpp
vector<int> get_even(vector<int> v) {
    vector<int> res;
    for (int i = 0; i < v.size(); i++) {
        if (v[i] % 2 == 0) {
            res.push_back(v[i]);
        }
    }
    return res;
}
```

但是在ocaml里面，我们不可以这样做，我们需要用尾递归的方式去实现

```ocaml
let get_even v = 
    let rec helper v res = 
        match v with
        | [] -> res
        | x :: xs -> if x mod 2 = 0 then helper xs (x :: res) else helper xs res
    in helper v []
```

这里可能就有很多人不太理解了，为什么要这样做？我们不可以直接修改么？

回到我们上面提到的Immutability的概念，我们在ocaml里面，是不允许修改一个变量的，所以我们只能通过返回值的方式去实现。

一个很重要的hint，在函数式语言里面，所有的链表可以分为头和尾两部分，头部是一个元素，尾部是一个链表，这样的链表就是一个list
那么我们实现的方式为

```text
对于所有链表的表现形式，我们大概分几种
1. 空链表，这种是base case
2. 头部是一个元素，尾部是一个链表，那么我们处理头部，然后递归处理尾部
比如上面的例子，x是一个元素，也就是头部，xs是一个链表，也就是尾部
```

那么我们如果想找出来所有大于10的元素，我们可以这样写

```ocaml
let get_greater_than_10 v = 
    let rec helper v res = 
        match v with
        | [] -> res
        | x :: xs -> if x > 10 then helper xs (x :: res) else helper xs res
    in helper v []
```

如果我们想把所有奇数下标的元素都找出来，可以这样做

```ocaml
let get_odd_index v = 
    let rec helper v res idx = 
        match v with
        | [] -> res
        | x :: xs -> if idx mod 2 = 1 then helper xs (x :: res) (idx + 1) else helper xs res (idx + 1)
    in helper v [] 0
```

### StringMap 以及 Hashtbl
这两个容器都是用来存储键值对的，StringMap是用来存储string和任意类型的键值对，Hashtbl是用来存储任意类型的键值对。

其区别有点类似于cpp里面的map和unordered_map，StringMap是有序的，Hashtbl是无序的。

除了这些耳熟能详的区别，StringMap和Hashtbl还有一个很重要的区别

就是StringMap是不可变的，而Hashtbl是可变的

我们可以声明一个StringMap，然后往里面添加元素，但是我们不能修改StringMap里面的元素

```ocaml
let m = StringMap.empty (*空*)
in
let m1 = StringMap.add "a" 1 m (* {"a":1}*)
in
let m2 = StringMap.add "b" 2 m1(* {"a":1 , "b": 2}*)
in
let m3 = StringMap.add "c" 3 m2 (* {"a":1 , "b": 2, "c": 3}*)
```
上面是一个基本的调用法，但如果我们在这一串代码执行完以后，我们再去看m，m仍然是一个空map

那么Hashtbl则不一样了
```ocaml
let m = Hashtbl.create 10 (*创建一个空的Hashtbl*)
in
Hashtbl.add m "a" 1; (* {"a":1}*)
Hashtbl.add m "b" 2; (* {"a":1 , "b": 2}*)
Hashtbl.add m "c" 3; (* {"a":1 , "b": 2, "c": 3}*)
```

我们去看m，m和我们上面的m3是一样的

那么思考一下，我们在写编译器的时候，Hashtbl那么方便，为什么还要用StringMap呢？

在cpp中，每个元素是被存放在symbol_table里面的

我们也知道，cpp里面有作用域这个说法，一个花括号bracket就是一个作用域
比如下面这样
```cpp
int main() {
    int a = 1;
    {
        int a = 2;
    }
    int c = 3;
}
```
显然a的值在进行到c的时候仍然是1，我们只是在花括号里面重新定义了一个a，但是这个a的作用域只在花括号里面

那么编译器怎么知道我们引用的a是哪个a呢？ 

这就是symbol_table的作用，symbol_table里面存放的是一个变量名和一个变量的信息映射，它还保存有上一级的symbol_table的指向地址

这样，当我们在一个作用域里面找到了一个变量，那么我们就可以直接返回这个变量的信息，如果没有找到，那么我们就去上一级的symbol_table里面找

一旦用了Hashtbl，那么我们就无法知道上一级的symbol_table是什么了，因为Hashtbl是可变的，我们可以在任何地方往里面添加元素

这个道理大家现在只用记住，以后我们会用到

### type关键字

type关键字用来定义一个类型，比如我们可以这样定义一个类型
```ocaml
type t = int
```

这样我们就定义了一个类型t，它的类型是int

我们也可以这样定义一个类型
```ocaml
type t = int * int
```
这样我们就定义了一个类型t，它的类型是一个pair，pair的类型是int * int

我们去解析这个pair的时候，我们可以这样做
```ocaml
let (a,b) = (1,2)
```
或者
```ocaml
let f e = match e with
    | (a,b) -> a + b
```

当然它也可以是不同的类型
```ocaml
type t = int * string
type t1 = string * int
type t2 = int * string * bool
```
我们去解析这几个元组的时候，我们可以这样做
```ocaml
let (a,b) = (1,"a")
let (a,b) = ("a",1)
let (a,b,c) = (1,"a",true)
```
或者
```ocaml
let f e = match e with
    | (int , string) -> int
    | (string , int) -> int
    | (int , string , bool) -> int
```

甚至这个类型还可以是递归的

想一下我们在cpp当中，可以嵌套vector
比如
```cpp
vector<vector<int>> a;
```
那么我们在ocaml当中，也可以这样定义一个类型, 比如我们定义一个类型t
它可以是个int pair，但是可以是多个pair
```ocaml
type t = int
| t * t
```

我们去解析这个类型的时候，我们可以这样做
```ocaml
let f e = match e with
    | int -> int
    | t * t -> do_something
```

这个可能稍微有那么一些抽象，但是大家必须要试着去理解。

可以把这些想象成一个分治问题，我们在每一层dfs的时候，我们不关心其他地方在干什么，只要我们知道
，在这个地方干什么，那么我们就可以做出正确的决策

其实说了这么多，不如大家去实践一下，我觉得这样更好理解一点

### 练习题
我在github上面放了一些练习题，大家可以去做一下，这样对于大家理解这些概念会更加有帮助

链接：[Ocaml作业](https://github.com/tiany7/OCaml-)

把1-5可以先写了

遇到不会的可以找我要答案











