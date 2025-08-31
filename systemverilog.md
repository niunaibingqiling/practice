# system verilog

## 数据类型

### 基本数据类型

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

### 定宽数组

