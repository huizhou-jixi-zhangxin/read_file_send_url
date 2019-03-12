
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "include/curl/curl.h"
#include "include/d_t_1.h"

static char date_time[100];
static char suc_url[50];
static char err_url[50];
static char temp_err_url[50];
static char log_path[50];
static char day[20];

static int flag = 0;

/**
 * out
 */
int get(const int *pSrc, const int SrcNum, char *pRes)
{
		//对应一串数字、作为密钥
        const int k[] = {0};
        const int KNum = sizeof(k)/sizeof(k[0]);

        int i = 0;
        for (i; i < SrcNum; ++i)
        {
                int ki = i % KNum;
                pRes[i] = (char)(pSrc[i] - k[ki]);
        }
        return SrcNum;
}

/**
* 时间工具函数
*/
void get_time(){
	
	time_t t;
	time(&t);
	struct tm *tmp_time = localtime(&t);
	strftime(date_time, sizeof(date_time), "%04Y-%02m-%02d %H:%M:%S", tmp_time);
	strftime(day, sizeof(day), "%04Y-%02m-%02d", tmp_time);
}

/**
* 目录创建
*/
int gen_common_content(){
	
	memset(date_time, 0, 100);
	memset(suc_url, 0, 50);
	memset(err_url, 0, 50);
	memset(temp_err_url, 0, 50);
	memset(log_path, 0, 50);
	memset(day, 0, 20);

	int ret = SUCCESS;
	get_time();
	
	//文件路径不存在则创建
	if (access(RES_PREFIX, F_OK) == -1){
		//结果目录
		ret = mkdir(RES_PREFIX, FILE_MODE);
		if (SUCCESS != ret){
			printf("%s %s file create failed! \n", date_time, RES_PREFIX);
			return FILE_CREATE_ERROR;
		}
		
		ret = mkdir(RES_SUCCESS_PATH, FILE_MODE);
		if (SUCCESS != ret){
			printf("%s %s file create failed! \n", date_time, RES_SUCCESS_PATH);
			return FILE_CREATE_ERROR;
		}
		
		ret = mkdir(RES_ERROR_PATH, FILE_MODE);
		if (SUCCESS != ret){
			printf("%s %s file create failed! \n", date_time, RES_ERROR_PATH);
			return FILE_CREATE_ERROR;
		}
	}
	
	if (access(LOG_FILE, F_OK) == -1){
		//日志目录
		ret = mkdir(LOG_FILE, FILE_MODE);
		if (SUCCESS != ret){
			printf("%s %s file create failed! \n", date_time, LOG_FILE);
			return FILE_CREATE_ERROR;
		}
	}
	
	strcpy(suc_url, RES_SUCCESS_PATH);
	strcat(suc_url, day);
	
	strcpy(err_url, RES_ERROR_PATH);
	strcat(err_url, day);
	
	//第一次发送失败的url
	strcpy(temp_err_url, RES_ERROR_PATH);
	//使用默认隐藏的文件地址 避免使用结构体指针带来的一系列问题 中间结果还是用临时文件来保存
	strcat(temp_err_url, ".temp");
	
	strcpy(log_path, LOG_FILE);
	strcat(log_path, day);

	return SUCCESS;
}

/**
* 统计源文件的行数
* @src_file : 文件路径
* @ n ：文件行数
*/
int file_wc(const char *src_file)
{
    FILE *fp;
    int n = 0;
    int ch;
	FILE *lf = fopen(log_path, "a+");
	
	if (NULL == lf){
		printf("%s file open failed! \n", date_time);

		return FILE_OPEN_ERROR;
	}

	if((fp = fopen(src_file,"r+")) == NULL)
	{
		fprintf(lf, "%s open file %s error! %s\n", date_time, src_file, strerror(errno));
		fclose(lf);
		
		return FILE_OPEN_ERROR;
	}

	while((ch = fgetc(fp)) != EOF) 
	{
	    if(ch == '\n')
	    {
	        n++;
	    }
	}

	fclose(fp);
	fclose(lf);
	
	return n;
}

/**
* 组装url
*/
void make_url(char *column[], url_info* ui){
	
    char url_prefix[URL_LEN];
    //字符串清零,避免多次调用时由于字符串越界的内存奔溃
    memset(url_prefix, 0, URL_LEN);
    //通过set.c加密过后的一串数字
    int in[] = {0};
    int len = sizeof(in) / sizeof(int);
    get(in, len, url_prefix);
    strcat(url_prefix, "&a=");
    strcat(url_prefix, _LINK_ID_);
    strcat(url_prefix, "&b=***");
    strcat(url_prefix, "&c=");
    strcat(url_prefix, column[0]);
    strcat(url_prefix, "&d=");
    strcat(url_prefix, column[1]);
    strcat(url_prefix, "&e=");
    strcat(url_prefix, column[2]);
    strcat(url_prefix, "&f=");
    strcat(url_prefix, column[3]);
    strcat(url_prefix, "&g=0");
    strcat(url_prefix, "&h=");
    strcat(url_prefix, column[4]);
    strcat(url_prefix, "&i=");
    strcat(url_prefix, column[5]);
    strcat(url_prefix, "&j=");
    strcat(url_prefix, column[6]);
    strcat(url_prefix, "&k=");
    strcat(url_prefix, column[7]);
	
    strcpy(ui->a, url_prefix);
	
	strcpy(ui->b, column[0]);
	strcpy(ui->c, column[1]);
	strcpy(ui->d, column[2]);
	strcpy(ui->e, column[3]);
	strcpy(ui->f, column[4]);
	strcpy(ui->g, column[5]);
	strcpy(ui->h, column[6]);
	strcpy(ui->j, column[7]);
}

/**
* curl 访问url操作
* 目前使用的是libcurl的easy接口
* 返回最终发送失败的url条数
*/
int url_func(url_info* uis[], int len){

    int ret;
    int n = 0;
	//error count
	int error = 0;
	FILE *ef;
	FILE *sf = fopen(suc_url,"a+");
	FILE *lf = fopen(log_path, "a+");
	
	if(sf == NULL || lf == NULL)
    {
		printf("%s file open failed! \n", date_time);
	
		return FILE_OPEN_ERROR;
    }
	
	//第一次发送 错误结果保存在临时文件中
	if (0 == flag){
		ef = fopen(temp_err_url, "w+");
		if(ef == NULL)
		{
			printf("%s file open failed! \n", date_time);
	
			return FILE_OPEN_ERROR;
		}
	}else{
	//第二次发送 错误结果保存在最终错误文件中
		ef = fopen(err_url, "w+");
		if(ef == NULL)
		{
			printf("%s file open failed! \n", date_time);
	
			return FILE_OPEN_ERROR;
		}
	}
	
    CURL *curl;
	CURLU *urlp;
	CURLUcode uc;

    while(n < len){
		curl_global_init(CURL_GLOBAL_ALL);
		
        curl = curl_easy_init();
        if (!curl) {
            continue;
        }
		
		curl_easy_setopt(curl, CURLOPT_URL, uis[n]->url);
		ret = curl_easy_perform(curl);
		//用于作为url终端打印的间隔
		printf("\n");
        
		if (ret == CURLE_OK){
			curl_easy_cleanup(curl);
			fprintf(sf, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", uis[n]->a, uis[n]->b, uis[n]->c, uis[n]->d, uis[n]->e, uis[n]->f, uis[n]->g, uis[n]->h);
			n++;
		}else
		{
			fprintf(lf, "%s flag:%d url access failed, error code : %d \n", date_time, flag, ret);
			fprintf(ef, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", uis[n]->a, uis[n]->b, uis[n]->c, uis[n]->d, uis[n]->e, uis[n]->f, uis[n]->g, uis[n]->h);
			n++;
			error++;
			curl_easy_cleanup(curl);
            continue;
		}
    }
	
    curl_global_cleanup();
	
	fclose(sf);
	fclose(lf);
	fclose(ef);
	
    return error;
}

/*
 * path: file path
 *
 * */
int file_func(const char *src_file)
{
    char buffer[MAX_LENTH];//以一个字符数组为缓存,按行读取内容,每次按一个字符串来处理
	memset(buffer, 0, MAX_LENTH);
	int num = 1;
    FILE *pf = fopen(src_file, "r");
	FILE *lf = fopen(log_path, "a+");
	char *column;
	int ret = SUCCESS;
    int i = 0;

    if(pf == NULL || lf == NULL)
    {
		printf("%s file open failed! \n", date_time);

		return FILE_OPEN_ERROR;
    }

    int wc = file_wc(src_file);
	url_info *uis[wc];

	if (0 >= wc){
		fprintf(lf, "%s src file no content! \n", date_time);
		
		fclose(pf);
		fclose(lf);
		return NO_SRC_FILE_COTENT;
	}

    for(i; i < wc; i++)
    {
		uis[i] = (url_info*)malloc(sizeof(url_info));
		
		if (NULL == uis[i]){
			fprintf(lf, "%s malloc memory failed! \n", date_time);
			fclose(pf);
			fclose(lf);
			return MEM_MALLOC_ERROR;
		}
		memset(uis[i], 0, sizeof(url_info));
    }
	
	i = 0;
	char *column_buff[COLUMN_LEN];
    while(fgets(buffer, MAX_LENTH, pf) != NULL)
    {
		num = 1;
		column = strtok(buffer, DELIM);
        column_buff[0] = column;
        //字符串切割
		while((column = strtok(NULL, DELIM))){
			if (column[strlen(column) - 1] == '\n'){
				column[strlen(column) - 1] = '\0';
			}
			column_buff[num] = column;
			num++;
		}
		if (COLUMN_LEN != num){
			//源数据列数不对
			fprintf(lf, "%s line: %d, src data have error!\n", date_time, i);
			continue;
		}
		make_url(column_buff, uis[i]);
		++i;
    }

    ret = url_func(uis, i);
    if(ret != SUCCESS){
        fprintf(lf, "%s flag:%d there are some error url!\n", date_time, flag);
    }

    i = 0;
    for(i; i < wc; i++)
    {
		free(uis[i]);
    }
	
	fclose(pf);
	fclose(lf);
	
    return ret;
}

/**
** 对外开放接口,第一次发完url失败的会发第二次
*/
int func(const char *src_file){
	
	int ret = SUCCESS;
	
	if (NULL == src_file){
		printf("please input source file path!!!\n");
		return NO_SRC_FILE;
	}
	
	ret = gen_common_content();
	if (ret != SUCCESS){
		printf("%s common content create failed! \n", date_time);
		return ret;
	}
	
	ret = file_func(src_file);
	if (ret != SUCCESS){
		flag = 1;
		ret = file_func(temp_err_url);
	}
	
	return ret;
}