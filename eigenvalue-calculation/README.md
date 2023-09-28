# 项目情况
本项目仅是实验用作，仅归档供参考
## 项目名称
特征矩阵计算

## 项目用途
输入碱基序列，构建此碱基片段的30bp,60bp,90bp的变化特征值，以用于后续的拼接识别。

## 项目构建
参见`CMakeList.txt`

# 输入输出文件
输入类似如下
```
1
ATCCGTCGNTTTACGTACNACGTTTACGGATCA
2
ATCCGTCGNTTTACGTACNACGTTTACGGATCA
...
```

输出类似如下
```
1
ATCCGTCGNTTTACGTACNACGTTTACGGATCA
1293.92 2398.02 2482.99
-238.73 21390.22 -927.19
2
ATCCGTCGNTTTACGTACNACGTTTACGGATCA
1293.92 2398.02 2482.99
-238.73 21390.22 -927.19
...

```