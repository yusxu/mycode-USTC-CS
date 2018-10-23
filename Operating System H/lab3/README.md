# 3-Betterxuys

3-Betterxuys created by Github Classroom

1. 本次实验中采用单向链表存储，链表节点存在该文件占用的第一个内存块的开头。
2. 每个内存块的第一个int型值表示与它逻辑相邻（顺序）的下一个内存块的号码。第二个int型值表示当前内存块已用多少字节（这个信息之后被发现是无用的，不会在write、truncate、read、unlink中用到）。
3. write函数为先计算出offset在哪个内存块和在内存块中位置，之后进行写操作。
4. 其实有种更简单地计算offset的方法，但是不好想，已在代码注释中写明（在truncate、read中已使用，write中使用的仍为思路简明的计算方法）。
5. 注意：truncate中size并不一定小于等于st_size。若size大于st_size，就会申请更大的空间。