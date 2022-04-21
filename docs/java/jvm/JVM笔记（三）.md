# 类与类加载

前面我们讲解了JVM的内存结构，包括JVM如何对内存进行划分，如何对内存区域进行垃圾回收。接下来，我们来研究一下类文件结构以及类的加载机制。

## 类文件结构

在我们学习C语言的时候，我们的编程过程会经历如下几个阶段：写代码、保存、编译、运行。实际上，最关键的一步是编译，因为只有经历了编译之后，我们所编写的代码才能够翻译为机器可以直接运行的二进制代码，并且在不同的操作系统下，我们的代码都需要进行一次编译之后才能运行。

> 如果全世界所有的计算机指令集只有x86一种，操作系统只有Windows一种，那也许就不会有Java语言的出现。

随着时代的发展，人们迫切希望能够在不同的操作系统、不同的计算机架构中运行同一套编译之后的代码。本地代码不应该是我们编程的唯一选择，所以，越来越多的语言选择了与操作系统和机器指令集无关的中立格式作为编译后的存储格式。

“一次编写，到处运行”，Java最引以为傲的口号，标志着平台不再是限制编程语言的阻碍。

实际上，Java正式利用了这样的解决方案，将源代码编译为平台无关的中间格式，并通过对应的Java虚拟机读取和运行这些中间格式的编译文件，这样，我们只需要考虑不同平台的虚拟机如何编写，而Java语言本身很轻松地实现了跨平台。

现在，越来越多的开发语言都支持将源代码编译为`.class`字节码文件格式，以便能够直接交给JVM运行，包括Kotlin（安卓开发官方指定语言）、Groovy、Scala等。

![image-20220223162914535](https://tva1.sinaimg.cn/large/e6c9d24egy1gznizn2l97j21qc0jedgq.jpg)

那么，让我们来看看，我们的源代码编译之后，是如何保存在字节码文件中的。

***

### 类文件信息

我们之前都是使用`javap`命令来对字节码文件进行反编译查看的，那么，它以二进制格式是怎么保存呢？我们可以使用WinHex软件（Mac平台可以使用[010 Editor](https://www.macwk.com/soft/010-editor)）来以十六进制查看字节码文件。

```java
public class Main {
    public static void main(String[] args) {
        int i = 10;
        int a = i++;
        int b = ++i;
    }
}
```

找到我们在IDEA中编译出来的class文件，将其拖动进去：

![image-20220223164725971](https://tva1.sinaimg.cn/large/e6c9d24egy1gznjij4fgpj21800u011h.jpg)

可以看到整个文件中，全是一个字节一个字节分组的样子，从左上角开始，一行一行向下读取。可以看到在右侧中还出现了一些我们之前也许见过的字符串，比如"<init>"、"Object"等。

实际上Class文件采用了一种类似于C中结构体的伪结构来存储数据（当然我们直接看是看不出来的），但是如果像这样呢？

```
Classfile /Users/nagocoler/Develop.localized/JavaHelloWorld/target/classes/com/test/Main.class
  Last modified 2022-2-23; size 444 bytes
  MD5 checksum 8af3e63f57bcb5e3d0eec4b0468de35b
  Compiled from "Main.java"
public class com.test.Main
  minor version: 0
  major version: 52
  flags: ACC_PUBLIC, ACC_SUPER
Constant pool:
   #1 = Methodref          #3.#21         // java/lang/Object."<init>":()V
   #2 = Class              #22            // com/test/Main
   #3 = Class              #23            // java/lang/Object
   #4 = Utf8               <init>
   #5 = Utf8               ()V
   #6 = Utf8               Code
   #7 = Utf8               LineNumberTable
   #8 = Utf8               LocalVariableTable
   #9 = Utf8               this
  #10 = Utf8               Lcom/test/Main;
  #11 = Utf8               main
  #12 = Utf8               ([Ljava/lang/String;)V
  #13 = Utf8               args
  #14 = Utf8               [Ljava/lang/String;
  #15 = Utf8               i
  #16 = Utf8               I
  #17 = Utf8               a
  #18 = Utf8               b
  #19 = Utf8               SourceFile
  #20 = Utf8               Main.java
  #21 = NameAndType        #4:#5          // "<init>":()V
  #22 = Utf8               com/test/Main
  #23 = Utf8               java/lang/Object
{
  public com.test.Main();
    descriptor: ()V
    flags: ACC_PUBLIC
    Code:
      stack=1, locals=1, args_size=1
         0: aload_0
         1: invokespecial #1                  // Method java/lang/Object."<init>":()V
         4: return
      LineNumberTable:
        line 11: 0
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0       5     0  this   Lcom/test/Main;

  public static void main(java.lang.String[]);
    descriptor: ([Ljava/lang/String;)V
    flags: ACC_PUBLIC, ACC_STATIC
    Code:
      stack=1, locals=4, args_size=1
         0: bipush        10
         2: istore_1
         3: iload_1
         4: iinc          1, 1
         7: istore_2
         8: iinc          1, 1
        11: iload_1
        12: istore_3
        13: return
      LineNumberTable:
        line 13: 0
        line 14: 3
        line 15: 8
        line 16: 13
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0      14     0  args   [Ljava/lang/String;
            3      11     1     i   I
            8       6     2     a   I
           13       1     3     b   I
}
SourceFile: "Main.java"
```

乍一看，是不是感觉还真的有点结构体那味？

而结构体中，有两种允许存在的数据类型，一个是无符号数，还有一个是表。

* 无符号数一般是基本数据类型，用u1、u2、u4、u8来表示，表示1个字节~8个字节的无符号数。可以表示数字、索引引用、数量值或是以UTF-8编码格式的字符串。
* 表包含多个无符号数，并且以"_info"结尾。

我们首先从最简的开始看起。

![image-20220223164126100](https://tva1.sinaimg.cn/large/e6c9d24egy1gznjcb9bipj21ro0iutfs.jpg)

首先，我们可以看到，前4个字节（共32位）组成了魔数（其实就是表示这个文件是一个JVM可以运行的字节码文件，除了Java以外，其他某些文件中也采用了这种魔数机制来进行区分，这种方式比直接起个文件扩展名更安全）

字节码文件的魔数为：CAFEBABE（这名字能想出来也是挺难的了，毕竟4个bit位只能表示出A-F这几个字母）

紧接着魔数的后面4个字节存储的是字节码文件的版本号，注意前两个是次要版本号（现在基本都不用了，都是直接Java8、Java9这样命名了），后面两个是主要版本号，这里我们主要看主版本号，比如上面的就是34，注意这是以16进制表示的，我们把它换算为10进制后，得到的结果为：`34  ->  3*16 + 4 = 52`，其中`52`代表的是`JDK8`编译的字节码文件（51是JDK7、50是JDK6、53是JDK9，以此类推）

JVM会根据版本号决定是否能够运行，比如JDK6只能支持版本号为1.1~6的版本，也就是说必须是Java6之前的环境编译出来的字节码文件，否则无法运行。又比如我们现在安装的是JDK8版本，它能够支持的版本号为1.1~8，那么如果这时我们有一个通过Java7编译出来的字节码文件，依然是可以运行的，所以说Java版本是向下兼容的。

紧接着，就是类的常量池了，这里面存放了类中所有的常量信息（注意这里的常量并不是指我们手动创建的final类型常量，而是程序运行一些需要用到的常量数据，比如字面量和符号引用等）由于常量的数量不是确定的，所以在最开始的位置会存放常量池中常量的数量（是从1开始计算的，不是0，比如这里是18，翻译为10进制就是24，所以实际上有23个常量）

接着再往下，就是常量池里面的数据了，每一项常量池里面的数据都是一个表，我们可以看到他们都是以_info结尾的：

![image-20220223171746645](https://tva1.sinaimg.cn/large/e6c9d24egy1gznkh0jr31j21800u07dm.jpg)

我们来看看一个表中定义了哪些内容：

![image-20220223172031889](https://tva1.sinaimg.cn/large/e6c9d24egy1gznkh14d4rj21b805wt9v.jpg)

首先上来就会有一个1字节的无符号数，它用于表示当前常量的类型（常量类型有很多个）这里只列举一部分的类型介绍：

|           类型            | 标志 |                             描述                             |
| :-----------------------: | :--: | :----------------------------------------------------------: |
|    CONSTANT_Utf8_info     |  1   |                    UTF-8编码格式的字符串                     |
|   CONSTANT_Integer_info   |  3   | 整形字面量（第一章我们演示的很大的数字，实际上就是以字面量存储在常量池中的） |
|    CONSTANT_Class_info    |  7   |                      类或接口的符号引用                      |
|   CONSTANT_String_info    |  8   |                      字符串类型的字面量                      |
|  CONSTANT_Fieldref_info   |  9   |                        字段的符号引用                        |
|  CONSTANT_Methodref_info  |  10  |                        方法的符号引用                        |
| CONSTANT_MethodType_info  |  16  |                           方法类型                           |
| CONSTANT_NameAndType_info |  12  |                   字段或方法的部分符号引用                   |

实际上这些东西，虽然我们不知道符号引用是什么东西，我们可以观察出来，这些东西或多或少都是存放类中一些名称、数据之类的东西。

比如我们来看第一个`CONSTANT_Methodref_info`表中存放了什么数据，这里我只列出它的结构表（详细的结构表可以查阅《深入理解Java虚拟机 第三版》中222页总表）：

|          常量           | 项目  | 类型 |                        描述                         |
| :---------------------: | :---: | :--: | :-------------------------------------------------: |
| CONSTANT_Methodref_info |  tag  |  u1  |                       值为10                        |
|                         | index |  u2  |   指向声明方法的类描述父CONSTANT_Class_info索引项   |
|                         | index |  u2  | 指向名称及类型描述符CONSTANT_NameAndType_info索引项 |

比如我们刚刚的例子中：

![image-20220223190659053](https://tva1.sinaimg.cn/large/e6c9d24ely1gznnkpf7cqj21b40503zi.jpg)

可以看到，第一个索引项指向了第3号常量，我们来看看三号常量：

![image-20220223190957382](https://tva1.sinaimg.cn/large/e6c9d24ely1gznnmsuh1pj219w03amxj.jpg)

|        常量         | 项目  | 类型 |           描述           |
| :-----------------: | :---: | :--: | :----------------------: |
| CONSTANT_Class_info |  tag  |  u1  |          值为7           |
|                     | index |  u2  | 指向全限定名常量项的索引 |

那么我们接着来看23号常量又写的啥：

![image-20220223191325689](https://tva1.sinaimg.cn/large/e6c9d24ely1gznnqfknqaj21fo0j6te5.jpg)

可以看到指向的UTF-8字符串值为`java/lang/Object`这下搞明白了，首先这个方法是由Object类定义的，那么接着我们来看第二项u2 `name_and_type_index`，指向了21号常量，也就是字段或方法的部分符号引用：

![image-20220223191921550](https://tva1.sinaimg.cn/large/e6c9d24ely1gzno0zakf9j21eg0qyqbl.jpg)

|           常量            | 项目  | 类型 |               描述               |
| :-----------------------: | :---: | :--: | :------------------------------: |
| CONSTANT_NameAndType_info |  tag  |  u1  |              值为12              |
|                           | index |  u2  |  指向字段或方法名称常量项的索引  |
|                           | index |  u2  | 指向字段或方法描述符常量项的索引 |

其中第一个索引就是方法的名称，而第二个就是方法的描述符，描述符明确了方法的参数以及返回值类型，我们分别来看看4号和5号常量：

![image-20220223192332068](https://tva1.sinaimg.cn/large/e6c9d24ely1gzno0z1yp1j21eg0qyqbl.jpg)

可以看到，方法名称为"<init>"，一般构造方法的名称都是<init>，普通方法名称是什么就是什么，方法描述符为"()V"，表示此方法没有任何参数，并且返回值类型为void，描述符对照表如下：

![image-20220223192518999](https://tva1.sinaimg.cn/large/e6c9d24ely1gzno2stssaj216i08mjsr.jpg)

比如这里有一个方法`public int test(double a, char c){ ... }`，那么它的描述符就应该是：`(DC)I`，参数依次放入括号中，括号右边是返回值类型。再比如`public String test(Object obj){ ... }`，那么它的描述符就应该是：`(Ljava/lang/Object;)Ljava/lang/String`，注意如果参数是对象类型，那么必须在后面添加`;`

对于数组类型，只需要在类型最前面加上`[`即可，有几个维度，就加几个，比如`public void test(int[][] arr)`，参数是一个二维int类型数组，那么它的描述符为：`([[I)V`

所以，这里表示的，实际上就是此方法是一个无参构造方法，并且是属于Object类的。那么，为什么这里需要Object类构造方法的符号引用呢？还记得我们在JavaSE中说到的，每个类都是直接或间接继承自Object类，所有类的构造方法，必须先调用父类的构造方法，但是如果父类存在无参构造，默认可以不用显示调用`super`关键字（当然本质上是调用了的）。

所以说，当前类因为没有继承自任何其他类，那么就默认继承的Object类，所以，在当前类的默认构造方法中，调用了父类Object类的无参构造方法，因此这里需要符号引用的用途显而易见，就是因为需要调用Object类的无参构造方法。

我们可以在反编译结果中的方法中看到：

```
public com.test.Main();
    descriptor: ()V
    flags: ACC_PUBLIC
    Code:
      stack=1, locals=1, args_size=1
         0: aload_0
         1: invokespecial #1                  // Method java/lang/Object."<init>":()V
         4: return
      LineNumberTable:
        line 11: 0
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0       5     0  this   Lcom/test/Main;
```

其中`invokespecial`（调用父类构造方法）指令的参数指向了1号常量，而1号常量正是代表的Object类的无参构造方法，虽然饶了这么大一圈，但是过程理清楚，还是很简单的。

虽然我们可以直接查看16进制的结果，但是还是不够方便，但是我们也不能每次都去使用`javap`命令，所以我们这里安装一个IDEA插件，来方便我们查看字节码中的信息，名称为`jclasslib Bytecode Viewer` ：

![image-20220223194128297](https://tva1.sinaimg.cn/large/e6c9d24ely1gznojlqgl3j216y0dc0u0.jpg)

安装完成后，我们可以在我们的IDEA右侧看到它的板块，但是还没任何数据，那么比如现在我们想要查看Main类的字节码文件时，可以这样操作：

![image-20220223194410699](https://tva1.sinaimg.cn/large/e6c9d24ely1gznomfiqu8j22ll0u0tfa.jpg)

首先在项目中选中我们的Main类，然后点击工具栏的视图，然后点击`Show Bytecode With Jclasslib`，这样右侧就会出现当前类的字节码解析信息了。注意如果修改了类的话，那么需要你点击运行或是构建，然后点击刷新按钮来进行更新。

接着我们来看下一个内容，在常量池之后，紧接着就是访问标志，访问标志就是类的种类以及类上添加的一些关键字等内容：

![image-20220223194942810](https://tva1.sinaimg.cn/large/e6c9d24ely1gznos6c7j9j21e60giq7s.jpg)

可以看到它只占了2个字节，那么它是如何表示访问标志呢?

![image-20220223200619811](https://tva1.sinaimg.cn/large/e6c9d24ely1gznp9glonej216i0hcjui.jpg)

比如我们这里的Main类，它是一个普通的class类型，并且访问权限为public，那么它的访问标志值是这样计算的：

`ACC_PUBLIC | ACC_SUPER = 0x0001 | 0x0020 = 0x0021`（这里进行的是按位或运算），可以看到和我们上面的结果是一致的。

再往下就是类索引、父类索引、接口索引：

![image-20220223200054866](https://tva1.sinaimg.cn/large/e6c9d24ely1gznp3uofdej219803q0t7.jpg)

可以看到它们的值也是指向常量池中的值，其中2号常量正是存储的当前类信息，3号常量存储的是父类信息，这里就不再倒推回去了，由于没有接口，所以这里接口数量为0，如果不为0还会有一个索引表来引用接口。

接着就是字段和方法表集合了：

![image-20220223200521912](https://tva1.sinaimg.cn/large/e6c9d24ely1gznp8gd1nfj21ai04mdgp.jpg)

由于我们这里没有声明任何字段，所以我们先给Main类添加一个字段再重新加载一下：

```java
public class Main {

    public static int a = 10;

    public static void main(String[] args) {
        int i = 10;
        int a = i++;
        int b = ++i;
    }
}
```

![image-20220223200733342](https://tva1.sinaimg.cn/large/e6c9d24ely1gznpbh3k7rj21bi06o3zn.jpg)

现在字节码就新增了一个字段表，这个字段表实际上就是我们刚刚添加的成员字段`a`的数据。

可以看到一共有四个2字节的数据：

![image-20220223200939786](https://tva1.sinaimg.cn/large/e6c9d24ely1gznpcxjzgfj216o06et9o.jpg)

首先是`access_flags`，这个与上面类标志的计算规则是一样的，表还是先列出来吧：

![image-20220223201053780](https://tva1.sinaimg.cn/large/e6c9d24ely1gznpe7is4wj21620eswh4.jpg)

第二个数据`name_index`表示字段的名称常量，这里指向的是5号常量，那么我们来看看5号常量是不是字段名称：

![image-20220223201327180](https://tva1.sinaimg.cn/large/e6c9d24ely1gznpgw09wjj21bc0tuk0x.jpg)

没问题，这里就是`a`,下一个是`descirptor_index`，存放的是描述符，不过这里因为不是方法而是变量，所以描述符直接写对应类型的标识字符即可，比如这里是`int`类型，那么就是`I`。

最后，`attrbutes_count`属性计数器，用于描述一些额外信息，这里我们暂时不做介绍。

接着就是我们的方法表了：

![image-20220223202153955](https://tva1.sinaimg.cn/large/e6c9d24ely1gznppnxpcqj21ai04odgx.jpg)

可以看到方法表中一共有三个方法，其中第一个方法我们刚刚已经介绍过了，它的方法名称为`<init>`，表示它是一个构造方法，我们看到最后一个方法名称为`<clinit>`，这个是类在初始化时会调用的方法（是隐式的，自动生成的），它主要是用于静态变量初始化语句和静态块的执行，因为我们这里给静态成员变量a赋值为10，所以会在一开始为其赋值：

![image-20220223202515287](https://tva1.sinaimg.cn/large/e6c9d24ely1gznpt5dhg3j224c0katcg.jpg)

而第二个方法，就是我们的`main`方法了，但是现在我们先不急着去看它的详细实现过程，我们来看看它的属性表。

属性表实际上类中、字段中、方法中都可以携带自己的属性表，属性表存放的正是我们的代码、本地变量等数据，比如main方法就存在4个本地变量，那么它的本地变量存放在哪里呢：

![image-20220223202955858](https://tva1.sinaimg.cn/large/e6c9d24ely1gznpy0i9ehj21by0hywii.jpg)

可以看到，属性信息呈现套娃状态，在此方法中的属性包括了一个Code属性，此属性正是我们的Java代码编译之后变成字节码指令，然后存放的地方，而在此属性中，又嵌套了本地变量表和源码行号表。

可以看到code中存放的就是所有的字节码指令：

![image-20220223203241262](https://tva1.sinaimg.cn/large/e6c9d24ely1gznq0wqe4xj215a0bi76l.jpg)

这里我们暂时不对字节码指令进行讲解（其实也用不着讲了，都认识的差不多了）。我们接着来看本地变量表，这里存放了我们方法中要用到的局部变量：

![image-20220223203356129](https://tva1.sinaimg.cn/large/e6c9d24ely1gznq26f7rhj219w0ekq5v.jpg)

可以看到一共有四个本地变量，而第一个变量正是main方法的形参`String[] args`，并且表中存放了本地变量的长度、名称、描述符等内容。当然，除了我们刚刚认识的这几个属性之外，完整属性可以查阅《深入理解Java虚拟机 第三版》231页。

最后，类也有一些属性：

![image-20220223203835282](https://tva1.sinaimg.cn/large/e6c9d24ely1gznq712n66j21dw0n20xw.jpg)

此属性记录的是源文件名称。

这样，我们对一个字节码文件的认识差不多就结束了，在了解了字节码文件的结构之后，是不是感觉豁然开朗？

***

### 字节码指令

虚拟机的指令是由一个字节长度的、代表某种特定操作含义的数字（操作码，类似于机器语言），操作后面也可以携带0个或多个参数一起执行。我们前面已经介绍过了，JVM实际上并不是面向寄存器架构的，而是面向操作数栈，所以大多数指令都是不带参数的。

由于之前已经讲解过大致运行流程，这里我们就以当前的Main类中的main方法作为教材进行讲解：

```java
public static void main(String[] args) {
    int i = 10;
    int a = i++;
    int b = ++i;
}
```

可以看到，main方法中首先是定义了一个int类型的变量i，并赋值为10，然后变量a接收`i++`的值，变量b接收`++i`的值。

那么我们来看看编译成字节码之后，是什么样的：

![image-20220223205928901](https://tva1.sinaimg.cn/large/e6c9d24ely1gznqsryzgfj225c0lgq6o.jpg)

* 首先第一句，`bipush`，将10送至操作数栈顶。
* 接下来将操作数栈顶的数值存进1号本地变量，也就是变量i中。
* 接着将变量i中的值又丢向操作数栈顶
* 这里使用`iinc`指令，将1号本地变量的值增加1（结束之后i的值就是11了）
* 接着将操作数栈顶的值（操作数栈顶的值是10）存入2号本地变量（这下彻底知道i++到底干了啥才会先返回后自增了吧，从原理角度来说，实际上i是先自增了的，但由于这里取的是操作数栈中的值，所以说就得到了i之前的值）
* 接着往下，我们看到++i是先直接将i的值自增1
* 然后在将其值推向操作数栈顶

![image-20220223214441621](https://tva1.sinaimg.cn/large/e6c9d24ely1gzns3syhe7j21x8090q5k.jpg)

而从结果来看，`i++`操作确实是先返回再自增的，而字节码指令层面来说，却是截然相反的，只是结果一致罢了。

***

### ASM字节码编程

既然字节码文件结构如此清晰，那么我们能否通过编程，来直接创建一个字节码文件呢？如果我们可以直接编写一个字节码文件，那么我们就可以省去编译的过程。ASM（某些JDK中内置）框架正是用于支持字节码编程的框架。

比如现在我们需要创建一个普通的Main类（暂时不写任何内容）

首先我们来看看如何通过编程创建一个Main类的字节码文件：

```java
public class Main {
    public static void main(String[] args) {
        ClassWriter writer = new ClassWriter(ClassWriter.COMPUTE_MAXS);
    }
}
```

首先需要获取`ClassWriter`对象，我们可以使用它来编辑类的字节码文件，在构造时需要传入参数：

* 0     这种方式不会自动计算操作数栈和局部临时变量表大小，需要自己手动来指定
* ClassWriter.COMPUTE_MAXS(1)     这种方式会自动计算上述操作数栈和局部临时变量表大小，但需要手动触发。
* ClassWriter.COMPUTE_FRAMES(2)   这种方式不仅会计算上述操作数栈和局部临时变量表大小，而且会自动计算StackMapFrames

这里我们使用`ClassWriter.COMPUTE_MAXS`即可。

接着我们首先需要指定类的一些基本信息：

```java
public class Main {
    public static void main(String[] args) {
        ClassWriter writer = new ClassWriter(ClassWriter.COMPUTE_MAXS);
        //因为这里用到的常量比较多，所以说直接一次性静态导入：import static jdk.internal.org.objectweb.asm.Opcodes.*;
        writer.visit(V1_8, ACC_PUBLIC,"com/test/Main", null, "java/lang/Object",null);
    }
}
```

这里我们将字节码文件的版本设定位Java8，然后修饰符设定为`ACC_PUBLIC`代表`public class Main`，类名称注意要携带包名，标签设置为`null`，父类设定为Object类，然后没有实现任何接口，所以说最后一个参数也是`null`。

接着，一个简答的类字节码文件就创建好了，我们可以尝试将其进行保存：

```java
public class Main {
    public static void main(String[] args) {
        ClassWriter writer = new ClassWriter(ClassWriter.COMPUTE_MAXS);
        writer.visit(V1_8, ACC_PUBLIC,"com/test/Main", null, "java/lang/Object",null);
        //调用visitEnd表示结束编辑
        writer.visitEnd();

        try(FileOutputStream stream = new FileOutputStream("./Main.class")){
            stream.write(writer.toByteArray());  //直接通过ClassWriter将字节码文件转换为byte数组，并保存到根目录下
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```

可以看到，在IDEA中反编译的结果为：

```java
package com.test;

public class Main {
}
```

我们知道，正常的类在编译之后，如果没有手动添加构造方法，那么会自带一个无参构造，但是我们这个类中还没有，所以我们来手动添加一个无参构造方法：

```java
//通过visitMethod方法可以添加一个新的方法
writer.visitMethod(ACC_PUBLIC, "<init>", "()V", null, null);
```

可以看到反编译的结果中已经存在了我们的构造方法：

```java
package com.test;

public class Main {
    public Main() {
    }
}
```

但是这样是不合法的，因为我们的构造方法还没有添加父类构造方法调用，所以说我们还需要在方法中添加父类构造方法调用指令：

```
public com.test.Main();
    descriptor: ()V
    flags: ACC_PUBLIC
    Code:
      stack=1, locals=1, args_size=1
         0: aload_0
         1: invokespecial #1                  // Method java/lang/Object."<init>":()V
         4: return
      LineNumberTable:
        line 11: 0
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0       5     0  this   Lcom/test/Main;
```

我们需要对方法进行详细编辑：

```java
//通过MethodVisitor接收返回值，进行进一步操作
MethodVisitor visitor = writer.visitMethod(ACC_PUBLIC, "<init>", "()V", null, null);
//开始编辑代码
visitor.visitCode();

//Label用于存储行号
Label l1 = new Label();
//当前代码写到哪行了，l1得到的就是多少行
visitor.visitLabel(l1);
//添加源码行数对应表（其实可以不用）
visitor.visitLineNumber(11, l1);

//注意不同类型的指令需要用不同方法来调用，因为操作数不一致，具体的注释有写
visitor.visitVarInsn(ALOAD, 0);
visitor.visitMethodInsn(INVOKESPECIAL, "java/lang/Object", "<init>", "()V", false);
visitor.visitInsn(RETURN);

Label l2 = new Label();
visitor.visitLabel(l2);
//添加本地变量表，这里加的是this关键字，但是方法中没用到，其实可以不加
visitor.visitLocalVariable("this", "Lcom/test/Main;", null, l1, l2, 0);

//最后设定最大栈深度和本地变量数
visitor.visitMaxs(1, 1);
//结束编辑
visitor.visitEnd();
```

我们可以对编写好的class文件进行反编译，看看是不是和IDEA编译之后的结果差不多：

```
{
  public com.test.Main();
    descriptor: ()V
    flags: ACC_PUBLIC
    Code:
      stack=1, locals=1, args_size=1
         0: aload_0
         1: invokespecial #8                  // Method java/lang/Object."<init>":()V
         4: return
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0       5     0  this   Lcom/test/Main
      LineNumberTable:
        line 11: 0
}
```

可以看到和之前的基本一致了，到此为止我们构造方法就编写完成了，接着我们来写一下main方法，一会我们就可以通过main方法来运行Java程序了。比如我们要编写这样一个程序：

```java
public static void main(String[] args) {
    int a = 10;
    System.out.println(a);
}
```

看起来很简单的一个程序对吧，但是我们如果手动去组装指令，会极其麻烦！首先main方法是一个静态方法，并且方法是public权限，然后还有一个参数`String[] args`，所以说我们这里要写的内容有点小多：

```java
//开始安排main方法
MethodVisitor v2 = writer.visitMethod(ACC_PUBLIC | ACC_STATIC, "main", "([Ljava/lang/String;)V", null, null);
v2.visitCode();
//记录起始行信息
Label l3 = new Label();
v2.visitLabel(l3);
v2.visitLineNumber(13, l3);

//首先是int a = 10的操作，执行指令依次为：
// bipush 10     将10推向操作数栈顶
// istore_1      将操作数栈顶元素保存到1号本地变量a中
v2.visitIntInsn(BIPUSH, 10);
v2.visitVarInsn(ISTORE, 1);
Label l4 = new Label();
v2.visitLabel(l4);
//记录一下行信息
v2.visitLineNumber(14, l4);

//这里是获取System类中的out静态变量（PrintStream接口），用于打印
v2.visitFieldInsn(GETSTATIC, "java/lang/System", "out", "Ljava/io/PrintStream;");
//把a的值取出来
v2.visitVarInsn(ILOAD, 1);
//调用接口中的抽象方法println
v2.visitMethodInsn(INVOKEVIRTUAL, "java/io/PrintStream", "println", "(I)V", false);

//再次记录行信息
Label l6 = new Label();
v2.visitLabel(l6);
v2.visitLineNumber(15, l6);

v2.visitInsn(RETURN);
Label l7 = new Label();
v2.visitLabel(l7);

//最后是本地变量表中的各个变量
v2.visitLocalVariable("args", "[Ljava/lang/String;", null, l3, l7, 0);
v2.visitLocalVariable("a", "I", null, l4, l7, 1);
v2.visitMaxs(1, 2);
//终于OK了
v2.visitEnd();
```

可以看到，虽然很简单的一个程序，但是如果我们手动去编写字节码，实际上是非常麻烦的，但是要实现动态代理之类的操作（可以很方便地修改字节码创建子类），是不是感觉又Get到了新操作（其实Spring实现动态代理的CGLib框架底层正是调用了ASM框架来实现的），所以说了解一下还是可以的，不过我们自己肯定是没多少玩这个的机会了。

***

## 类加载机制

现在，我们已经了解了字节码文件的结构，以及JVM如何对内存进行管理，现在只剩下最后一个谜团等待解开了，也就是我们的类字节码文件到底是如何加载到内存中的，加载之后又会做什么事情。

### 类加载过程

首先，要加载一个类，一定是出于某种目的的，比如我们要运行我们的Java程序，那么就必须要加载主类才能运行主类中的主方法，又或是我们需要加载数据库驱动，那么可以通过反射来将对应的数据库驱动类进行加载。

所以，一般在这些情况下，如果类没有被加载，那么会被自动加载：

* 使用new关键字创建对象时
* 使用某个类的静态成员（包括方法和字段）的时候（当然，final类型的静态字段有可能在编译的时候被放到了当前类的常量池中，这种情况下是不会触发自动加载的）
* 使用反射对类信息进行获取的时候（之前的数据库驱动就是这样的）
* 加载一个类的子类时
* 加载接口的实现类，且接口带有`default`的方法默认实现时

比如这种情况，那么需要用到另一个类中的成员字段，所以就必须将另一个类加载之后才能访问：

```java
public class Main {
    public static void main(String[] args) {
        System.out.println(Test.str);
    }

    public static class Test{
        static {
            System.out.println("我被初始化了！");
        }

        public static String str = "都看到这里了，不给个三连+关注吗？";
    }
}
```

这里我们就演示一个不太好理解的情况，我们现在将静态成员变量修改为final类型的：

```java
public class Main {
    public static void main(String[] args) {
        System.out.println(Test.str);
    }

    public static class Test{
        static {
            System.out.println("我被初始化了！");
        }

        public final static String str = "都看到这里了，不给个三连+关注吗？";
    }
}
```

可以看到，在主方法中，我们使用了Test类的静态成员变量，并且此静态成员变量是一个final类型的，也就是说不可能再发生改变。那么各位觉得，Test类会像上面一样被初始化吗？

按照正常逻辑来说，既然要用到其他类中的字段，那么肯定需要加载其他类，但是这里我们结果发现，并没有对Test类进行加载，那么这是为什么呢？我们来看看Main类编译之后的字节码指令就知道了：

![image-20220224131511381](https://tva1.sinaimg.cn/large/e6c9d24ely1gzoizzv7azj227c0lcjvp.jpg)

很明显，这里使用的是`ldc`指令从常量池中将字符串取出并推向操作数栈顶，也就是说，在编译阶段，整个`Test.str`直接被替换为了对应的字符串（因为final不可能发生改变的，编译就会进行优化，直接来个字符串比你去加载类在获取快得多不是吗，反正结果都一样），所以说编译之后，实际上跟Test类半毛钱关系都没有了。

所以说，当你在某些情况下疑惑为什么类加载了或是没有加载时，可以从字节码指令的角度去进行分析，一般情况下，只要遇到`new`、`getstatic`、`putstatic`、`invokestatic`这些指令时，都会进行类加载，比如：

![image-20220224132029992](https://tva1.sinaimg.cn/large/e6c9d24ely1gzoj5isswmj22520j877u.jpg)

这里很明显，是一定会将Test类进行加载的。除此之外，各位也可以试试看数组的定义会不会导致类被加载。

好了，聊完了类的加载触发条件，我们接着来看一下类的详细加载流程。

![image-20220224132621764](https://tva1.sinaimg.cn/large/e6c9d24ely1gzojblu4woj21380jkjtf.jpg)

首先类的生命周期一共有7个阶段，而首当其冲的就是加载，加载阶段需要获取此类的二进制数据流，比如我们要从硬盘中读取一个class文件，那么就可以通过文件输入流来获取类文件的`byte[]`，也可以是其他各种途径获取类文件的输入流，甚至网络传输并加载一个类也不是不可以。然后交给类加载器进行加载（类加载器可以是JDK内置的，也可以是开发者自己撸的，后面会详细介绍）类的所有信息会被加载到方法区中，并且在堆内存中会生成一个代表当前类的Class类对象（那么思考一下，同一个Class文件加载的类，是唯一存在的吗？），我们可以通过此对象以及反射机制来访问这个类的各种信息。

数组类要稍微特殊一点，通过前面的检验，我没发现数组在创建后是不会导致类加载的，数组类型本身不会通过类加载器进行加载的，不过你既然要往里面丢对象进去，那最终依然是要加载类的。

接着我们来看验证阶段，验证阶段相当于是对加载的类进行一次规范校验（因为一个类并不一定是由我们使用IDEA编译出来的，有可能是像我们之前那样直接用ASM框架写的一个），如果说类的任何地方不符合虚拟机规范，那么这个类是不会验证通过的，如果没有验证机制，那么一旦出现危害虚拟机的操作，整个程序会出现无法预料的后果。

验证阶段，首先是文件格式的验证：

* 是否魔数为CAFEBABE开头。
* 主、次版本号是否可以由当前Java虚拟机运行
* Class文件各个部分的完整性如何。
* ...

有关类验证的详细过程，可以参考《深入理解Java虚拟机 第三版》268页。

接下来就是准备阶段了，这个阶段会为类变量分配内存，并为一些字段设定初始值，注意是系统规定的初始值，不是我们手动指定的初始值。

再往下就是解析阶段，此阶段是将常量池内的符号引用替换为直接引用的过程，也就是说，到这个时候，所有引用变量的指向都是已经切切实实地指向了内存中的对象了。

到这里，链接过程就结束了，也就是说这个时候类基本上已经完成大部分内容的初始化了。

最后就是真正的初始化阶段了，从这里开始，类中的Java代码部分，才会开始执行，还记得我们之前介绍的`<clinit>`方法吗，它就是在这个时候执行的，比如我们的类中存在一个静态成员变量，并且赋值为10，或是存在一个静态代码块，那么就会自动生成一个`<clinit>`方法来进行赋值操作，但是这个方法是自动生成的。

全部完成之后，我们的类就算是加载完成了。

***

### 类加载器

Java提供了类加载器，以便我们自己可以更好地控制类加载，我们可以自定义类加载器，也可以使用官方自带的类加载器去加载类。对于任意一个类，都必须由加载它的类加载器和这个类本身一起共同确立其在Java虚拟机中的唯一性。

也就是说，一个类可以由不同的类加载器加载，并且，不同的类加载器加载的出来的类，即使来自同一个Class文件，也是不同的，只有两个类来自同一个Class文件并且是由同一个类加载器加载的，才能判断为是同一个。默认情况下，所有的类都是由JDK自带的类加载器进行加载。

比如，我们先创建一个Test类用于测试：

```java
package com.test;

public class Test {
    
}
```

接着我们自己实现一个ClassLoader来加载我们的Test类，同时使用官方默认的类加载器来加载：

```java
public class Main {
    public static void main(String[] args) throws ReflectiveOperationException {
        Class<?> testClass1 = Main.class.getClassLoader().loadClass("com.test.Test");
        CustomClassLoader customClassLoader = new CustomClassLoader();
        Class<?> testClass2 = customClassLoader.loadClass("com.test.Test");

     	  //看看两个类的类加载器是不是同一个
        System.out.println(testClass1.getClassLoader());
        System.out.println(testClass2.getClassLoader());
				
      	//看看两个类是不是长得一模一样
        System.out.println(testClass1);
        System.out.println(testClass2);

      	//两个类是同一个吗？
        System.out.println(testClass1 == testClass2);
      
      	//能成功实现类型转换吗？
        Test test = (Test) testClass2.newInstance();
    }

    static class CustomClassLoader extends ClassLoader {
        @Override
        public Class<?> loadClass(String name) throws ClassNotFoundException {
            try (FileInputStream stream = new FileInputStream("./target/classes/"+name.replace(".", "/")+".class")){
                byte[] data = new byte[stream.available()];
                stream.read(data);
                if(data.length == 0) return super.loadClass(name);
                return defineClass(name, data, 0, data.length);
            } catch (IOException e) {
                return super.loadClass(name);
            }
        }
    }
}
```

通过结果我们发现，即使两个类是同一个Class文件加载的，只要类加载器不同，那么这两个类就是不同的两个类。

所以说，我们当时在JavaSE阶段讲解的每个类都在堆中有一个唯一的Class对象放在这里来看，并不完全正确，只是当前为了防止各位初学者搞混。

实际上，JDK内部提供的类加载器一共有三个，比如上面我们的Main类，其实是被AppClassLoader加载的，而JDK内部的类，都是由BootstrapClassLoader加载的，这其实就是为了实现双亲委派机制而做的。

![image-20220225132629954](https://tva1.sinaimg.cn/large/e6c9d24ely1gzpoy41z31j20wb0u040w.jpg)

有关双亲委派机制，我们在JavaSE阶段反射板块已经讲解过了，所以说这就不多做介绍了。
