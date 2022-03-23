`exit`命令用来退出当前`shell`进程，并返回一个退出状态；使用`$?`可以接收这个退出状态。

`exit`命令可以接受一个整数值作为参数，代表退出状态。如果不指定，默认状态值是 0。

`exit`退出状态只能是一个介于 0~255 之间的整数，其中只有 0 表示成功，其它值都表示失败。

---

示例：

创建脚本`test.sh`，内容如下：
```sh
#! /bin/bash

if [ $# -ne 1 ]  # 如果传入参数个数等于1，则正常退出；否则非正常退出。
then
    echo "arguments not valid"
    exit 1
else
    echo "arguments valid"
    exit 0
fi
```

执行该脚本：
```sh
root@ubuntu:~$ chmod +x test.sh 
root@ubuntu:~$ ./test.sh abc
arguments valid
root@ubuntu:~$ echo $?  # 传入一个参数，则正常退出，exit code为0
0
root@ubuntu:~$ ./test.sh 
arguments not valid
root@ubuntu:~$ echo $?  # 传入参数个数不是1，则非正常退出，exit code为1
1
```
