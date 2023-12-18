#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/fs.h>

#include <stdio.h>

void set_immutable_flag(const char *f) {
    int flags;
    int fd = open(f, O_RDONLY);

    if (fd < 0) {
        perror(f);
    } else {
        if (ioctl(fd, FS_IOC_GETFLAGS, &flags) < 0) {
            perror(f);
        } else {
            flags |= FS_IMMUTABLE_FL;
            if (ioctl(fd, FS_IOC_SETFLAGS, &flags) < 0) {
                perror(f);
            } else {
                printf("%s: OK\n", f);
            }
        }
        close(fd);
    
    }
}

int main()
{
    // same result but without kernel module
    set_immutable_flag("local_file.txt");
    return 0;
}