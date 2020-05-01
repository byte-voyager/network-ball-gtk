
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "utils.h"


int get_mem_percentage()
{

    // read memory information from /proc/meminfo
    FILE *fd_men = fopen("/proc/meminfo", "r");
    if (fd_men == NULL)
    {
        perror("open file /proc/meminfo error");
        exit(-1);
    }

    char tmp[255];
    fgets(tmp, 255, fd_men);
    unsigned long men = 0;
    unsigned long free = 0;
    sscanf(tmp, "MemTotal: %ld KB", &men);
    fgets(tmp, 255, fd_men);
    fgets(tmp, 255, fd_men);
    sscanf(tmp, "MemAvailable: %ld KB", &free);

    unsigned long use = men - free;
    int percent = ((float)use / (float)men) * 100;

    fclose(fd_men);
    return percent;
}


void get_bandwidth(unsigned long long int *receiveBytes, unsigned long long int *sendBytes)
{
    char *buf;
    const int bufSize = 255;
    FILE *devfd;

    buf = (char *)calloc(bufSize, 1);

    devfd = fopen("/proc/net/dev", "r");
    if (devfd == NULL)
    {
        perror("open file /proc/net/dev failure.");
        exit(-1);
    }

    // Ignore the first and second lines of the file.
    fgets(buf, bufSize, devfd); // fgets will return if reading a newline.
    fgets(buf, bufSize, devfd);
    *receiveBytes = 0;
    *sendBytes = 0;

    while (fgets(buf, bufSize, devfd))
    {
        unsigned long long int rBytes, sBytes;
        char *line = strdup(buf);

        char *dev;
        dev = strtok(line, ":");
        gchar *is_lo = g_strrstr(dev, "lo");
        if (is_lo != NULL)
        { // if end with lo
            continue;
        }
        sscanf(buf + strlen(dev) + 2, "%llu %*d %*d %*d %*d %*d %*d %*d %llu", &rBytes, &sBytes);
        *receiveBytes += rBytes;
        *sendBytes += sBytes;
        free(line);
    }

    fclose(devfd);
    free(buf);
}


float kb2m(unsigned long int kb)
{
    return (float)kb / 1024;
}