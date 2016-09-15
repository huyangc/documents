---
title: C++面试常见问题（二）
categories: C++
tags: [C++,面试] 

---

**转载请注明出处**：[blog.justkitt.com](http://blog.justkitt.com)

## 20. STL有哪些容器，对比vector和set
### 序列式容器
序列式容器一般是指那些存储形式表象是连续的，不间断的形式，这种形式的存储容器被称为序列式容器。
<!-- more -->
#### vector
vector的底层实现是数组，vector存储的是参数的一份拷贝，所以对于存放非指针来说，在存入vector之后，会被拷贝一份存入vector中。vector支持随机存取，就是说可以用下标的形式存储。同时vector的底层扩充机制是每次申请上次内存空间两倍的空间，然后将之前的内容拷贝到新的内存块中，然后消除原先的内存块。对于vector来说，一般使用push_back进行存放，从尾部加元素比其他位置加元素的效率要高，从中部加元素需要把之后的元素全部向后挪。
#### list
C++中的list是一个双向链表，每个节点都有指向其前一个节点的指针和后一个节点的指针，所以对于list来说，不支持随机存取，即[i]这种形式的存取是不被允许的。但是list对于插入这种情况的支持是非常友好的，因为只要改变一下指针指向的位置就可以了。
#### deque
vector是单向开口的连续线性空间，deque是双向开口的连续线性空间。具体的区别在于deque允许在头部进行元素的插入或移除操作，deque有**缓冲区**和**中控器**。中控器控制缓冲区的个数，缓冲区实际存放数据。具体的deque介绍可以看[这里](http://ibillxia.github.io/blog/2014/07/13/stl-source-insight-3-sequential-containers-3-deque-and-stack-queue/)，有关deque的图可以看[这里](http://www.programlife.net/stl-deque.html)。
### 关联式容器 
关联式容器有两种形式，一种底层是以树来进行实现的，在C++中没有unordered，一种底层是以hash函数来实现的，用unordered_开头。
#### set和map
底层用红黑树来实现，里面的数据是有序的，如果用迭代器的方式进行取出，则会得到有序的结果。map其实是set的一种特殊情况，感觉set在每个节点上存入一个pair，就可以构造出map的形式。（对key进行排序），这时候需要key的类型需要有实现比较函数``<``，``=``。
#### unordered_set和unordered_map
这两个函数底层用hash函数+桶的方式来实现，桶后面接上链表的方式，这种hash的方式称为**开链法**，这里面还需要理解的还有**[线性探测](https://zh.wikipedia.org/wiki/%E7%BA%BF%E6%80%A7%E6%8E%A2%E6%B5%8B)**，**[二次探测](https://en.wikipedia.org/wiki/Quadratic_probing)**。这些都是解决哈希冲突的方法。

## 21. 红黑树的定义和解释
红黑树是一种自平衡二叉树，对插入时间、删除时间和查询时间提供了最好可能的最坏情况担保。对于二叉查找树来说，有可能树是不平衡的，偏向一边的，这样在极端情况下，查询和插入的时间复杂度可能达到O(n^2)，这时候就需要一种情况让树平衡。红黑树通过以下性质来保证树的形状在大体上是平衡的：
- 树的每个节点是红色或者黑色的
- 根节点是黑色的
- 所有叶子节点都是黑色的（叶子节点是NIL节点）
- 从根到叶子节点的路径上不能存在两个连续的红色节点
- 从任一节点到其每个叶子节点的所有简单路径都有相同数量的黑色节点。

所以红黑树的**插入、删除、查找时间复杂度都是O(log n)**

红黑树的典型情况如下图![红黑树](http://obbyqbbco.bkt.clouddn.com/QQ%E6%88%AA%E5%9B%BE20160803185858.jpg)
红黑树的默认插入直接遍历整棵树，根据插入的节点的值找到相应的位置，将节点颜色变成红色（因为要满足从一个根节点到其所有叶子节点的路径上的黑色节点数量都相同），然后根据不能有连续相同的红色进行相应的调整。
> 新节点为X，父节点为P，祖父节点为G，伯父节点为S，曾祖父节点为GG。根据X的插入位置及外围节点（S和GG）的颜色，有以下四种考虑。
> 以下图片全部引用自《STL源码剖析》

- S为黑色X在外侧插入。对此情况，对P、G做一次单旋转，并更改P，G颜色，即可重新满足红黑树的条件了
![情况1](http://obbyqbbco.bkt.clouddn.com/%E7%BA%A2%E9%BB%91%E6%A0%911.jpg)
- S为黑切X为内侧插入，首先对P、X做一次单旋转，并且更改G、X的颜色，然后在对G做一次单旋转即可。（先左旋，再右旋）
![情况2](http://obbyqbbco.bkt.clouddn.com/%E7%BA%A2%E9%BB%91%E6%A0%912.jpg)
- S为红且X为外侧插入。先对P、G做一次单旋转，并改变X的颜色。如果GG为黑色，则满足情况，否则再对GG做调整。
![情况3](http://obbyqbbco.bkt.clouddn.com/%E7%BA%A2%E9%BB%91%E6%A0%913.jpg)
- 这种情况就对应于情况3中GG是红色的情况，需要不断往上调整。
![情况4](http://obbyqbbco.bkt.clouddn.com/%E7%BA%A2%E9%BB%91%E6%A0%914.jpg)
 
## 22. const关键字的作用
这个在第7点中又给出

## 23. 静态成员函数和数据成员有什么意义？
静态成员函数表示的是这个类的全体实例所具有的通用的方法，静态数据成员表示的是这个类所有实例所共有的数据。

## 24. 什么是模板特化？为什么要模板特化？
首先模板具有两种类型：**函数模板**和**类模板**，类模板可以有全特化和偏特化，而函数模板只有全特化。
```C++
template<typename T1, typename T2>    //模板
class Test
{
public:
	Test(T1 i,T2 j):a(i),b(j){cout<<"模板类"<<endl;}
private:
	T1 a;
	T2 b;
};

template<>
class Test<int , char>    //全特化
{
public:
	Test(int i, char j):a(i),b(j){cout<<"全特化"<<endl;}
private:
	int a;
	char b;
};

template <typename T2>
class Test<char, T2>		//偏特化
{
public:
	Test(char i, T2 j):a(i),b(j){cout<<"偏特化"<<endl;}
private:
	char a;
	T2 b;
};
```
函数模板只有全特化，代码如下：

```C++
//模板函数  
template<typename T1, typename T2>  
void fun(T1 a , T2 b)  
{  
    cout<<"模板函数"<<endl;  
}  
  
//全特化  
template<>  
void fun<int ,char >(int a, char b)  
{  
    cout<<"全特化"<<endl;  
}  
  
//函数不存在偏特化：下面的代码是错误的  
/* 
template<typename T2> 
void fun<char,T2>(char a, T2 b) 
{ 
    cout<<"偏特化"<<endl; 
} 
*/
```
>以上代码来自[这里](http://blog.csdn.net/thefutureisour/article/details/7964682)

为什么要模板偏特化：
偏特化的版本可以针对具体的类型做出更加友好的动作，更加具有针对性。（对模板元编程仍然不是很熟悉，可能做出的解释不是很准确，这里需要进一步考证，同时也希望有人能回答这个问题...）

## 25. explicit是干什么用的？
```C++
class A{
public:
	A(){}
	A(int){}
}
```
对于以上类声明，如果有语句A = 1，那么这句话是对的，它会调用A(int)这个构造函数来生成一个A的实例，这个叫做“隐式转换”，在相应构造函数的声明后加上explicit关键词之后，就可以避免这种“隐式转换”的发生。
```C++
class A{
public:
	A(){}
	explicit A(int) {}
}

A a = 1; //这句话是错的
A a(1)   //这句话是对的
```
在很多地方都推荐将这种构造函数加上``explicit``关键词。

## 26. strcpy返回类型是干嘛用的
返回值是char*的考虑是为直接可以将返回值作为另一个函数的参数传入，可以用于函数链式使用。

## 27. 内存溢出有那些因素？
首先是几个概念：
### 内存泄露
内存泄露是指由于疏忽或错误造成的程序未能释放已经不再使用的内存的情况。本质上来说，就是new了一个对象，但是没有用delete去删除它。
常见的内存泄露发生的情况：

### 内存溢出
内存溢出是指需要分配的内存超过了系统所能给予的，就产生了内存的溢出问题。有两种内存溢出的情况：
1. 堆内存溢出
直接malloc超级大的一块内存，超过系统内容。
2. 栈内存溢出
典型的有stackoverflow，就是递归层数较多，产生了无限递归这种情况。

栈满了的时候push一个就会产生溢出，叫做上溢，栈空的时候pop，叫做下溢。
缓冲区溢出是一种特殊的内存溢出（广义），导致缓冲区溢出的原因就在于不检查数组容量和下标。缓冲区溢出攻击是利用函数调用过程中的进出栈原理，在调用函数的时候，就把函数返回地址写入到一个栈上，然后新函数的局部变量写入到了这块内存所在的栈中，因为栈是自下而上增长的，所以不断写就会覆盖掉函数返回地址所在的那块内存，然后当函数返回的时候，就会执行到那块内存中指向的位置的代码，在该指向位置插入自己所想被执行的代码就可以完成缓冲区溢出攻击。

## 28. new与malloc的区别，delet和free的区别
- new是一个操作符，可以被每个类重写。
- new底层可以首先调用malloc申请一块适合的内存空间，然后使用该类型的构造函数进行函数的构造生成。
- new返回的是具体的类型，而malloc返回的是void*指针。
- new内存分配失败后会返回异常，而malloc分配失败的返回是NULL
- new无需指定内存大小，而malloc需要
- new[]用于专门进行数组的处理，而malloc还是一样，知识申请内存空间的大小不同
- 因为new是操作符，所以new可以被重载，但是malloc不行

具体的区别有个[总结](http://www.cnblogs.com/QG-whz/p/5140930.html#_label1_0)

![malloc和new的区别](http://obbyqbbco.bkt.clouddn.com/new%E5%92%8Cmalloc%E7%9A%84%E5%8C%BA%E5%88%AB.jpg)


## 29. 为什么要用static_cast转换而不用c语言中的转换？
首先这些C++里的四种类型转换都会提供类型检查，对于不太合适的类型转换会不允许进行。
static_cast相当于传统的C语言中那些“较为合理”的强制类型转换，较多的使用于基本数据类型之间的转换、基类对象指针（或引用）和派生类对象指针（或引用）之间的转换、一般的指针和void*类型的指针之间的转换等。static_cast操作对于类型转换的合理性会作出检查，对于一些过于“无理”的转换会加以拒绝。例如下面的转换：
```C++
double d = 3.14;
double *p = static_cast<double*>(d);
```
>以上内容来自[这里](http://blog.csdn.net/k346k346/article/details/47750813)

就会遭到拒绝，但是这个在C语言中的转换是可以执行的。灵活性带来的是高风险，C++限制了部分灵活性，降低了风险。

## 30. 异常机制是怎么回事？
详见[这里](http://blog.csdn.net/lyrebing/article/details/36186721)

## 31. 迭代器删除元素的会发生什么?
这里考的是迭代器失效的问题。有些容器删除某些元素的时候，根据元素的位置，可能会导致一些迭代器失效。下面是不同容器的各种操作迭代器是否会失效的说明：
- 对于**关联式容器（map，set，multimap，multiset）**，删除当前的iterator，仅仅会导致当前iterator失效，只要在erase时，递增当前iterator即可。这是因为对于这些关联式容器，底层使用红黑树来实现，插入、删除一个节点不会对其他节点产生影响。
```C++
for(auto it = cont.begin();it!=end();){
	cont.erase(it++);
}
```
- 对于序列式容器（特别是支持随机存取的容器：vector、deque），删除当前iterator会使**后面所有元素的iterator都失效**，因为这些支持随机存取的容器使用连续分配的内存，删除一个元素会导致后面的所有元素都往前移动一个位置。删除代码
```C++
for(auto it = cont.begin();it!=cont.end();){
	it = cont.erase(it);
}
```
**list**是序列式容器里的特例，因为它使用数组存取，对于元素的获取也只是通过指针，所以当前iterator失效并不会影响其他的元素，同样可以使用it++的形式删除。

## 32. 各容器特性总结
对于deque的容量更新方式，可以参考[这里](http://ibillxia.github.io/blog/2014/07/13/stl-source-insight-3-sequential-containers-3-deque-and-stack-queue/)描述的比较清楚。
![各容器比较](http://obbyqbbco.bkt.clouddn.com/%E5%90%84%E5%AE%B9%E5%99%A8%E6%AF%94%E8%BE%83.jpg)

## 33. 必须在构造函数初始化式里进行初始化的数据成员有哪些？
说明白点就是那些const类型的数和具有const情况的数。
- const关键词修饰的数据成员
- 类型是引用的数据成员
- 需要初始化的成员是对象的情况
```C++
class A{
public:
	int& i;
	const int j;
	string str;
	A(int& i,int j,string& str):i(i),j(j),str(str){}
}
```
至于第三种情况的原因是如果不是用初始化列表的形式进行初始化的话，会造成首先生成一个对象，然后再将另一个对象的值赋给这个新生成的对象，会调用构造函数&拷贝函数（operator=)，如果直接使用初始化列表的形式，那么只会调用拷贝构造函数。效率会高一些。

初始化列表的实际产生方式会以适当的顺序在实际的构造函数中会产生一些赋值的代码，赋值的顺序是类中成员数据的定义顺序，与初始化列表的书写顺序无关。但是初始化列表一定会在构造函数内的实际代码之前。
```C++
class A{
public:
	int a;
	int b;
	int c;
	A():c(1),b(1){
		a = c;
	}
}
```
在以上代码中，b元素的赋值会在c之前，但是a元素的赋值会在b、c之后。

## 34. 类的封装：private，protected，public
这些词可以作用在两个地方，第一个是修饰类内成员函数、成员数据，另一个是在继承的时候修饰基类。下面会根据这两种情况分别讨论。
- 修饰类内成员函数和数据成员
	- private
	只能由该类中的函数和该类的友元函数访问
	- protected
	可以被该类中的函数，子类中的函数和友元函数访问
	- public
	可以被该类中的函数，子类中的函数，友元函数和该类的对象访问
- 修饰基类
	- private
	父类中的public、protected属性在子类中自动变成private
	- protected
	父类中的public、protected属性在子类中变成protected
	- public
	不改变父类中的public和protected属性。

**总结**
```C++
class A{
public:
	int a;
	int getA(){return a;}
	int getB(){return b;}
	int getC(){return c;}
	double average(){return static_cast<double>(sum())/3;}
protected:
	int b;
	
private:
	int c;
	int sum(){return a+b+c;}
}
A a;
a.getA();
a.getB();
a.getC();
a.sum(); //提示访问限制，无法访问sum函数。
a.b;//提示访问限制，无法访问b
```
如果有B类public继承A类，则A中的所有内容的访问限制被B继承，如果是protected继承，那么public域会变成protected，如果是private继承，那么public域和protected域都会变成private域。如果不加关键词，默认使用protected继承。即``class B:A``等价于``class B: protected A``

## 35. auto_ptr类
属于智能指针类，在C++11版本之后，就变成了shared_ptr和unique_ptr。在C++11中，auto_ptr被置为deprecated。
- unique_ptr
unique_ptr从名字上来看可以看出，这个对象里存放的内容在整个程序中只能出现一次。不能有copy语义，只能是move语义
```C++
std::unique_ptr<int> p1(new int(5));
std::unique_ptr<int> p2 = p1; // 编译会出错
std::unique_ptr<int> p3 = std::move (p1); // 转移所有权，现在那块内存归p3所有, p1成为无效的指针。
p3.reset(); //释放内存。
p1.reset(); //实际上什么都没做。
```
- shared_ptr
shared_ptr从名字上可以看出，这是个可以共享的指针，可以有多个指针同时指向同一块内存，这是unique_ptr无法做到的。底层用引用计数的方式实现，即每增加一个指针，该内存的引用计数+1，减少一个指针，内存引用计数-1，当为0的时候，内存就会被清除。具体代码如下
```C++
std::shared_ptr<int> p1(new int(5));
std::shared_ptr<int> p2 = p1; // 都指向同一内存。
p1.reset(); // 因为p2还在，所以内存没有释放。
p2.reset(); // 释放内存，因为没有shared_ptr指向那块内存了。
```
>随着引用计数而来的就是循环引用产生无法删除的情况。为了避免循环引用的情况出现，可以使用weak_ptr，弱引用只引用不增加或删减计数。如果一块内存被shared_ptr和weak_ptr同时引用，当所有的shared_ptr析构了之后，不管有没有weak_ptr引用该内存，内存会被释放。 from [wikipedia](https://zh.wikipedia.org/wiki/%E6%99%BA%E8%83%BD%E6%8C%87%E9%92%88)  
```C++
std::shared_ptr<int> p1(new int(5));
std::weak_ptr<int> wp1 = p1; // 还是只有p1有所有权。
{
  std::shared_ptr<int> p2 = wp1.lock(); // p1和p2都有所有权
  if (p2) // 使用前需要检查
  { 
    // 使用p2
  }
} // p2析构了，现在只有p1有所有权。
p1.reset(); // 内存被释放。
std::shared_ptr<int> p3 = wp1.lock(); // 因为内存已经被释放了，所以得到的是空指针。
if（p3）
{
  // 不会执行到这。
}
```

## 36. 指向数据成员的指针
这个点一般会在选择题或者填空题中问。
```C++
class A{
public:
	int x,y,z;
}
```
问``&A::z``是多少？
表示的是z在A类中的偏移是多少，最低限度是x、y大小的总和，但是对于有virtual成员函数的类来说，还要加上一个vptr。按照这种情况推理，那么就是12，但是如果真这么回答，你就**错了**！为了区分``&A::x``和0，会将所有的地址偏移加上1。
- 取一个nonstatic data member的地址，就会得到它在class中的偏移量
- 取一个绑定在类实例上的data member的地址，就会得到该member在内存中的真正地址。

## 37. C++空类有哪些成员函数
- 缺省构造函数
- 缺省拷贝构造函数
- 缺省析构函数
- 缺省赋值运算符
- 缺省取址运算符
- 缺省取值运算符（const）

## 38. C++修饰类关键词总结
所谓修饰类关键词就是那种可以作用在其他关键词之上增加一些语义信息的关键词，下面给出我目前知道的所有修饰类关键词
### **const**
- 在类中修饰成员函数表示该成员函数不能将类成员变量当做左值使用
- 在类中修饰成员变量表示成员变量只能被赋值一次，需要在构造函数的初始化列表里进行赋值
- **位置**：成员函数后，成员变量前。
### **static**
- 在类中修饰成员函数表示该成员函数对类的所有实例都一样，是一个类方法。由于调用不需要this指针，所以在static成员函数中不能调用非static成员函数。
- 在类中修饰成员变量表示成员变量是整个类的成员变量，所有类都一样
- **位置**：成员函数和成员变量前。
### **virtual**
- 只能修饰成员函数，放在成员函数前，若后面加上=0表示纯虚函数。
### **final**
- 修饰类表示该类是最终版本，不允许作为基类被继承
- 修饰虚函数表示该虚函数不允许被重写**不能修饰非虚函数**
- **位置**：在类和成员函数的后面。
	```C++
	class FinalBase final{
	public:
		int i = 0;
		void virtual say() final{
		cout << "hello" << endl;
		}
	};
	class deriveClass :public FinalBase { //会报错，不允许继承final修饰的类
	public:
		void say(){};    //就算上面可以继承，这个函数也会报错，因为不能重写final修饰的虚函数
	};
	```
### **override**
- 只能修饰类中的虚成员函数
- **位置**：成员函数名之后
	```C++
	struct A
	{
    	virtual void foo();
    	void bar();
	};
 	struct B : A
	{
    	void foo() const override; // Error: B::foo does not override A::foo (signature mismatch) const是一定要满足的
    	void foo() override; // OK: B::foo overrides A::foo
    	void bar() override; // Error: A::bar is not virtual
	};
	```
### **mutable**
- 用于修饰类成员变量，表示即使用一个const的函数或者类对象，这个成员变量仍然可以作为左值进行更改
- **位置**：``mutable int n2``
### constexpr 
constexpr关键词定义表示可以在编译期间就可以知道函数或者变量的值，不一定要在类中使用。更具体的解释可以参考[这里](https://www.devbean.net/2012/05/cpp11_constexpr/)
- 修饰函数表示该函数可以在编译器运行，所以函数的参数都必须是常量

### **=default**
- 一般用在那种能由编译器生成的函数**37**中的函数，表示希望编译器帮忙生成函数主体，然后自己可以不用实现
### **=delete**
- 一般用在类中用于删除某个函数，表示该函数实际上不存在。
**所有有=开头的关键词都用在函数声明式之后大括号之前**。



... To Be Continued