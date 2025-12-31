#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE 4096

static void write_msg(int fd, const char *msg) {

    while (*msg) {
        if (write(fd, msg, 1) != 1) {
            break;
        }
        msg++;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        write_msg(STDERR_FILENO,
                  "Usage: my_copy <src> <dest>\n");
        return 1;
    }

    const char *src  = argv[1];
    const char *dest = argv[2];

    
    int overwrite = 0;
    if (access(dest, F_OK) == 0) {
        char answer;
        write_msg(STDOUT_FILENO,
                  "Destination exists. Overwrite? (y/n): ");
        if (read(STDIN_FILENO, &answer, 1) != 1) {
            write_msg(STDERR_FILENO,
                      "Failed to read answer\n");
            return 1;
        }
        if (answer == 'n' || answer == 'N') {
            write_msg(STDOUT_FILENO,
                      "Copy canceled by user.\n");
            return 0;
        }
        overwrite = 1;
    }

  
    int src_fd = open(src, O_RDONLY);
    if (src_fd < 0) {
        perror("open src");
        return 1;
    }


    int flags = O_WRONLY | O_CREAT;
    if (overwrite)
        flags |= O_TRUNC;

    int dest_fd = open(dest, flags, 0644);
    if (dest_fd < 0) {
        perror("open dest");
        close(src_fd);
        return 1;
    }

  
    char buf[BUF_SIZE];
    ssize_t nread;

    while ((nread = read(src_fd, buf, BUF_SIZE)) > 0) {
        ssize_t written = 0;
        while (written < nread) {
            ssize_t nw = write(dest_fd,
                               buf + written,
                               nread - written);
            if (nw < 0) {
                perror("write");
                close(src_fd);
                close(dest_fd);
                return 1;
            }
            written += nw;
        }
    }

    if (nread < 0) {
        perror("read");
    }

    if (close(src_fd) < 0)
        perror("close src");
    if (close(dest_fd) < 0)
        perror("close dest");

    return (nread < 0) ? 1 : 0;
}
