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