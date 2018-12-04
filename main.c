#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/*
 * Function 'create_shared_mem' should allocate a segment of the shared memory 
 * with the size 'size' and name 'name' and return its address.
 * Segment should allow both reading and writing the data. 
 */


#if defined(POSIX) 
void* create_shared_mem(const char* name, off_t size) 
{
    fprintf(stdout, "POSIX: name(%s), size(%ld)\n",name,size);
    int mdesc, cr;
    mdesc = shm_open(name, O_CREAT | O_RDWR, 0644);

    if(mdesc == -1)
    {
        fprintf(stdout, "error shm_open\n");
        return NULL;
    }
    
    cr = ftruncate(mdesc,size);

    if(cr == -1)
    {
        fprintf(stdout, "error ftruncate\n");
        return NULL; 
    }

    void *ptr =  mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, mdesc, 0);

    fprintf(stdout, "POSIX: name(%s), size(%ld) sortie\n",name,size);
    return ptr;




}
#else

void* create_shared_mem(const char* name, off_t size) 
{
    fprintf(stdout, "name(%s), size(%ld)\n",name,size);
    key_t key = ftok(name, 'p');
    int id = shmget(key, size,IPC_CREAT | 0600);

    if(id == -1)
    {
        fprintf(stdout, "error shmget\n" );
        exit(-1);
    }
    fprintf(stdout, "name(%s), size(%ld) sortie\n",name,size);
    return shmat(id,NULL,0);


}
#endif

int main(void) {
    pid_t pid = fork();
    fprintf(stdout, "pid(%d)\n",pid);
    if (pid != 0) {
        fprintf(stdout, "appel create shrared mem: pid(%d)\n",pid);
        char* buf = create_shared_mem("/data", 100);
        if (buf == NULL)
        {
            fprintf(stdout, "sortie create_shared_mem error, (%d)\n",pid);
            exit(-1);
        }
        float* fbuf = (float*) buf;
        wait(NULL);
        printf("Values: %.3f %e %.3f\n", fbuf[0], *(float*)(buf + 37), fbuf[10]);
        munmap(buf, 100);
    } else {
        void* buf = (float*) create_shared_mem("/data", 40);
        if (buf == NULL)
        {
            fprintf(stdout, "sortie create_shared_mem error, (%d)\n",pid);
            exit(-1);
        }
        memset(buf, 0xBF, 40); 
        munmap(buf, 40);
    }
    shm_unlink("/data");
}
