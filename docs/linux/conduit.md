#### 概念
管道类似于文件重定向，可以将前一个命令的`stdout`重定向到下一个命令的`stdin`。

---

#### 要点
1. 管道命令仅处理`stdout`，会忽略`stderr`。

2. 管道右边的命令必须能接受`stdin`。

3. 多个管道命令可以串联。

---

#### 与文件重定向的区别

文件重定向左边为命令，右边为文件。

管道左右两边均为命令，左边有`stdout`，右边有`stdin`。

---

#### 举例
统计当前目录下所有python文件的总行数，其中`find`、`xargs`、`wc`等命令可以参考[常用命令](/PJM-Wiki/linux/common_commond/)这一节内容。
```sh
find . -name '*.py' | xargs cat | wc -l
```