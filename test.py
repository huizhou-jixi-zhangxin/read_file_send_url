from ctypes import *


def main():
    
    result = cdll.LoadLibrary("./***.so")
    ret = result.s_2_s_func('./data')
    print ret

    if(ret != 0):
        print "function failed!!!"
    

if __name__ == "__main__":
    main()
