#ifndef LOGUTILLS_H_
#define LOGUTILLS_H_
#include<stdio.h>
#define VPLOGI(fmt,...) {\
	printf("\033[1;37m""%s:line %d[info]:\t""\033[0m",__FUNCTION__,__LINE__);\
	printf("\033[1;37m" fmt "\033[0m", __VA_ARGS__);\
	printf("\n");\
}
#define VPLOGW(fmt,...) {\
	printf("\033[1;33m""%s:line %d[waring]:\t""\033[0m",__FUNCTION__,__LINE__);\
	printf("\033[1;33m" fmt "\033[0m",__VA_ARGS__);\
	printf("\n");\
}
#define VPLOGE(fmt,...) {\
	printf("\033[1;31m""%s:line %d[Erro]:\t""\033[0m",__FUNCTION__,__LINE__);\
	printf("\033[1;31m" fmt "\033[0m", __VA_ARGS__);\
	printf("\n");\
}
#endif //LOGUTILLS_H_