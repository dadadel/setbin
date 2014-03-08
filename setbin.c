#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COUPLES_OFFSET_VALUE 200

unsigned long offsets[MAX_COUPLES_OFFSET_VALUE];
char values[MAX_COUPLES_OFFSET_VALUE];
int nb;

char change(offset, value)
{
    int i;
    char c = value;

    for (i = 0; i < nb; i++)
    {
        if (offset == offsets[i])
        {
            c = values[i];
            break;
        }
    }
    return c;
}

int main(int argc, char **argv)
{
    char *src, *dst;
    unsigned long i;
    char c;
    int fs, fd, ret;

    if (argc < 5)
    {
        printf("Bad number of arguments.\n");
        printf("USAGE: %s  source_file dest_file offset value [offset2 value2 [...]]\n", argv[0]);
        return 0;
    }
    src = argv[1];
    dst = argv[2];

    nb = (argc - 3) / 2;
    nb = nb < MAX_COUPLES_OFFSET_VALUE ? nb : MAX_COUPLES_OFFSET_VALUE;
    for (i = 0; i < nb; i++)
    {
        offsets[i] = atol(argv[3 + (i * 2)]);
        values[i] = atoi(argv[4 + (i * 2)]);
    }

    fs = open(src, O_RDONLY);
    if (fs < 0)
    {
        printf("Failed to open file %s\n", src);
        return 0;
    }

    fd = open(dst, O_CREAT | O_WRONLY);
    if (fd < 0)
    {
        close(fs);
        printf("Failed to create file %s\n", dst);
        return 0;
    }

    i = 0L;
    while(1)
    {
        ret = read(fs, &c, 1);
        if (ret < 1)
        {
            printf("nothing read (offset 0x%lX)\n", i);
            break;
        }
        c = change(i, c);
        ret = write(fd, &c, 1);
        if (ret < 1)
        {
            printf("Failed to write at offset 0x%lX\n", i);
            break;
        }
        i++;
    }

    close(fs);
    close(fd);
    return 0;
}
