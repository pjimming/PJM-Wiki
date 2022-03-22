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