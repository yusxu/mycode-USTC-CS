# Lab1实验报告

徐煜森  PB16110173

#### 实验题目：调试Linux操作系统启动过程，并找出两个重要事件

#### 实验环境：

​	Ubuntu 16.04 LTS、qemu、gdb 8.0.1、Linux内核4.15.14

#### 实验初始准备：

1. 安装qemu。

2. 从http://ftp.gnu.org/gnu/gdb/下载gdb8.0.1源代码，并在remote.c（目录为/gdb/remote.c）中添加部分代码，之后编译安装。若不对代码进行相应修改，则会在调试中报错：Remote 'g' packet reply is too long: ......）。

   在如下注释后添加代码，修改后代码：

   ```
   /* Further sanity checks, with knowledge of the architecture.  */

   /*  if (buf_len > 2 * rsa->sizeof_g_packet)

   	error (_("Remote 'g' packet reply is too long: %s"), rs->buf);

   */

   if (buf_len > 2 * rsa->sizeof_g_packet) {

   	rsa->sizeof_g_packet = buf_len;

   	for (i = 0; i < gdbarch_num_regs(gdbarch); i++) {

   		if (rsa->regs[i].pnum == -1)    

       		continue;

   		if (rsa->regs[i].offset >= rsa->sizeof_g_packet)

       		rsa->regs[i].in_g_packet = 0;

      		else	rsa->regs[i].in_g_packet = 1;

   	}
   }
   //.......
   ```

3. 从www.kernel.org下载Linux最新内核4.15.14，解压编译。之后从相应目录下分别拷贝出bzImage、vmlinux、initramfs-4.15.12.img。

4. 使用命令

   ```
   qemu-system-x86_64 -kernel ./bzImage -initrd ./initrd.img -gdb tcp::1234  -S
   ```

   尝试启动。

   关于命令的参数： -kernel指定大内核文件、-initrd指定一个initrd.img文件、-gdb启动qemu内嵌的gdbserver、-S挂起gdbserver使qemu启动后暂停。

5. 启动后发现qemu花屏，去掉参数-initrd后可正常启动，所以推测是initramfs-4.15.12.img的问题，在本机中找到initrd文件，代替由内核编译而来的initramfs-4.15.12.img，可正常启动。

6. 启动发现设置断点无效，原因为内核在解压时解压至随机内存地址，在启动命令中加参数-append “nokaslr”后断点可以生效。

7. 启动后qemu报错：内存不足，在启动命令中加参数 -m 2G。

至此，可以正常使用qemu、gdb调试内核。

#### 实验过程：

1. 启动qemu：

   ```
   qemu-system-x86_64 -kernel ./bzImage -initrd ./initrd.img -append "nokaslr" -gdb tcp::1234  -S
   ```

2. 打开另一个终端，目录应为Linux内核源码所在文件夹，否则gdb调试时找不到源码，启动gdb，并连接上远程端口1234，设置断点：

   ```
   gdb vmlinux
   (gdb) target remote:1234
   (gdb) b start_kernel
   (gdb) c
   ```

3. 之后即可开始调试，使用next命令进入下一步，也可在/init/main.c中找到相应源代码，对照进行调试。

#### 实验结果：

​	内核初始化过程由start_kernel函数开始，至第一个用户进程init结束，调用一系列初始化函数对所有内核组件进行初始化。其中，start_kernel、rest_init、kernel_init、init_post为四个主要函数。

下面主要分析start_kernel()和rest_init()函数。

1. ##### start_kernel函数

   start_kernel函数是内核汇编代码和C代码的交界处，在此之前由汇编代码完成环境的初始化，把内核载入内存、设置C的运行环境。下面挑出一些重要部分分析。

   1. smp_setup_processor_id()是start_kernel中第一个调用的函数，当有多个CPU时该函数会返回启动时的CPU号，否则什么也不做。
   2. local_irq_disable()  关闭当前CPU中断。
   3. pr_notice("%s",linux_banner);  打印Linux内核版本信息。注意：这里不会立即在qemu命令行中打印，而是保存在缓冲区中，等到console_init()之后才会打印。
   4. setup_arch(&command_line)，注意：每个体系结构有不同的setup_arch（）函数，具体编译哪个函数由Makefile中arch变量决定。
   5. vf_cache_init_early()  初始化虚拟文件系统。
   6. trap_init()  函数完成对系统保留的中断向量（异常、非屏蔽中断以及系统调用）的初始化。
   7. sched_init()  函数完成进程调度器的初始化。
   8. rcu_init()  函数初始化Read-Copy Update机制。
   9. init_IRQ()  完成trap_init()之外的中断向量的初始化。
   10. init_timers()  初始化定时器相关的数据结构。
   11. hrtimers_init()  初始化高精度时钟。
   12. time_init()  初始化系统时钟源。
   13. console_init()  初始化控制台以显示之前存在缓冲区的内容。
   14. **这时可以观察到qemu中显示出许多信息：如显示内存状态 Memory:1991276K/2096632K available (10252K kernel code, 1296K rwdata, 3308K rodata, 2000K init, 1196K bss, 105356K reserved, 0K cma-reserved)。**
   15. mem_encrypt_init()  初始化内存加密。
   16. thread_stack_cache_init()  初始化线程、堆栈、缓存。
   17. fork_init()  根据物理内存大小计算允许创建进程的数量。
   18. check_bugs()  测试并记录CPU的各种缺陷，以便内核其他部分工作。

2. ##### rest_init()函数

   **start_kernel()最后调用了rest_init()函数进行后续的初始化，rest_init()函数最重要的任务就是启动内核线程kernel_init，创建1号进程。**

   1. pid = kernel_thread(kernel_init, NULL, CLONE_FS)  初始化第一个用户进程，即1号进程。
   2. arch_cpu_idle_prepare()，do_idle() 进行CPU队列进程切换，将0号进程设置为idle。

   进入idle循环以消耗CPU空闲时间片，当有其他工作要进行时可以进行抢占。

init进程负责触发其他必须的进程，使系统进入整体可用的状态，基于这种模式，init进程是其他所有进程的起源。

#### 实验小结：

​	调试start_kernel函数后面的部分比较容易，困难的地方在于如何调试start_kernel之前由汇编语言完成初始化的步骤，但是由于参考资料很少，操作也十分复杂（需要通过改内核代码，让qemu在控制台输出信息），一时间难以完成，好在实验只要求找出两个重要事件（start_kernel及之后有许多重要事件），所以可以把之前部分的调试在实验后学习完成。

​	通过本次实验，让我对Linux操作系统的启动有了初步的认识，在实验过程中也锻炼了使用Linux系统的能力。

​	最后，让qemu和gdb正常工作可以说是本次实验中最困难的部分了。



#### 7月4日更新

gdb输出调试信息

使用backtrace查看函数调用栈

![backtrace](backtrace.png)

使用info f打印帧的信息

![infof](infof.png)

