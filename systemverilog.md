# 学习system verilog标准手册--IEEE Std 1800-2009

## 1 基本构成元素

### 1.1 module

SystemVerilog中的基本块是模块，包含在关键字module和endmodule之间。模块主要用于表示设计块，但也可以用作验证代码以及验证块和设计块之间的互连的容器。模块可以包含的一些构造包括以下内容：

- 端口：带有端口声明
- 数据声明，如网络、变量、结构和联合
- 常量声明
- 用户定义的类型定义
- 类定义
- 从包中导入声明
- 子例程定义
- 其他模块、接口、程序、接口、检查器和原语的实例化
- 类对象的实例化
- 连续赋值
- 程序块
- 生成块
- 指定块

```system verilog
module mux2to1 (input wire a, b, sel, // combined port and type declaration
output logic y);
always_comb begin // procedural block
if (sel) y = a; // procedural statement
else y = b;
end
endmodule: mux2to1 
```

### 1.2 program

program块包含在关键字program和endprogram之间，用于对测试台环境进行建模。module构造适用于硬件描述。然而，对于testbench，重点不是硬件级别的细节，如布线、结构层次和互连，而是对验证设计的完整环境进行建模。
program块有以下三个作用：

- 提供了一个执行testbench的入口点。
- 创建了一个范围，用于封装program范围的数据、任务和函数。
- 提供了一个范围来指定Reactive（仿真调度的特定区域）区域中的调度。

program作为设计和testbench之间的一个明确的分界线，它规定了专门的仿真执行的相关内容。与时钟块（clocking）一起，提供了设计和testbench之间的无竞争冒险，并实现了周期和事务级的抽象。

program可以包含数据声明、类定义、子例程定义、对象实例以及一个或多个initial或final过程。它不能包含always块、原语实例、module、interface或其他program实例。

SystemVerilog的抽象和建模结构简化了测试台的创建和维护。每个program块通过实例化和之间的连接使它们能够作为通用模型使用。

```system verilog
program test (input clk, input [16:1] addr, inout [7:0] data); 
    initial begin 
    ...      
endprogram
```

### 1.3 interface

interface由interface…endinterface关键字界定，封装了设计模块之间以及设计与验证模块间的通信接口协议，是一种经过命名的网络或变量集合体。

interface可在设计中实例化，并能够与其他实例化的module、接口interface以及程序program的接口里的端口进行互连。以前的design中往往存在大量由重复命名构成的端口声明与连接列表，通过采用这种分组命名的interface机制，能够显著缩减设计描述的代码规模，同时提升系统的可维护性。

接口interface支持参数化配置，可包含常量、变量、函数及任务等成员元素，其数据类型既可在内部显式声明，亦可作为参数动态传入。成员变量和函数的访问须通过接口实例名进行限定引用。通过此机制，通过interface连接的模块可直接调用接口实例的成员子程序实现通信驱动。

这种将功能逻辑封装于接口内部并与模块解耦的设计范式，使得通信协议的抽象层级或粒度调整仅需替换具有相同成员签名但实现方式不同的接口实例即可完成，而关联模块完全无需任何修改。这种设计显著提升了系统架构的可扩展性与协议升级的灵活性。

为规范module的端口的信号方向性并管控在特定module中的使用接口中的子程序，标准定义了modport构造机制。modport的方向属性始终以包含该接口实例的模块的视角进行定义。

除子程序外，接口还可内嵌进程结构（如initial或always过程块）及连续赋值assign语句，这对系统级建模与测试平台构建具有重要价值。通过该特性，接口可集成自有的协议验证机制，自动校验所有经由该接口连接的模块是否符合既定协议规范。此外，诸如功能覆盖率采集与分析、协议合规性检查及断言验证等高级应用亦可直接嵌入接口实现。

```system verilog
interface simple_bus(input logic clk); // 接口interface定义
    logic req, gnt; 
    logic [7:0] addr, data; 
    logic [1:0] mode; 
    logic start, rdy;  
endinterface: simple_bus  
module memMod(simple_bus a); // 当memMod模块在top层实例化时，a.req代表’simple_bus’接口实例sb_intf内的req信号
    always @(posedge clk) a.gnt <= a.req & avail; 
endmodule  
module cpuMod(simple_bus b); // 传入接口
    ...  
endmodule 
module top; 
    logic clk = 0;  
    simple_bus sb_intf(.clk(clk)); // 实例化interface  
    memMod mem(.a(sb_intf)); // 把interface连接到mem模块 
    cpuMod cpu(.b(sb_intf)); // 把interface连接到cpu模块
endmodule
```
