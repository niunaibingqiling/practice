# system verilog

## 1. 数据类型

### 1.1 基本数据类型

以下是基本数据类型，可以使用unsigned来将有符号的声明为无符号的。

|名称|位宽|类型|符号性|
|----|----|----|----|
|logic|1|4状态|无符号|
|bit|1|2状态|无符号|
|byte|8|2状态|有符号|
|shortint|16|2状态|有符号|
|int|32|2状态|有符号|
|longint|64|2状态|有符号|
|integer|32|4状态|有符号|
|time|64|4状态|无符号|
|real|双精度|2状态|有符号|

调用isunknown可以在表达式的任意位出现 X 或 Z 时返回1,调用$time,可以返回当前仿真时间。

```verilog
    if(isunknown(xxxx)) 
        $display("%0t",$time);
```

### 1.2 数组

如果访问访问越界地址，会返回默认值，比如，4状态元素会返回未知值x，2状态元素会返回0。wire类型没有驱动时输出高阻态z。如果访问地址索引有未知态，也会返回默认值。

数组元素使用32比特的字为边界，一个数组元素，存放在一个或多个字。对于4状态类型的数组，会比2状态类型的数组多消耗一倍的空间。

#### 1.2.1 定宽数组

定宽数组两种声明方式，使用长度或索引声明两种方式，索引从0开始。

``` verilog
    int array[L1:L2];
    int array[L2-L1];
```

#### 1.2.2 基本数组操作

##### 1.2.2.1 循环
for或foreach循环，system verilog使用$size函数获取定长数组的长度。遍历元素的两种方式。

```verilog
    // 方法1
    for(int i = 0; i < $size(array);i++) begin
        ... // 使用array[i]来访问元素
    end

    // 方法2
    foreach(array[i]) begin
        ... // 使用array[i]访问元素
    end
```

对于多维元素，$size函数只获取第一维的长度，foreach遍历使用[]加两个索引的方式。

```verilog
program test;
    initial begin
        int array[2][3];
        $display("%p",$size(array));
        foreach(array[i,j])begin
            $display(array[i][j]);
        end
    end
endprogram
```

上面代码的运行结果就是

![alt text](image-2.png)

##### 1.2.2.2 复制与比较

数组复制与比较

数组可以用数组名直接复制，也可以用数组名来进行等于和不等于的比较，不适用数值运算。

```verilog
    initial begin
        bit [31:0] array1[5] = {1,2,3,4,5};
        bit [31:0] array2[5] = {2,3,4,5,1};

        if(array1 != array2) begin
            $display("%p != %p",array1,array2);
        end
        array1 = array2;
        $display("%p == %p",array1,array2);
    end
```

数组比较和赋值的结果如下
![alt text](image-3.png)

##### 1.2.2.3 使用数组下标和位下标