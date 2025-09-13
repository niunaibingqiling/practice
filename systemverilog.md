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

##### 1.2.3 合并数组

合并数组可以作为1个整体来访问，也可以作为数组。例如1个32bit的寄存器，可以看作4个8bit数据，也可以看作单个无符号数。

它的存放方式是连续的bit合集，中间没有没用的空间，另外，合并数组赋值时，高比特放在高索引位置。

```verilog
// 合并数组声明方式
        bit [3:0] [7:0] bytes[4];
        bytes[0] = 32'habcd_abcd;
        bytes[1] = 32'hdecb_dada;
        bytes[2] = 32'haaaa_dada;
        bytes[3] = 32'hbbbb_dada;
        foreach(bytes[i]) begin
            //$displayh(bytes[i],
            //            bytes[i][3],
            //            bytes[i][3][7]);
            // 第二种使用,, 来传入默认参数
            $displayh(bytes[i],,
                        bytes[i][3],,
                        bytes[i][3][7]);
        end
```

执行结果如下
![alt text](image-4.png)

还可以使用合并数组，来实现合并数组的等待触发，使用方法如下

```verilog
    bit [3:0][7:0] array[3]

    //
    forever begin
        // 当数组array[0]的值发生变化，再执行下一步操作
        @array[0]
        // 操作
        ...
        //
    end
```

##### 1.2.4 动态数组

为了避免定长数组所造成的空间浪费，system verilog提供了动态数组，在仿真进行时，为其分配空间或者调整空间。

动态数组在使用前，必须调用new[]操作符来分配空间，也可以把数组传递给new[]操作符，实现数组值的复制。

```verilog
    // 动态数组声明
    int darray1[],darray2[];
    int sarray[5];

    initial begin
        // 创建长度为5的动态数组
        darray1 = new[5];
        foreach (darray1[i]) begin
            std::randomize(darray1[i]);
        end
        $display("%p",darray1);
        $display("%p\n",darray2);

        // 动态数组赋值
        darray2 = darray1;
        $display("%p",darray1);
        $display("%p\n",darray2);

        // 动态数组删除
        darray2.delete();
        $display("%p",darray1);
        $display("%p\n",darray2);

        // 动态数组创建赋值
        darray2 = new[20](darray1);
        $display("%p",darray1);
        $display("%p\n",darray2);

        // 动态数组与定长数组赋值
        sarray = darray1;
        $display("%p",darray1);
        $display("%p\n",sarray);
    end

    
```

运行结果
![alt text](image-5.png)

##### 1.2.5 队列

队列可以在任何一个位置增删元素，也可以使用索引访问。当增加的元素超过了原来的队列尺寸，system verilog会分配更多的空间。队列、动态数组、定宽数组之间还可以相互赋值，但是赋值时的数组大小应一致。

```verilog
    // 定义队列
    int q1[$],q2[$] = {1,2,3,4,5};
    initial begin
        // 在0索引位置插入一个元素
        q1.insert(0,2);
        $display("%p",q1);
        // 删除0这个位置的元素
        q1.delete(0);
        $display("%p",q1);


        q2.push_front(1);
        q2.pop_back();

        q2.push_back(8);
        q2.pop_front();

        foreach(q2[i]) begin
            $display(q2[i]);
        end

        // 使用索引范围，$代表最开始的索引或最后的索引
        q2 = {q2[1:$],q2[$:3]};
        $display(q2);

        // 队列给数组赋值
        sarray = q2;
        $display(sarray);

        // 删除队列
        q2 = {};
        q2.delete();
        $display(q2);

    end
```

运行结果如下

![alt text](image-7.png)

##### 1.2.6 关联数组

system verilog提供了关联数组，来实现稀疏存储，建立高效的查找映射关系。

关联数组的定义使用在中括号里放置数据类型的形式来声明，例如[int]、[packet]，也可以使用[*]不明确类型的方式来声明，不用使用new方法来创建空间。

```verilog
    initial begin
        // 声明索引是string类型的关联数组
        int switch[string],min_address,max_address;
        string index;
        switch["min_address"] = 10;

        switch["max_address"] = 100;

        $display(switch.exists("min_address"));

        // foreach遍历关联数组
        foreach(switch[i]) begin
            $display(switch[i]);
        end

        // 在不知道索引有哪些时，可以通过first、next、last、prev方法来获取
        switch.first(index);
        $display(index);

        switch.next(index);
        $display(index);

        switch.last(index);
        $display(index);

        switch.prev(index);
        $display(index);

    end


```

执行结果如下

![alt text](image-8.png)

##### 1.2.7 数组的常用方法

这里的数组包括了定长数组、动态数组、队列和关联数组。

###### 数组缩减方法

支持数组中各个元素相互求和（sum）、积（product）、与（and）、或（or）和异或（xor）

```verilog


```
