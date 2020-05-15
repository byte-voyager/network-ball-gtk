#ifndef UTILS_H
#define UTILS_H

int get_mem_percentage(void);

void get_bandwidth(unsigned long long int *receiveBytes, unsigned long long int *sendBytes);


float kb2m(unsigned long int kb);

float rand_color();

float get_color();
#endif /*UTILS_H*/
