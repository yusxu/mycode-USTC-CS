#define FUSE_USE_VERSION 26
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fuse.h>
#include <sys/mman.h>

#define root ((filenode*)((char*)mem[0] + 2*sizeof(int)))->next
#define find_start_at *(int*)mem[0]
//mem[0] store last found blank block num and root
//mem[1] ..... store file

#define BLOCKNR 65536
#define BLOCKSIZE 65536
// 65536 = 64 * 1024
// 65536 * 65536 = 4 * 1024 * 1024 * 1024   amazing ! wonderful !

// dan wei : byte
// each block start with 2 int num :
// int[0] for next block num   
// if(int[0] == -1) it is last block
// int[1] for block used int[0] byte

typedef struct fn {
    char *filename;
    int head_blocknum;
    int content_start_at; 
    struct stat *st;
    struct fn *next;
}filenode;

size_t blocksize = BLOCKSIZE;

static const size_t size = 4 * 1024 * 1024 * (size_t)1024;
static void *mem[BLOCKNR];

//static filenode *root = NULL;

//int find_start_at = 1; 

static filenode *get_filenode(const char *name)
{
    filenode *node = root;
    while(node) {
        if(strcmp(node->filename, name + 1) != 0)
            node = node->next;
        else
            return node;
    }
    return NULL;
}

int next_block(int blocknum){
	return *(int*)mem[blocknum];
}

int cur_block_used(int blocknum){
	return *((int*)mem[blocknum]+1);
}

int min(int a, int b){
	return (a<b)? a:b;
}

int max(int a, int b){
	return (a>b)? a:b;
}


int find_blank_block(){
	int i;
	for(i = (find_start_at + 1)%BLOCKNR ;i != find_start_at; i=(i+1)%BLOCKNR){
		if(!mem[i]){
			find_start_at = i;
			return i;
		}
	}
	return -1;
}

int init_block(int blocknum){
	mem[blocknum] = mmap(NULL, blocksize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if(!mem[blocknum]){
		printf("mmap failed!\n");
		return -1;
	}
    memset(mem[blocknum], 0, blocksize);
    *(int*)mem[blocknum] = -1;
    *((int*)mem[blocknum] + 1) = 2 * sizeof(int);
    return 1;
}

void* mymalloc(int blocknum ,int size){
	int offset = *((int*)mem[blocknum]+1);
	*((int*)mem[blocknum]+1) += size;
	return mem[blocknum] + offset;
}

static int create_filenode(const char *filename, const struct stat *st)
{
    /*filenode *new = (filenode *)malloc(sizeof(filenode));
    new->filename = (char *)malloc(strlen(filename) + 1);
    memcpy(new->filename, filename, strlen(filename) + 1);
    new->st = (struct stat *)malloc(sizeof(struct stat));
    memcpy(new->st, st, sizeof(struct stat));*/
    
    int i = find_blank_block();
    if(i == -1){
    	printf("there is no blank block");
    	return -ENOSPC;
	}
    if(init_block(i) == -1){
    	return -ENOMEM;
	}
    filenode *newnode = (filenode*)mymalloc(i ,sizeof(filenode));
    newnode->filename = (char*)mymalloc(i, strlen(filename) + 1);
    memcpy(newnode->filename, filename, strlen(filename) + 1);
    newnode->st = (struct stat *)mymalloc(i, sizeof(struct stat));
    memcpy(newnode->st, st, sizeof(struct stat));
    
    newnode->head_blocknum = i;
    newnode->content_start_at = *((int*)mem[i] + 1);
    // include 2 * sizeof(int) and fn used size
    
    newnode->next = root;
    root = newnode;
    return 1;
}

static void *oshfs_init(struct fuse_conn_info *conn)
{ 
	init_block(0);
	//temp->next is root
	filenode *temp = (filenode*)mymalloc(0, sizeof(filenode));
	temp->next = NULL;
	
	find_start_at = 0;
	
    return NULL;
}

static int oshfs_getattr(const char *path, struct stat *stbuf)
{
    int ret = 0;
    filenode *node = get_filenode(path);
    if(strcmp(path, "/") == 0) {
        memset(stbuf, 0, sizeof(struct stat));
        stbuf->st_mode = S_IFDIR | 0755;
    } else if(node) {
        memcpy(stbuf, node->st, sizeof(struct stat));
    } else {
        ret = -ENOENT;
    }
    return ret;
}

static int oshfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    filenode *node = root;
    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    while(node) {
        filler(buf, node->filename, node->st, 0);
        node = node->next;
    }
    return 0;
}

static int oshfs_mknod(const char *path, mode_t mode, dev_t dev)
{
    struct stat st;
    st.st_mode = S_IFREG | 0644;
    st.st_uid = fuse_get_context()->uid;
    st.st_gid = fuse_get_context()->gid;
    st.st_nlink = 1;
    st.st_size = 0;
    create_filenode(path + 1, &st);
    return 0;
}

static int oshfs_open(const char *path, struct fuse_file_info *fi)
{
    return 0;
}

static int oshfs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    /*filenode *node = get_filenode(path);
    node->st->st_size = offset + size;
    node->content = realloc(node->content, offset + size);
    memcpy(node->content + offset, buf, size);*/
    
    /*mywrite*/
    printf("use write\n");
    filenode *node = get_filenode(path);
    if(offset + size > node->st->st_size){
    	node->st->st_size = offset + size;
	}
    int i; //just temp
    int cur_block = node->head_blocknum;
    int offset_blocknr = 1;
    //cur_block is where offset is
	//offset_blocknr is number of passed blocks (offset pass how many blocks)
    int now_space = 0;
    now_space += blocksize - node->content_start_at;
    while(now_space < offset){
    	if(next_block(cur_block) == -1){
    		i = find_blank_block();
    		if(i == -1){
                printf("there is no blank block");
            	return -ENOSPC;
        	}
    		//init_block(i);
    		if(init_block(i) == -1){
            	return -ENOMEM;
	        }
    		*(int*)mem[cur_block] = i;
    		*(int*)mem[i] = -1;
		}
		now_space += blocksize - 2 * sizeof(int);
		cur_block = next_block(cur_block);
		offset_blocknr++;
	}
	//offset_in_block is offset in last block 
	int offset_in_block = offset;
	if(offset_blocknr == 1){
		offset_in_block = offset;
	}
	else{
		offset_in_block -= (blocksize - node->content_start_at);
		for(i=1; i<offset_blocknr-1; i++)
			offset_in_block -= (blocksize - 2 * sizeof(int));
	}
	
	//write begin
	int tempsize = size;
	int complete_size = 0;
	// first block need offset
	int writesize;
	if(offset_blocknr == 1){
		writesize = min(blocksize - node->content_start_at - offset_in_block, tempsize);
		memcpy(mem[cur_block] + node->content_start_at + offset_in_block, buf, writesize);
		*((int*)mem[cur_block]+1) = max(*((int*)mem[cur_block]+1), node->content_start_at + offset_in_block + writesize);
		tempsize -= writesize;
		complete_size += writesize;
	}
	else{
		writesize = min(blocksize - 2*sizeof(int) - offset_in_block, tempsize);
		memcpy(mem[cur_block] + 2*sizeof(int) + offset_in_block, buf, writesize);
		*((int*)mem[cur_block]+1) = max(*((int*)mem[cur_block]+1), 2*sizeof(int) + offset_in_block + writesize);
		tempsize -= writesize;
		complete_size += writesize;
	}
	// following blocks
	while(tempsize > 0){
		if(next_block(cur_block) == -1){
			i = find_blank_block();
			if(i == -1){
    	        printf("there is no blank block");
            	return -ENOSPC;
	        }
			//init_block(i);
			if(init_block(i) == -1){
            	return -ENOMEM;
        	}
			*(int*)mem[cur_block] = i;
    		*(int*)mem[i] = -1;
		}
		*((int*)mem[cur_block]+1) = blocksize;
		cur_block = next_block(cur_block);
		writesize = min(blocksize - 2*sizeof(int), tempsize);
		memcpy(mem[cur_block] + 2*sizeof(int), buf + complete_size, writesize);
		*((int*)mem[cur_block]+1) = max(*((int*)mem[cur_block]+1), 2*sizeof(int) + writesize);
		tempsize -= writesize;
		complete_size += writesize;
	}	
    return size;
}

static int oshfs_truncate(const char *path, off_t size)
{
    /*filenode *node = get_filenode(path);
    node->st->st_size = size;
    node->content = realloc(node->content, size);
    return 0;*/
    
    printf("use truncate\n");
    
    filenode *node = get_filenode(path);
    node->st->st_size = size;
    int i;
    int now_space = blocksize - node->content_start_at;
    int cur_block = node->head_blocknum;
    while(now_space < size){
    	if(next_block(cur_block) == -1){
    		printf("truncate error: last block\n");
    		/*??? truncate want to have larger space ???*/
    		i = find_blank_block();
    		if(i == -1){
            	printf("there is no blank block");
    	        return -ENOSPC;
	        }
			//init_block(i);
			if(init_block(i) == -1){
            	return -ENOMEM;
	        }
			*(int*)mem[cur_block] = i;
    		*(int*)mem[i] = -1;
		}
    	cur_block = next_block(cur_block);
    	now_space += blocksize - 2*sizeof(int);
	}
	
	// now_space - size = size after size in a block
    // blocksize - (now_space - size) = node->content_start_at + offset_in_block
	*((int*)mem[cur_block]+1) = blocksize-(now_space-size);
	
	//free memory
	printf("truncate free noused block\n");
	i = next_block(cur_block);
	int next;
	while(i != -1){
		next = next_block(i);
		munmap(mem[i], blocksize);
		mem[i] = NULL;
		i = next;
	}
	
	//last pointer
	*(int*)mem[cur_block] = -1;
    
    return 0;
}

static int oshfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    /*filenode *node = get_filenode(path);
    int ret = size;
    if(offset + size > node->st->st_size)
        ret = node->st->st_size - offset;
    memcpy(buf, node->content + offset, ret);
    return ret;*/
    
    printf("use read\n");
    filenode *node = get_filenode(path);
    int ret = size;
    if(offset + size > node->st->st_size)
        ret = node->st->st_size - offset;
        
    int now_space = blocksize - node->content_start_at;
    int cur_block = node->head_blocknum;
    while (now_space < offset){
        cur_block = next_block(cur_block);
        now_space += blocksize - 2*sizeof(int);
    }
    //cur_block is where offset is
    
    //find an easy way to code suddenly 
    // it is hard to compute by brain but it is easy to code
    // now_space - offset = size after offset in a block
    // blocksize - (now_space - offset) = node->content_start_at + offset_in_block
    int readsize = min(ret, now_space-offset);
    memcpy(buf, mem[cur_block] + blocksize - (now_space-offset), readsize);
    int complete_size = readsize;
    while (complete_size < ret && next_block(cur_block) != -1){
        cur_block = next_block(cur_block);
        readsize = min(ret-complete_size, blocksize-2*sizeof(int));
        memcpy(buf+complete_size, mem[cur_block] + 2*sizeof(int), readsize);
        complete_size += readsize;
    }
    return ret;
}

static int oshfs_unlink(const char *path)
{
	printf("use unlink\n");
    filenode *node = get_filenode(path);
    int cur_block = node->head_blocknum;
    filenode *temp = root;
    if(root == node){
    	root = root->next;
    	printf("deleted link list head node!\n");
	}
	else{
		while(temp->next != node)
    		temp = temp->next;
		temp->next = node->next;
	}
    printf("deleted link list node!\n");
    
    int next;
    while(cur_block != -1){
        next = next_block(cur_block);
        munmap(mem[cur_block],blocksize);
        mem[cur_block] = NULL;
        cur_block = next;
    }
    printf("free memory!\n");
    return 0;
}

static const struct fuse_operations op = {
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
    return fuse_main(argc, argv, &op, NULL);
}
