---
title: C++面试常见问题（一）
categories: C++
tags: [C++,面试] 

---

**转载请注明出处**：[blog.justkitt.com](http://blog.justkitt.com)

## 1. 多态性都有哪些？
重写分为两种情况，子类重写父类的虚函数，子类重写父类的非虚函数。前一种情况被称为多态。多态属于函数地址晚绑定（运行时绑定）的情况，而后一种属于静态的，地址是早绑定的，在编译器编译期间就会确定函数的地址被称为覆盖。
<!-- more -->
- 静态多态
使用模板来进行的多态实现
```C++
class Car
{
public:
    void run() const
    {
        std::cout << "run a car"<<endl;
    }
};
class Airplane
{
public:
    void run() const
    {
        std::cout << "run a airplane"<<endl;
    }
};
template<class Vehicle>
void run_vehicle(const Vehicle& vehicle){
	vehicle.run();
}
```
- 动态多态
使用虚函数来实现
```C++
class Vehicle{
public:
	void virtual run() const = 0;
};
void Vehicle::run() const{    //纯虚函数同样可以被实现，但是仍然不能被实例化，注意仍然要带有const修饰符。
	cout<<"base class"<<endl;
}
void Vehicle::run()
class Airplane: public Vehicle{
public:
	void run() const{    //注意： 这里如果没有const则没有重写父类虚函数
		cout<<"Airplane run"<<endl;
	}
};
int main(){
	Vehicle* ve;
	Airplane airplane;
	ve = &airplane;
	ve->run();    //输出 Airplane run
}
```

## 2. 动态绑定怎么实现？
表现形式：用基类指针指向子类实例，通过基类指针来调用子类中的函数。
底层原理：在含有虚函数的类中有个虚表指针（vptr），这个指针指向一个函数虚表（vtbl），函数的实际地址会在实际运行的时候再存在这个虚表里，函数的执行通过vptr首先找到vtbl然后再在vtbl中找到相应的条目，获取到实际的地址。
## 3. 类型转换有哪些？
- 最基本的C的强制类型转换：在对象前面用(typename) 来表示
- **``anothertype = static_cast<type>(instance)``** :基本数据类型之间的转换，若涉及到类，则只能在有相互联系的类型中相互转换
- **``const_cast``**:更改表达式的常量属性，可以把一个表达式变成常量或者把一个常量表达式变成非常量
- **``reinterpret_cast``**：和C风格相似的能力，可以转换任何内置类型为其他数据类型，可以转换指针类型为其他类型，把其他类型转换为指针，无需考虑类型安全。*不推荐使用*
- **``dynamic_cast``**:其他的转换都是编译时完成，dynamic_cast是运行时完成，需要进行类型检查；不能用于内置数据类型转换；转换成功返回引用或指针，失败返回``nullptr``；基类必须拥有虚函数，否则编译不通过；**向上类型转换dynamic_cast和static_cast一样，但是向下类型转换，dynamic_cast有类型检查，static_cast没有，故在这种情况下比static_cast更安全**。

### 总结
在C++中内置类型转换使用static_cast，类之间的类型转换用dynamic_cast，常量属性赋予用const_cast，指针之间的各种转换使用reinterpret_cast。

## 4. 操作符重载，具体如何定义？
操作符重载有两种形式：
- 定义在类中
- 定义在类外，但是通过友元声明是类的友元函数。

```C++
class A{
public:
	A(int data):data(data){}
	A operator+(A&);                 //
	A operator-(A&);                 //定义在类中
	A operator*(A&);                 //
	A operator/(A&);                 //
	friend A operator+(A&,A&);	   //
	friend A operator-(A&,A&);       // 友元函数
	friend A operator*(A&,A&);       //  
	friend A operator/(A&,A&);       //
	friend ostream& operator<<(ostream& out,A& a); //输入输出流操控运算符重载需要用友元函数的形式
private:
 	int data;
}
A A::operator+(A& a){
	return A(data+a.data)
}
...

A operator+(A& a1,A& a2){
	return A(a1.data+a2.data);
}

ostream& operator<<(ostream& out,A& a){
	out<<a.data<<endl;
	return out;
}
```
解释：
虽然声明在类内部看上去会稍微简单一些，但是考虑到类内部的操作符的调用形式实际上可以写成obj.operator+(obj1)这种形式，也就是说，只能应对一个obj+xxx的形式，对于xxx+obj这种形式使用类内运算符重载就无法支持了，所以一般推荐使用友元函数的方式。对于拷贝运算符=，需要特别注意的是一定要把参数声明为引用或指针（一般为引用），这样可以避免一个无限递归的过程。实参的传递需要进行拷贝构造。

## 5. 内存对齐的原则是什么？
- 对于结构的各个成员，第一个成员位于偏移量为0的位置，以后每个数据成员的偏移量必须是min(#pragma pack()制定的数，这个数据成员自身的长度）的倍数
- 数据成员完成各自对齐之后，结构（或联合）本身也要进行对齐，对齐将按照#pragma pack制定的数值和结构（联合）最大数据成员长度中，比较小的那个进行。

### 主要作用：
经过内存对齐后，CPU访问内存速度大大提升。

## 6. 模板怎么实现？
- 函数模板

```C++
template <typename T> //typename和class没有什么不同
int compare(const T& v1,const T& v2){
...//do something
}
```
实例化函数模板：一般由编译器来进行实例化的操作。
- 类模板

```C++
template <typename T> class A{
public:
	T a;
...
}
```
类模板的实例化：一般由函数调用者决定。

### 实现方法

## 7. 指针和const的用法
``const int * p``
``int const * p``本质上两者是一样的。表示指针指向的是一个常量int，无法通过指针改变这个内容的值。
``int * const p``表示指针是一个常量指针，无法改变指针指向的地址。

### const可以修饰的位置
函数参数：表示函数不会通过参数修改实际传入的值
类的成员函数：表示该成员函数不会修改类中的值，也无法修改。（在该类型的成员函数中出现的类成员都只能作为右值出现）
类的成员变量：表示该类成员在所有类的成员函数中都不允许以左值的方式出现，在构造函数中必须通过初始化列表的方式进行初始化。否则会报错。
类的实例：被const修饰的类的实例也只能调用其const成员函数。

## 8. 虚函数、纯虚函数、虚函数与析构函数。
虚函数是指在类成员函数中以``virtual``关键词定义的函数，纯虚函数是在虚函数定义的基础上在最后加上``=0``。在一个具有多态性质的基类中，需要定义虚析构函数，因为如果不定义虚析构函数，在函数析构的过程中，就只会处理基类所对应的那块内存，对于整个继承链上的函数来说是不充分的。例子如下：
```C++
class A{
public:
	void virtual run(){         //虚函数
		cout<<"run in A"<<endl;
	}
	void virtual eat() = 0;    //纯虚函数
}
class B{
public:
	void run(){
		cout<<"run in B"<<endl;
	}
	void eat(){
		cout<<"eat in B"<<endl;
	}
}
```
首先，定义有纯虚函数的类就是抽象类，抽象类是不允许被实例化的（包括new的形式），如果一个类继承了抽象类，那么这个类如果在没有实现纯虚函数的前提下，仍然是抽象类，但是实现了之后就不属于抽象类了。基类中的纯虚函数可以自己实现，也可以不实现，不论是否实现，都无法实例化。
## 9. 内联函数
**定义**：内联和函数在函数调用的过程中，会使用函数的定义体来直接替换函数的调用程序，这种替代行为会发生在编译阶段而不是程序运行阶段，这样的好处在于函数的调用会涉及到一系列的栈的使用，入栈和出栈的过程会被省略。省略的函数调用过程会提高效率和性能。
**与宏定义的区别**：过程实际上是一样的，都是通过展开来避免函数调用的开销，但是内联函数会在展开的时候进行参数类型、返回值等，宏定义并不会，所以内联函数会更加安全。
### 内联函数代码
```C++
inline void f(int i,int j){
	... //do something
}
```
### 宏定义代码
```C++
#define multi(a,b) (a*b)
inline int multi(a,b);    //产生错误，重名
int main(){
	multi(1,2)
	multi(1+2,2+3)  //产生意想不到的结果
}
```
## 10. typedef

1. 定义一种类型别名，而不只是简单的宏替换。对于``char* pa,pb``，pb并不是指针类型，但是使用typedef之后，有
```C++
typedef char* PCHAR;
PCHAR pa,pb;
```
2. 在C中定义一个结构需要使用关键词structure，C++中可以直接使用名称，C不能省略structure，所以可以用typedef来进行类型声明
3. typedef定义了一种类型的新别名
```C++
typedef char* PSTR;
int mystrcmp(const PSTR,const PSTR);
```
实际上定义了一个char* const，即常量指针而不是指向常量的指针。

## 11. 排序算法
### 快速排序
从各种类型的面试来看，快速排序在没有问题问的时候经常会出现，但是由于最近可能问烂了，所以有可能会问归并和堆排序。快速排序的思想是，首先找到一个key，然后用这个key把整个数组分成两个部分，前一个部分的数字都小于它，后一个部分的数字都大于它，这样这个key就被排到了它本身应该在的地方。采用归并的思想去解决排序的问题。代码如下
```C++
int partition(vector<int>& nums,int begin,int end){
	int j = begin;
	int key = nums[begin];
    for(int i = begin+1;i<=end;++i){
		if(nums[i]<key){
			++j;
			if(j != i)
				swap(nums[i],nums[j]);
		}
	}
	swap(nums[j],nums[begin]);
	return j;
}
void quick_sort(vector<int>& nums,int begin,int end){
	if(begin<end){
		int mid = partition(nums,begin,end);
		quick_sort(nums,begin,mid-1);
		quick_sort(nums,mid+1,end);
	}
}	
```
### 归并排序
归并排序的思路在于不断地把一个数组分成两部分，直到最后分成两两比较的形式，然后再进行合并。所以归并排序比较重要的函数是merge函数。代码如下
```C++
void merge(vector<int>& nums, int first, int mid, int last, vector<int>& temp) {
	int i = first, j = mid + 1;
	int m = mid, n = last;
	int k = 0;
	while (i <= m&&j <= n) {
		if (nums[i]<nums[j])
			temp[k++] = nums[i++];
		else
			temp[k++] = nums[j++];
	}
	while (i <= m) temp[k++] = nums[i++];
	while (j <= n) temp[k++] = nums[j++];
	for (i = 0; i<k; ++i) {
		nums[first+i] = temp[i];
	}
}
void merge_sort(vector<int>& nums, int begin, int end, vector<int>& temp) {
	if (begin<end) {
		int mid = begin + (-begin + end) / 2;
		merge_sort(nums, begin, mid,temp);
		merge_sort(nums, mid + 1, end,temp);
		merge(nums, begin, mid, end, temp);
	}
}
```
### 堆排序
堆排序的思路是先构建一个最小堆（最大堆），然后从这个最小（最大）堆里取根节点上的那个数，之后维护堆的性质，直到把堆中的所有元素都取完。代码如下
```C++
void maxHeapify(vector<int>& nums,int i,int size){
	int leftChild = 2*i;
	int rightChild = 2*i+1;
	int maxIndex = i;
	if(i<=size/2){            //仅调整非叶结点
		if(leftChild<=size&&nums[leftChild]>nums[maxIndex])
			maxIndex = leftChild;
		if(rightChild<=size&&nums[rightChild]>nums[maxIndex])
			maxIndex = rightChild;
		if(maxIndex!=i){
			swap(nums[maxIndex],nums[i]);
			maxHeapify(nums,maxIndex,size);
		}
	}
}	
void buildHeap(vector<int>& nums,int size){
	for(int i = size/2;i>=1;--i){  //数组的index从1开始，方便计算 leftChild = i*2,rightChild = i*2 + 1;
		maxHeapify(nums,i,size);
	}
}
void heap_sort(vector<int>& nums, int size){
	buildHeap(nums,size);
	for(int i = size;i>=1;--i){
		swap(nums[1],nums[i]);
		maxHeapify(nums,1,i-1);
	}
}
```
### 冒泡排序
冒泡排序的思想是重复走访要排序的数列，一次比较两个元素，如果他们的顺序错误就把他们交换过来。整个算法的过程会使得越小的元素经由交换慢慢“浮”到数列的前端，每次排完一轮，最大的那个数就到了最后那个位置上。
```C++
bubble_sort(vector<int>& nums){
	for(int i = 0;i<nums.size();++i){
		for(int j = 0;j<nums.size()-i-1;++j){
			if(a[j]>a[j+1])
				swap(a[j],a[j+1]);
		}
	}
}
```
## 12. 链接指示： extern "C"的作用？
C++使用链接指示指出任意非C++函数所用的语言。链接指示有两种形式：单个的和复合的。
- 单个的
``extern "C" size_t strlen(const char*);``
- 复合的
```C++
extern "C"{
	int strcmp(const char* ,const char*);
	char* strcat(char*,const char*);
}
```

## 13. C语言和C++有什么区别？
C++是一个面向对象的语言，C语言是一个面向过程的语言，C++中含有类这个概念，许多内容都围绕着类来进行，例如面向对象的三大特性：封装、继承、多态都是围绕着类来进行的，这对于C语言来说是完全不可能的。同时C++还添加了异常处理的方式。

## 14. strcpy函数的编写
```C++
char* strcpy(char* strDest,char* strSrc){
    assert(strDest != nullptr);
    assert(strSrc != nullptr);
    char* ret = strDest;
    for(int i = 0;strSrc[i]!='\0';++i){
        strDest[i] = strSrc[i];
	}
	strDest[i] = '\0';
	return ret;
}
```
需要考虑的点：
- 传入的参数不应为nullptr
- 要记录返回内存地址的首地址
- 在用for循环检测到'\0'的时候要记得在最后加上'\0'
- 可能会发生覆盖的现象，即strDest的位置在strSrc和'\0'之前。
- 返回值是char*的考虑是为直接可以将返回值作为另一个函数的参数传入

## 15. 数据结构二叉树的非递归遍历
思路：一般在问一个递归问题的非递归解法的时候，可以考虑用栈来模拟递归函数带来的那种进出的感觉。对于二叉树的遍历方式，主要有四种情况：
- 前序遍历

递归形式：

```C++
void helper(vector<int>& ret,TreeNode* root){
	if(root == nullptr)
		return;
	ret.push_back(root->val);
	helper(ret,root->left);
	helper(ret,root->right);
}

```
非递归形式：
```C++
void pre_traverse(vector<int>& ret,TreeNode* root){
	stack<TreeNode*> s;
	TreeNode* p = root;
	while(p!=nullptr||!s.empty()){
		while(p!=nullptr){
			ret.push_back(p->val);
			s.push(p);
			p = p->left;
		}
		if(!s.empty()){
			p = s.top();
			s.pop();
			p = p->right;    //在把p的右节点存入的时候弹出p
		}
	}
}
```
- 中序遍历
递归形式
```C++
void helper(vector<int>& ret,TreeNode* root){
	if(root == nullptr){
		return;
	}
	helper(ret,root->left);
	ret.push_back(root->val);
	helper(ret,root->right);
}
```
非递归形式
```C++
void inorder_traverse(vector<int>& ret,TreeNode* root){
	stack<TreeNode*> s;
	auto p = root;
	while(p!=nullptr||!s.empty()){
		while(p!=nullptr){
			s.push(p);
			p = p->left;
		}
		if(!s.empty()){
			p = s.top();
			ret.push_back(p->val);
			s.pop();
			p = p->right;
		}
	}
}
```
- 后序遍历
递归方法
```C++
void helper(vector<int>& ret,TreeNode* root){
	if(root == nullptr)
		return;
	helper(ret,root->left);
	helper(ret,root->right);
	ret.push_back(root->val);
}
```
非递归方法
思路：保证根节点在左孩子和右孩子被访问后才能被访问，所以对于任一节点P，先将其入栈，如果P不存在左孩子和右孩子，则直接访问它，如果P存在左孩子或右孩子，但是孩子都已经被访问过了，也同样可以访问该节点。如果是不能访问该节点的情况，就依次将P的左孩子和右孩子依次入栈。
```C++
void post_order(vector<int>& ret,TreeNode* root){
	stack<TreeNode*> s;
	TreeNode* cur;
	TreeNode* pre = nullptr;
	s.push(root);
	while(!s.empty()){
		cur = s.top();
		if((cur->left == nullptr&&cur->right == nullptr)||
		   (pre!=nullptr&&(pre == cur->left||pre == cur->right)))
		{
			ret.push_back(cur->val);
			s.pop();
			pre = cur;
		}
		else
		{
			if(cur->right != nullptr)
				s.push(cur->right);
			if(cur->left != nullptr)
				s.push(cur->left);
		}
	}
}
```
- 层次遍历
层次遍历使用队列来完成，每次记录都记录在当前队列中的节点的数量。然后用这个队列就可以完成所有的遍历情况。

## 16. 继承机制中对象之间是如何转换的？
在子类的实例对象所在的内存中，一般而言内存顶部是基类的内容。如果将一个子类对象赋予一个基类对象（非指针和引用），那么会发生自动截断，仅仅保留父类内容。子类对象值可以赋予基类对象，但是基类对象值赋予子类对象会产生问题。

## 17. 继承机制中引用和指针之间如何转换？
C++中的指针仅仅表示这个指针值所指向的那块内存地址的大小而已，所以我感觉继承机制中指针之间的转换不就是表示所指向的内存大小会改变而已吗？因为类的继承机制，基类内容会在子类对象的上部分，所以对于基类指针来说，指向的内存块也仅仅是基类的范围，指不到子类的范围，但是子类的指针会比基类指针范围大，如果将基类对象赋予子类指针，会产生问题。

## 18. 虚函数，虚函数表里面内存如何分配？
虚函数和虚函数表的情况是建立在基类和子类之间的继承关系基础上的，所以我们考虑一下三种集成情况。
- 继承基类，没有重写基类虚函数
- 继承基类，重写了基类虚函数
- 多重继承，重写基类虚函数

下面给出的图片是多重继承的类型，图片来自[这里](http://speakingbaicai.blog.51cto.com/5667326/1686404)
![多重继承](https://s6.postimg.org/6s05j9ky9/image.jpg)
即：继承会得到基类的虚指针，虚指针指向基类虚表，虚表里存放的是基类的虚函数地址，如果子类有更改，会相应地将虚表中的地址值给覆盖。这样就完成了继承过程中虚地址的分配。**多重继承会得到所有基类的虚地址**。

## 19. 如何定义一个只能在堆上（new）或者栈上实例化的类？
- 只能在堆上实例化的类
我们假设我们要实例化的类名为A，那么只能在堆上实例化的类的意思就是不能用``A a``这种形式来进行类对象的实例化，只能用``A* a = new A``这种形式来进行实例化。首先的想法是**将构造函数设为private**，这种方式有问题，就是第一种情况不能被访问，同时第二种情况也不能，因为new操作会先申请一块内存，然后在这块内存上调用类的构造函数去初始化，如果将类的构造函数设为private的话，new操作也无法进行初始化。所以这是有问题的。然后考虑到在栈上的空间会自己释放内存，然后调用类的析构函数，而new出来的对象需要自己调用delete，如果不调用delete的话，会一直存在于内存中。所以考虑可以将析构函数变成private的。这样可以new出来一个新对象，但是对象删除的时候需要额外写一个函数去操作。
```C++
class A{
public:
	void destroy(){
		delete this
	}
private:
	~A(){}
}
```
以上这种问题的方式是在A类作为基类的情况下无法存在的，因为析构函数被声明为private的，所以任何继承A类的类都无法析构。如果需要让A类作为基类，则常将其析构函数设为virtual，然后在子类重写，已实现多态。所以可以将其设置为protected。
- 只能建立在栈上
只有使用new运算符对象才会建立在堆上，所以将new运算符私有化之后就可以让类的对象无法实例化在堆上。
```C++
class A{
private:
	void * operator new(size_t){}
	void operator delete(void * ptr){}
public:
	A(){}
	~A(){}
};
```