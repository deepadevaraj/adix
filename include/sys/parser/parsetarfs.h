#ifndef _PARSETARFS_H
#define _PARSETARFS_H
/* parses tarfs and prints the names of files
* returns the number of files in tarfs
*/

#include <defs.h>
#include <sys/filesystems/file_structures.h>
int parsetar();

/* reads @numbytes from @offset of @filename into @buffer
* note: does not handle the case when size of @buffer is less than @numbytes
*/
int read_tarfs( char *filename, uint64_t offset, uint64_t numbytes, char * buffer);

int sys_open(const char* pathname);

int64_t sys_read(int fd, void *buf, uint64_t count);

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

/**
 * Seek to new position at an offset 'offset' relative to 'whence'
 * @param fd     file descriptor
 * @param offset offset in bytes
 * @param whence SEEK_SET for start of file, SEEK_CUR for current seek location, SEEK_END for end of file
 * @return       OK or ERROR
 */
int sys_lseek(int fd, off64_t offset, int whence);

int sys_close(int fd);

int sys_opendir(const char *pathname);

struct posix_header_ustar* sys_readdir(int fd, uint64_t ret);

int sys_closedir(int fd);

extern struct operation_pointers tarfs_ops;

struct operation_pointers* get_tarfs_ops();

#endif
