# 学习system verilog标准手册--IEEE Std 1800-2009

## 1 基本构成元素

### 1.1 module

SystemVerilog中的基本块是模块，包含在关键字**module**和**endmodule**之间。模块主要用于表示设计块，但也可以用作验证代码以及验证块和设计块之间的互连的容器。模块可以包含的一些构造包括以下内容：

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

program块包含在关键字**program**和**endprogram**之间，用于对测试台环境进行建模。module构造适用于硬件描述。然而，对于testbench，重点不是硬件级别的细节，如布线、结构层次和互连，而是对验证设计的完整环境进行建模。
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

interface由**interface…endinterface**关键字界定，封装了设计模块之间以及设计与验证模块间的通信接口协议，是一种经过命名的网络或变量集合体。

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

### 1.4 checker

验证器(checker)结构由关键字**checker...endchecker**定义，是一种封装了断言和建模代码的验证功能模块。该结构的主要用途包括：(1)作为验证库的基础功能单元；(2)构建形式化验证所需的抽象辅助模型组件。

### 1.5 primitive

源语模块用于表示底层逻辑门和开关元件。SystemVerilog提供了若干内置的原语类型。设计人员可通过用户自定义原语(User Defined Primitives，UDP)对内置原语进行扩展，其语法结构由关键字**primitive...endprimitive**界定。内置原语与用户自定义原语结构支持建立时序精确的数字电路模型，该类建模通常称为门级建模。

### 1.6 子程序subroutine

**子程序**提供了一种封装可执行代码的机制，可在多处调用。子程序分为两种形式：**任务task**和**函数function**。  
**任务**以语句形式调用，可包含任意数量的输入（input）、输出（output）、双向（inout）及引用（ref）参数，但不返回值。任务执行时可阻塞仿真时间，即任务结束时可能处于比调用时刻更晚的仿真时间点。  
**函数**可返回值，亦可定义为**无返回值函数（void function）**。有返回值函数可以在表达式中作为操作数使用，而无返回值函数则以语句形式调用。函数可包含输入、输出、双向及引用参数，但必须在不阻塞仿真时间；不过，函数可以派生出能阻塞时间的子进程。

### 1.7 包package

模块（modules）、接口（interfaces）、程序块（programs）和验证器（checkers）为其内部声明提供了局部命名空间。在这些结构中声明的标识符仅在其作用域内有效，不会影响或与其他构建块的声明产生冲突。
包（packages）提供了可被其他模块、接口等构件共享的声明空间，其声明内容可通过导入机制引入其他构建块（包括其他包）。

package关键字用于定义一个包，其语法以package开始、endpackage结束。

```system verilog
package ComplexPkg; 
    typedef struct {  
        shortreal i, r; 
    } Complex;  
    function Complex add(Complex a, b); 
        add.r = a.r + b.r; 
        add.i = a.i + b.i; 
    endfunction  
    function Complex mul(Complex a, b); 
        mul.r = (a.r * b.r) - (a.i * b.i); 
        mul.i = (a.r * b.i) + (a.i * b.r); 
    endfunction 
endpackage : ComplexPkg
```

# 1.7 配置 

SystemVerilog提供了定义设计配置功能，该功能用于说明模块实例与特定SystemVerilog源代码之间的绑定关系。配置机制的实现依赖于库（library）系统。一个库是由模块（module）、接口（interface）、程序（program）、检查器（checker）、原语（primitive）、包（package）及其他配置（configure）构成的集合。通过独立的库映射文件（library map file）可精确指定各库中模块单元对应的源代码位置。在仿真器或其他需要解析SystemVerilog源代码的软件工具的启动时，此类库映射文件的命名规范通常作为调用选项。

