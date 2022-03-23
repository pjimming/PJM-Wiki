## 1.概论

#### 概论

shell是我们通过命令行与操作系统沟通的语言。

shell脚本可以直接在命令行中执行，也可以将一套逻辑组织成一个文件，方便复用。

Terminal中的命令行可以看成是一个“shell脚本在逐行执行”。

Linux中常见的shell脚本有很多种，常见的有：

- Bourne Shell(`/usr/bin/sh`或`/bin/sh`)
- Bourne Again Shell(`/bin/bash`)
- C Shell(`/usr/bin/csh`)
- K Shell(`/usr/bin/ksh`)
- zsh
- …

Linux系统中一般默认使用bash，所以接下来讲解bash中的语法。

文件开头需要写`#! /bin/bash`，指明bash为脚本解释器。

#### 脚本示例

新建一个`test.sh`文件，输入一下内容：
```sh
#! bin/bash
echo "Hello World!"
```

#### 运行方式

在Terminal内输入以下命令

1.作为可执行文件
```sh
root@ubuntu:~$ chmod +x test.sh # 为test.sh文件增加可执行权限
root@ubuntu:~$ ./test.sh        # 在当前文件夹执行
HelloWorld!                     # 脚本输出
```

2.解释器执行
```sh
root@ubuntu:~$ bash test.sh
HelloWorld! # 脚本输出
```

---

## 2.注释

#### 单行注释

每行中`#`之后的内容均是注释。

```sh
# 这是一行注释

echo 'Hello World'  #  这也是注释
```

#### 多行注释

格式：
```sh
:<<EOF
第一行注释
第二行注释
第三行注释
EOF
```
其中EOF可以换成其它任意字符串。例如：
```sh
:<<abc
第一行注释
第二行注释
第三行注释
abc

:<<!
第一行注释
第二行注释
第三行注释
!
```

---

## 3.变量

#### 定义变量

定义变量，不需要加`$`符号，例如：
```sh
name1='pjm'  # 单引号定义字符串
name2="pjm"  # 双引号定义字符串
name3=pjm    # 也可以不加引号，同样表示字符串
```

---

#### 使用变量
使用变量，需要加上`$`符号，或者`${}`符号。花括号是可选的，主要为了帮助解释器识别变量边界。
```sh
name=pjm
echo $name  # 输出pjm
echo ${name}  # 输出pjm
echo ${name}666  # 输出pjm666
```

---

#### 只读变量
使用readonly或者declare可以将变量变为只读。
```sh
name=pjm
readonly name
declare -r name  # 两种写法均可

name=abc  # 会报错，因为此时name只读
```

---

#### 删除变量
unset可以删除变量。
```sh
name=pjm
unset name
echo $name  # 输出空行
```

---

#### 变量类型
1.自定义变量（局部变量）

    子进程不能访问的变量

2.环境变量（全局变量）

    子进程可以访问的变量

自定义变量改成环境变量：
```sh
root@ubuntu:~$ name=pjm  # 定义变量
root@ubuntu:~$ export name  # 第一种方法
root@ubuntu:~$ declare -x name  # 第二种方法
```
环境变量改为自定义变量：
```sh
root@ubuntu:~$ export name=pjm  # 定义环境变量
root@ubuntu:~$ declare +x name  # 改为自定义变量
```

---

#### 字符串
字符串可以用单引号，也可以用双引号，也可以不用引号。

单引号与双引号的区别：

- 单引号中的内容会原样输出，不会执行、不会取变量；

- 双引号中的内容可以执行、可以取变量；

```sh
name=pjm  # 不用引号
echo 'hello, $name \"hh\"'  # 单引号字符串，输出 hello, $name \"hh\"
echo "hello, $name \"hh\""  # 双引号字符串，输出 hello, pjm "hh"
```

获取字符串长度
```sh
name="pjm"
echo ${#name}  # 输出3
```

提取子串
```sh
name="hello, pjm"
echo ${name:0:5}  # 提取从0开始的5个字符
```

---

## 4.默认变量

#### 文件参数变量
在执行shell脚本时，可以向脚本传递参数。`$1`是第一个参数，`$2`是第二个参数，以此类推。特殊的，`$0`是文件名（包含路径）。例如：

创建文件`test.sh`：
```sh
#! /bin/bash

echo "文件名："$0
echo "第一个参数："$1
echo "第二个参数："$2
echo "第三个参数："$3
echo "第四个参数："$4
```
然后执行该脚本：
```sh
root@ubuntu:~$ chmod +x test.sh 
root@ubuntu:~$ ./test.sh 1 2 3 4
文件名：./test.sh
第一个参数：1
第二个参数：2
第三个参数：3
第四个参数：4
```

#### 其它参数相关变量

| 参数         | 说明                                                                             |
|--------------|---------------------------------------------------------------------------------|
| `$#`         | 代表文件传入的参数个数，如上例中值为4                                               |
| `$*`         | 由所有参数构成的用空格隔开的字符串，如上例中值为`"$1 $2 $3 $4"`                      |
| `$@`	       | 每个参数分别用双引号括起来的字符串，如上例中值为`"$1" "$2" "$3" "$4"`                |
| `$$`         | 脚本当前运行的进程ID                                                              |
| `$?`         | 上一条命令的退出状态（注意不是stdout，而是exit code）。0表示正常退出，其他值表示错误   |
| `$(command)` | 返回`command`这条命令的stdout（可嵌套）                                            |
| `command`    | 返回`command`这条命令的stdout（不可嵌套）                                          |

---

## 5.数组

数组中可以存放多个不同类型的值，只支持一维数组，初始化时不需要指明数组大小。

数组**下标从0开始。**

---

#### 定义

数组用小括号表示，元素之间用空格隔开。例如：
```sh
array=(1 abc "def" pjm)
```

也可以直接定义数组中某个元素的值：
```sh
array[0]=1
array[1]=abc
array[2]="def"
array[3]=pjm
```

---

#### 读取数组中某个元素的值

格式：
```sh
${array[index]}
```
例如：
```sh
array=(1 abc "def" pjm)
echo ${array[0]}
echo ${array[1]}
echo ${array[2]}
echo ${array[3]}
```

---

#### 读取整个数组

格式：
```sh
${array[@]}  # 第一种写法
${array[*]}  # 第二种写法
```

例如：
```sh
array=(1 abc "def" pjm)

echo ${array[@]}  # 第一种写法
echo ${array[*]}  # 第二种写法
```

---

#### 数组长度
类似于字符串
```sh
${#array[@]}  # 第一种写法
${#array[*]}  # 第二种写法
```

例如：
```sh
array=(1 abc "def" pjm)

echo ${#array[@]}  # 第一种写法
echo ${#array[*]}  # 第二种写法
```

---

## 6.expr命令

`expr`命令用于求表达式的值，格式为：

```
expr 表达式
```

表达式说明：

- 用空格隔开每一项
- 用反斜杠放在shell特定的字符前面（发现表达式运行错误时，可以试试转义）
- 对包含空格和其他特殊字符的字符串要用引号括起来
- expr会在`stdout`中输出结果。如果为逻辑关系表达式，则结果为真，`stdout`为1，否则为0。
- expr的`exit code`：如果为逻辑关系表达式，则结果为真，`exit code`为0，否则为1。

---

#### 字符串表达式

- `length STRING`

返回`STRING`的长度

- `index STRING CHARSET`

`CHARSET`中任意单个字符在`STRING`中最前面的字符位置，**下标从1开始**。如果在`STRING`中完全不存在`CHARSET`中的字符，则返回0。

- `substr STRING POSITION LENGTH`

返回`STRING`字符串中从POSITION开始，长度最大为LENGTH的子串。如果POSITION或LENGTH为负数，0或非数值，则返回空字符串。

示例：
```sh
str="Hello World!"

echo `expr length "$str"`  # ``不是单引号，表示执行该命令，输出12
echo `expr index "$str" aWd`  # 输出7，下标从1开始
echo `expr substr "$str" 2 3`  # 输出 ell
```

#### 整数表达式
`expr`支持普通的算术操作，算术表达式优先级低于字符串表达式，高于逻辑关系表达式。

- `+ -`

加减运算。两端参数会转换为整数，如果转换失败则报错。

- `* / %`

乘，除，取模运算。两端参数会转换为整数，如果转换失败则报错。

- `()` 可以该表优先级，但需要用反斜杠转义

示例：
```sh
a=3
b=4

echo `expr $a + $b`  # 输出7
echo `expr $a - $b`  # 输出-1
echo `expr $a \* $b`  # 输出12，*需要转义
echo `expr $a / $b`  # 输出0，整除
echo `expr $a % $b` # 输出3
echo `expr \( $a + 1 \) \* \( $b + 1 \)`  # 输出20，值为(a + 1) * (b + 1)
```

#### 逻辑关系表达式
- `|`

如果第一个参数非空且非0，则返回第一个参数的值，否则返回第二个参数的值，但要求第二个参数的值也是非空或非0，否则返回0。如果第一个参数是非空或非0时，不会计算第二个参数。

- `&`

如果两个参数都非空且非0，则返回第一个参数，否则返回0。如果第一个参为0或为空，则不会计算第二个参数。

- `< <= = == != >= >`

比较两端的参数，如果为true，则返回1，否则返回0。”==”是”=”的同义词。”expr”首先尝试将两端参数转换为整数，并做算术比较，如果转换失败，则按字符集排序规则做字符比较。

- `()` 可以该表优先级，但需要用反斜杠转义

示例：
```sh
a=3
b=4

echo `expr $a \> $b`  # 输出0，>需要转义
echo `expr $a '<' $b`  # 输出1，也可以将特殊字符用引号引起来
echo `expr $a '>=' $b`  # 输出0
echo `expr $a \<\= $b`  # 输出1

c=0
d=5

echo `expr $c \& $d`  # 输出0
echo `expr $a \& $b`  # 输出3
echo `expr $c \| $d`  # 输出5
echo `expr $a \| $b`  # 输出3
```

---

## 7.read命令

`read`命令用于从标准输入中读取单行数据。当读到文件结束符时，`exit code`为1，否则为0。

参数说明

- `-p`: 后面可以接提示信息

- `-t`：后面跟秒数，定义输入字符的等待时间，超过等待时间后会自动忽略此命令

实例：
```sh
root@ubuntu:~$ read name  # 读入name的值
PJM Wiki  # 标准输入
root@ubuntu:~$ echo $name  # 输出name的值
PJM Wiki  #标准输出
root@ubuntu:~$ read -p "Please input your name: " -t 30 name  # 读入name的值，等待时间30秒
Please input your name: PJM Wiki  # 标准输入
root@ubuntu:~$ echo $name  # 输出name的值
PJM Wiki  # 标准输出
```

---

## 8.echo命令

`echo`用于输出字符串。命令格式：
```sh
echo STRING
```

---

#### 显示普通字符串
```sh
echo "Hello PJM Wiki"
echo Hello PJM Wiki  # 引号可以省略
```

---

#### 显示转义字符
```sh
echo "\"Hello PJM Wiki\""  # 注意只能使用双引号，如果使用单引号，则不转义
echo \"Hello PJM Wiki\"  # 也可以省略双引号
```

---

#### 显示变量
```sh
name=pjm
echo "My name is $name"  # 输出 My name is pjm
```

---

#### 显示换行
```sh
echo -e "Hi\n"  # -e 开启转义
echo "PJM Wiki"
```

输出结果：
```
Hi

PJM Wiki
```

---

#### 显示不换行
```sh
echo -e "Hi \c" # -e 开启转义 \c 不换行
echo "PJM Wiki"
```

输出结果：
```
Hi PJM Wiki
```

---

#### 显示结果定向至文件
```sh
echo "Hello World" > output.txt  # 将内容以覆盖的方式输出到output.txt中
```

---

#### 原样输出字符串，不进行转义或取变量(用单引号)
```sh
name=PJM Wiki
echo '$name\"'
```

输出结果
```
$name\"
```

---

#### 显示命令的执行结果
```sh
echo `date`
```

输出结果：
```
Wed Mar 23 13:24:22 CST 2022
```

---

## 9.printf命令

`printf`命令用于格式化输出，类似于`C/C++`中的`printf`函数。

默认**不会在字符串末尾添加换行符**。

命令格式：
```sh
printf format-string [arguments...]
```

---

#### 用法示例

脚本内容：
```sh
printf "%10d.\n" 123  # 占10位，右对齐
printf "%-10.2f.\n" 123.123321  # 占10位，保留2位小数，左对齐
printf "My name is %s\n" "pjm"  # 格式化输出字符串
printf "%d * %d = %d\n"  2 3 `expr 2 \* 3` # 表达式的值作为参数
```

输出结果：
```
       123.
123.12    .
My name is pjm
2 * 3 = 6
```

---

## 10.test命令与判断符号[]

#### 逻辑运算符&&和||
- `&&` 表示与，`||` 表示或

- 二者具有短路原则：

`expr1 && expr2`：当`expr1`为假时，直接忽略`expr2`

`expr1 || expr2`：当`expr1`为真时，直接忽略`expr2`

- 表达式的`exit code`为0，表示真；为非零，表示假。**（与`C/C++`中的定义相反）**

---

#### test命令

在命令行中输入`man test`，可以查看`test`命令的用法。

`test`命令用于判断文件类型，以及对变量做比较。

`test`命令用`exit code`返回结果，而不是使用`stdout`。0表示真，非0表示假。

例如：
```sh
test 2 -lt 3  # 为真，返回值为0
echo $?  # 输出上个命令的返回值，输出0
```

```sh
root@ubuntu:~$ ls  # 列出当前目录下的所有文件
test.sh
root@ubuntu:~$ test -e test.sh && echo "exist" || echo "Not exist"
exist  # test.sh 文件存在
root@ubuntu:~$ test -e test2.sh && echo "exist" || echo "Not exist"
Not exist  # testh2.sh 文件不存在
```

---

#### 文件类型判断
命令格式：
```sh
test -e filename  # 判断文件是否存在
```

| 测试参数 | 代表意义 |
|---------|----------|
| -e | 文件是否存在 |
| -f | 是否为文件 |
| -d | 是否为目录 |

---

#### 文件权限判断
命令格式：
```sh
test -r filename  # 判断文件是否可读
```

| 测试参数 | 代表意义 |
|--|--|
-r|	文件是否可读
-w|	文件是否可写
-x|	文件是否可执行
-s|	是否为非空文件

---

#### 整数间的比较
命令格式：
```sh
test $a -eq $b  # a是否等于b
```

| 测试参数 | 代表意义 |
|--|--|
-eq|	a是否等于b
-ne|	a是否不等于b
-gt|	a是否大于b
-lt|	a是否小于b
-ge|	a是否大于等于b
-le|	a是否小于等于b

---

#### 字符串比较
测试参数|	代表意义|
|--|--|
test -z STRING|	判断STRING是否为空，如果为空，则返回true
test -n STRING|	判断STRING是否非空，如果非空，则返回true（-n可以省略）
test str1 == str2|	判断str1是否等于str2
test str1 != str2|	判断str1是否不等于str2

---

#### 多重条件判定
命令格式：
```sh
test -r filename -a -x filename
```

测试参数|	代表意义|
|--|--|
-a|	两条件是否同时成立
-o|	两条件是否至少一个成立
!|	取反。如 test ! -x file，当file不可执行时，返回true

---

#### 判断符号[]
`[]`与`test`用法几乎一模一样，更常用于`if`语句中。另外`[[]]`是`[]`的加强版，支持的特性更多。

例如：
```sh
[ 2 -lt 3 ]  # 为真，返回值为0
echo $?  # 输出上个命令的返回值，输出0
```
```sh
root@ubuntu:~$ ls  # 列出当前目录下的所有文件
test.sh
root@ubuntu:~$ [ -e test.sh ] && echo "exist" || echo "Not exist"
exist  # test.sh 文件存在
root@ubuntu:~$ [ -e test2.sh ] && echo "exist" || echo "Not exist"
Not exist  # testh2.sh 文件不存在
```
注意：

- `[]`内的每一项都要用空格隔开

- 中括号内的变量，最好用双引号括起来

- 中括号内的常数，最好用单或双引号括起来

例如：
```sh
name="PJM Wiki"
[ $name == "PJM Wiki" ]  # 错误，等价于 [ PJM Wiki == "PJM Wiki" ]，参数太多
[ "$name" == "PJM Wiki" ]  # 正确
```

---

## 11.判断语句

#### if…then形式
类似于`C/C++`中的`if-else`语句。

---

#### 单层if
命令格式：
```sh
if condition
then
    语句1
    语句2
    ...
fi
```

示例：
```sh
a=3
b=4

if [ "$a" -lt "$b" ] && [ "$a" -gt 2 ]
then
    echo ${a}在范围内
fi
```
输出结果：
```
3在范围内
```

---

#### 单层if-else
命令格式
```sh
if condition
then
    语句1
    语句2
    ...
else
    语句1
    语句2
    ...
fi
```

示例：
```sh
a=3
b=4

if ! [ "$a" -lt "$b" ]
then
    echo ${a}不小于${b}
else
    echo ${a}小于${b}
fi
```
输出结果：
```
3小于4
```

---

#### 多层if-elif-elif-else
命令格式
```sh
if condition
then
    语句1
    语句2
    ...
elif condition
then
    语句1
    语句2
    ...
elif condition
then
    语句1
    语句2
else
    语句1
    语句2
    ...
fi
```

示例：
```sh
a=4

if [ $a -eq 1 ]
then
    echo ${a}等于1
elif [ $a -eq 2 ]
then
    echo ${a}等于2
elif [ $a -eq 3 ]
then
    echo ${a}等于3
else
    echo 其他
fi
```
输出结果：
```
其他
```

---

#### case…esac形式
类似于`C/C++`中的`switch`语句。

命令格式
```sh
case $变量名称 in
    值1)
        语句1
        语句2
        ...
        ;;  # 类似于C/C++中的break
    值2)
        语句1
        语句2
        ...
        ;;
    *)  # 类似于C/C++中的default
        语句1
        语句2
        ...
        ;;
esac
```
示例：
```sh
a=4

case $a in
    1)
        echo ${a}等于1
        ;;  
    2)
        echo ${a}等于2
        ;;  
    3)                                                
        echo ${a}等于3
        ;;  
    *)
        echo 其他
        ;;  
esac
```
输出结果：
```
其他
```

---

## 12.循环语句

#### for…in…do…done
命令格式：
```sh
for var in val1 val2 val3
do
    语句1
    语句2
    ...
done
```

示例1，输出`a 2 cc`，每个元素一行：
```sh
for i in a 2 cc
do
    echo $i
done
```

示例2，输出当前路径下的所有文件名，每个文件名一行：
```sh
for file in `ls`
do
    echo $file
done
```

示例3，输出`1-10`
```sh
for i in $(seq 1 10)
do
    echo $i
done
```

示例4，使用`{1..10} 或者 {a..z}`
```sh
for i in {a..z}
do
    echo $i
done
```

---

#### for ((…;…;…)) do…done
命令格式：
```sh
for ((expression; condition; expression))
do
    语句1
    语句2
done
```

示例，输出1-10，每个数占一行：
```sh
for ((i=1; i<=10; i++))
do
    echo $i
done
```

---

#### while…do…done循环
命令格式：
```sh
while condition
do
    语句1
    语句2
    ...
done
```

示例，文件结束符为Ctrl+d，输入文件结束符后read指令返回false。
```sh
while read name
do
    echo $name
done
```

---

#### until…do…done循环
当条件为真时结束。

命令格式：
```sh
until condition
do
    语句1
    语句2
    ...
done
```

示例，当用户输入yes或者YES时结束，否则一直等待读入。
```sh
until [ "${word}" == "yes" ] || [ "${word}" == "YES" ]
do
    read -p "Please input yes/YES to stop this program: " word
done
```

---

#### break命令
跳出当前一层循环，注意与`C/C++`不同的是：`break`不能跳出`case`语句。

示例
```sh
while read name
do
    for ((i=1;i<=10;i++))
    do
        case $i in
            8)
                break
                ;;
            *)
                echo $i
                ;;
        esac
    done
done
```
该示例每读入非`EOF`的字符串，会输出一遍`1-7`。

该程序可以输入 ++ctrl+d++ 文件结束符来结束，也可以直接用 ++ctrl+c++ 杀掉该进程。

---

#### continue命令
跳出当前循环。

示例：
```sh
for ((i=1;i<=10;i++))
do
    if [ `expr $i % 2` -eq 0 ]
    then
        continue
    fi
    echo $i
done
```
该程序输出1-10中的所有奇数。

---

#### 死循环的处理方式
如果Terminal可以打开该程序，则输入 ++ctrl+c++ 即可。

否则可以直接关闭进程：

1.使用`top`命令找到进程的`PID`

2.输入`kill -9 PID`即可关掉此进程

---