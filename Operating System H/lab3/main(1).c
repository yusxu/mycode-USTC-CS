#define FUSE_USE_VERSION 26
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fuse.h>
#include <sys/mman.h>

struct filenode                         //文件节点以链表的形式存储
{
    char *filename;
    void *content;
    struct stat *st;
    struct filenode *next;
};

static const size_t size = 4 * 1024 * 1024 * (size_t)1024;      //表示大小（总共1G)
static void *mem[64 * 1024];                                    //mem表示一个指针数组，保存每个block所对应的的指针

static struct filenode *root = NULL;

static struct filenode *get_filenode(const char *name)          //寻找和name名字一致的文件节点，并将其return，找不到，返回空
//在fileattr中调用
{
    struct filenode *node = root;
    while(node)
    {
        if(strcmp(node->filename, name + 1) != 0)
            node = node->next;
        else
            return node;
    }
    return NULL;
}

static void create_filenode(const char *filename, const struct stat *st)        //创造一个新的文件节点（在mknod中调用）
{
    struct filenode *new = (struct filenode *)malloc(sizeof(struct filenode));
    //为新生成的文件节点分配空间
    new->filename = (char *)malloc(strlen(filename) + 1);
    //为filename分配空间：
    //为什么这里是长度加一？
    memcpy(new->filename, filename, strlen(filename) + 1);
    //将filenamecopy到对应的节点下面
    new->st = (struct stat *)malloc(sizeof(struct stat));
    //新节点的文件属性结构体分配空间
    memcpy(new->st, st, sizeof(struct stat));
    //将st的内容copy到结构体的节点下面
    new->next = root;
    //从前面拼接
    new->content = NULL;
    root = new;
}

static void *oshfs_init(struct fuse_conn_info *conn)
{
    size_t blocknr = sizeof(mem) / sizeof(mem[0]);  //blocknr=64*1024
                                                    //mem的大小为64*1024，size的大小为4 * 1024 * 1024 * (size_t)1024 （1G)
    size_t blocksize = size / blocknr;              //blocksize就是mem中每个指针对应的区域的大小 （每一个的大小是64kb）
    // Demo 1 第一种实现方式
    for(int i = 0; i < blocknr; i++)
    {
        mem[i] = mmap(NULL, blocksize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        //在内存中建立映射，由系统分配起始空间，长度为一个blocksize，设定模式为页内容可以被读写，一次映射64kb
        //建立私有映射或者匿名映射
        //由于设定的是匿名映射，所以文件描述词设定为1
        //被映射对象的起点为0
        memset(mem[i], 0, blocksize);
        //将每个mem所指向的区块的值全部置为0
    }
    //此处相当于完成了存储空间的初始化
    for(int i = 0; i < blocknr; i++)
    {
        munmap(mem[i], blocksize);
    }
    //用于取消对应的内存映射

    // Demo 2   第二种实现方式
    mem[0] = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    //给mem[0]分配空间
    for(int i = 0; i < blocknr; i++)
    {
        mem[i] = (char *)mem[0] + blocksize * i;
        memset(mem[i], 0, blocksize);
    }
    //每一个mem的起始地址的设置
    for(int i = 0; i < blocknr; i++)
        munmap(mem[i], blocksize);
    //取消对应的内存映射
    return NULL;
}

static int oshfs_getattr(const char *path, struct stat *stbuf)
{
    int ret = 0;
    struct filenode *node = get_filenode(path);         //调用get_filenode函数，寻找与路径一致的文件节点
    if(strcmp(path, "/") == 0)                          //????????
    {
        memset(stbuf, 0, sizeof(struct stat));
        stbuf->st_mode = S_IFDIR | 0755;                //缓冲区中的文件保护模式设定为目录
    }
    else if(node)                                       //如果节点存在，那么就把节点的文件属性copy到属性缓冲区中
        memcpy(stbuf, node->st, sizeof(struct stat));
    else                                                //若不存在，那么就返回错误
        ret = -ENOENT;
    return ret;
}

static int oshfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
//读出所有文件的信息
{
    struct filenode *node = root;
    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    while(node)
    {                                       //依次读出每个文件的信息
        filler(buf, node->filename, node->st, 0);
        node = node->next;
    }
    return 0;
}

static int oshfs_mknod(const char *path, mode_t mode, dev_t dev)            //创造一个节点
{
    struct stat st;                                                         //定义一个状态结构体
    st.st_mode = S_IFREG | 0644;                                            //保护模式定义为普通文件
    st.st_uid = fuse_get_context()->uid;
    st.st_gid = fuse_get_context()->gid;
    st.st_nlink = 1;                                                        //硬链接个数设置为1个
    st.st_size = 0;                                                         //初始的文件大小为0
    create_filenode(path + 1, &st);                                         //调用了创造文件节点的函数，并将路径和定义的st和传进去
    return 0;
}

static int oshfs_open(const char *path, struct fuse_file_info *fi)          //打开文件
{
    return 0;
}

static int oshfs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
//将缓冲区（buf）中的数据写到一个打开的文件中
{
    struct filenode *node = get_filenode(path);                     //打开文件
    node->st->st_size = offset + size;                              //修改文件的大小标志
    node->content = realloc(node->content, offset + size);          //重新为文件内容分配空间
    memcpy(node->content + offset, buf, size);                  //将缓冲区内的内容copy到新的空间内
    return size;
}

static int oshfs_truncate(const char *path, off_t size)     //用于修改文件的大小
{
    struct filenode *node = get_filenode(path);             //打开文件
    node->st->st_size = size;                               //文件的大小设定为size大小
    node->content = realloc(node->content, size);           //重新为内容分配一个大小为size的空间
    return 0;
}

static int oshfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
//从一个已经打开的文件中读出数据
{
    struct filenode *node = get_filenode(path);                     //寻找对应的节点
    int ret = size;                                                 //ret初值为读取数据的大小
    if(offset + size > node->st->st_size)                           //如果读取的数据的大小超过了文件的总大小
        ret = node->st->st_size - offset;                           //那么实际读取的数值的数量为文件大小减去偏移量
    memcpy(buf, node->content + offset, ret);                       //将文件中以offset起始的数据copy到缓冲区中
    return ret;                                                     //返回读取数据的大小
}

static int oshfs_unlink(const char *path)               //用于删除一个节点
{
    struct filenode *node1 = get_filenode(path);
    struct filenode *node2 = root;
    if (node1==root)                        //特殊处理文件为链表头的情况
    {
        root=node1->next;
        node1->next=NULL;
    }
    else if (node1)                         //若node1存在
    {
            while(node2->next!=node1&&node2!=NULL)
                    node2 = node2->next;
            node2->next=node1->next;
            node1->next=NULL;
    }
    else return 0;
    //开始删除node1节点

}

static const struct fuse_operations op = {              //不同的op所对应的函数
    .init = oshfs_init,
    .getattr = oshfs_getattr,
    .readdir = oshfs_readdir,
    .mknod = oshfs_mknod,
    .open = oshfs_open,
    .write = oshfs_write,
    .truncate = oshfs_truncate,
    .read = oshfs_read,
    .unlink = oshfs_unlink,
};

int main(int argc, char *argv[])
{
    return fuse_main(argc, argv, &op, NULL);            //调用fuse函数
}
