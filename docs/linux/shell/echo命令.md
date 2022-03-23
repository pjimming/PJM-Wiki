

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
