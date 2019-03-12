/************************发送源数据格式******************************/
/**********************末尾有换行符**********************************/
/************************列与列之间分隔符为\t(tab)*******************/
/**a	b	c	d	e	f	g h**/
/**...**/
/**************************************************************/


#ifndef _D_T_1_H
#define _D_T_1_H

#define _LINK_ID_ "***"
#define COLUMN_LEN 8
/************************************************************************/

#define MAX_LENTH 1024
#define DELIM "\t"
#define URL_LEN 8192
#define MAXBSIZE 512
#define LOG_FILE "./log/"
#define RES_PREFIX "./res_data/"
//url记录
#define RES_SUCCESS_PATH "./res_data/success/"
#define RES_ERROR_PATH "./res_data/error/"
//文件权限
#define FILE_MODE 0777

typedef struct{
	char a[URL_LEN];
	char b[MAXBSIZE];
	char c[MAXBSIZE];
	char d[MAXBSIZE];
	char e[MAXBSIZE];
	char f[MAXBSIZE];
	char g[MAXBSIZE];
	char h[MAXBSIZE];
	char i[MAXBSIZE];
}url_info;

/******************用户调用接口******************/
int func(const char *src_file);

/******************自定义返回值******************/
//成功
#define SUCCESS 0
//文件打开失败
#define FILE_OPEN_ERROR 1
//文件创建失败
#define FILE_CREATE_ERROR 2
//内存分配失败
#define MEM_MALLOC_ERROR 3
//源文件缺失
#define NO_SRC_FILE 4
//源文件为空
#define NO_SRC_FILE_COTENT 5

#endif //_D_T_1_H