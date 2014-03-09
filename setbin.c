#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_COUPLES_OFFSET_VALUE 200

typedef struct _bin_couples_t
{
    int nb;
    unsigned long *offsets;
    char *values;
} bin_couples_t;

char change(unsigned long offset, char value, bin_couples_t *cpl)
{
    int i;
    char c = value;

    for (i = 0; i < cpl->nb; i++)
    {
        if (offset == cpl->offsets[i])
        {
            c = cpl->values[i];
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
    bin_couples_t cpl;

    if (argc < 5)
    {
        printf("Bad number of arguments.\n");
        printf("USAGE: %s  source_file dest_file offset value [offset2 value2 [...]]\n", argv[0]);
        return 0;
    }
    src = argv[1];
    dst = argv[2];

    cpl.nb = (argc - 3) / 2;
    cpl.offsets = (unsigned long *)malloc(sizeof(unsigned long) * cpl.nb);
    if (cpl.offsets == NULL)
    {
        printf("Memory allocation failed\n");
        return 0;
    }
    cpl.values = (char *)malloc(sizeof(char) * cpl.nb);
    if (cpl.values == NULL)
    {
        printf("Memory allocation failed\n");
        return 0;
    }
    for (i = 0; i < cpl.nb; i++)
    {
        cpl.offsets[i] = atol(argv[3 + (i * 2)]);
        cpl.values[i] = atoi(argv[4 + (i * 2)]);
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
        c = change(i, c, &cpl);
        ret = write(fd, &c, 1);
        if (ret < 1)
        {
            printf("Failed to write at offset 0x%lX\n", i);
            break;
        }
        i++;
    }
    free(cpl.offsets);
    free(cpl.values);
    close(fs);
    close(fd);
    return 0;
}
