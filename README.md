# 简介

所谓的位数组，主要是为了有效地利用内存空间而设计的一种存储数据的方式。在这种结构中一个整数在内存中用一位(1 bit)表示。这里所谓的表示就是如果整数存在，相应的二进制位就为1，否则为0。

在嵌入式裸机开发中，ram资源是非常有限的，像一些开关变量只有0和1，经常是通过bit位来管理，c语言进行位操作是非常高效的。

但是嵌入式一般是32位系统（8位机只有8位），如果直接用一个变量进行管理时，管理最大的长度为32位，如uint32_t，有上限。所以本文定义了一个静态位数组和管理行为，方便用户实现无限位宽的管理需要。实现了常用的bit控制行为。

本项目地址：[bobwenstudy/bit_array_static (github.com)](https://github.com/bobwenstudy/bit_array_static)

项目开发过程中参考了如下两个项目：[windsgo/sliding_counter: 滑动窗口计数器，静态内存分配，基于bit-array二进制位数组 (github.com)](https://github.com/windsgo/sliding_counter)和[noporpoise/BitArray: C bit array structs and methods (github.com)](https://github.com/noporpoise/BitArray)。




# 代码结构

代码结构如下所示：

- **bit_array.h**：BitArray管理API，都是inline实现，可以根据需要转成c实现。
- **main.c**：测试例程。
- **build.mk**和**Makefile**：Makefile编译环境。
- **README.md**：说明文档

```shell
bit_array_static
 ├── bit_array.h
 ├── build.mk
 ├── main.c
 ├── Makefile
 └── README.md
```





# 使用说明

具体如何使用直接看例程就行，非常简单，看函数名和变量名即可。

默认是使用uint32_t为最小存储单元，可以通过`BIT_ARRAY_CONFIG_64`使能切换为uint64_t为最小存储单元。

## BitArray声明

如下定义了一个100bit长度的位数组，在32位系统中，实际暂用了4个uint32_t的存储空间。

```c
BIT_ARRAY_DEFINE(test_bit_array, 100);
```



## 位基本操作API

主要有以下API。

```c
static inline int bit_array_get(const bit_array_t *target, int bit);
static inline void bit_array_clear(bit_array_t *target, int bit);
static inline void bit_array_set(bit_array_t *target, int bit);
static inline void bit_array_toggle(bit_array_t *target, int bit);
static inline void bit_array_assign(bit_array_t *target, int bit, int val);
static inline void bit_array_clear_all(bit_array_t *target, int num_bits);
static inline void bit_array_set_all(bit_array_t *target, int num_bits);
static inline void bit_array_toggle_all(bit_array_t *target, int num_bits);

static inline int bit_array_num_bits_set(bit_array_t *target, int num_bits);
static inline int bit_array_num_bits_cleared(bit_array_t *target, int num_bits);
```



## 位数组copy API

主要有以下API。

```c
static inline void bit_array_copy(bit_array_t* dst, int dstindx,
                    const bit_array_t* src, int srcindx,
                    int length, int src_num_bits, int dst_num_bits);
static inline void bit_array_copy_all(bit_array_t* dst, const bit_array_t* src, int num_bits);
```





## 位数组逻辑运算API

主要有以下API。

```c
static inline void bit_array_and(bit_array_t* dest, const bit_array_t* src1, const bit_array_t* src2, int num_bits);
static inline void bit_array_or (bit_array_t* dest, const bit_array_t* src1, const bit_array_t* src2, int num_bits);
static inline void bit_array_xor(bit_array_t* dest, const bit_array_t* src1, const bit_array_t* src2, int num_bits);
static inline void bit_array_not(bit_array_t* dest, const bit_array_t* src, int num_bits);
```





## 位数组移位操作API

主要有以下API。

```c
void bit_array_shift_right(bit_array_t* target, int num_bits, int shift_dist, int fill);
void bit_array_shift_left (bit_array_t* target, int num_bits, int shift_dist, int fill);
```





# 测试说明

## 环境搭建

目前测试暂时只支持Windows编译，最终生成exe，可以直接在PC上跑。

目前需要安装如下环境：
- GCC环境，笔者用的msys64+mingw，用于编译生成exe，参考这个文章安装即可。[Win7下msys64安装mingw工具链 - Milton - 博客园 (cnblogs.com)](https://www.cnblogs.com/milton/p/11808091.html)。


## 编译说明

本项目都是由makefile组织编译的，编译整个项目只需要执行`make all`即可。


也就是可以通过如下指令来编译工程：

```shell
make all
```

而后运行执行`make run`即可运行例程，例程中实现了上述API的基本测试。

```shell
PS D:\workspace\github\bit_array_static> make run
Building   : "output/main.exe"
Start Build Image.
objcopy -v -O binary output/main.exe output/main.bin
copy from `output/main.exe' [pei-i386] to `output/main.bin' [binary]
objdump --source --all-headers --demangle --line-numbers --wide output/main.exe > output/main.lst
Print Size
   text    data     bss     dec     hex filename
  55932    7028    2644   65604   10044 output/main.exe
./output/main.exe
Testing test_assign ........................................................ pass
Testing test_set_clear ..................................................... pass
Testing test_toggle ........................................................ pass
Testing test_copy .......................................................... pass
Testing test_logic ......................................................... pass
Testing test_shift ......................................................... pass
Executing 'run: all' complete!
PS D:\workspace\github\bit_array_static>
```

可以看到，所有测试都通过。



