
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * in
 */
int set(const char *pSrc, int Res[], int len)
{
	printf("%s\n", pSrc);
    //一串数字、作为密钥
	const int k[] = {0};
	const int KNum = sizeof(k)/sizeof(k[0]);
        int i = 0;
	for (i; i < len; ++i)
	{
		int ki = i % KNum;
		Res[i] = (int)(pSrc[i]) + k[ki];
	}
	return i;

}

/**
 * out
 */
int get(const int *pSrc, const int SrcNum, char *pRes)
{
        //一串数字、作为密钥
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

int main(int argc, char *argv[])
{
    int ret = 0;
    char *url_prefix = "https://***";
    int len = 0;
    len = strlen(url_prefix);
    int *res;
    res = (int *)malloc(len * sizeof(int));
    ret = set(url_prefix, res, len);
    if (len != ret){
        printf("set error~\n");
        free(res);
	return 1;
    }
    
    int i = 0;
	//去掉最后一个逗号组成的数组作为明文输入
    for(i; i < ret; i++){
        printf("%d,", res[i]);
    }
    printf("\n");
    char Res[len];
    get(res, len, Res);
    printf("%s\n", Res);
    free(res);

    return 0;
}