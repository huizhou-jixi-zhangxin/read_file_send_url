 c langue 版：
    
    所有代码均在s_2_s.c这个文件内
    
    由于用到了libcurl,所以编译需要依赖curl库,gcc编译语句如下：
    
    编译可执行文件 a.out
        
        无符号表、不适用于调试
        
        gcc geb_url.c -L./curl/lib -lcurl
        
        有符号表、可以用于调试
        gcc -g -Og geb_url.c -L./curl/lib -lcurl
        
        
    编译动态库 s_2_s.so
        
        这是指明了依赖库的路径，在系统环境有差异的时候可以尝试
        
        该命令适用于调试完成后去除多余的符号表时使用
        
        gcc geb_url.c -fPIC -L./curl/lib -lcurl -Wl,-rpath ./curl/lib -Wl,-rpath-link ./curl/lib -shared -o s_2_s.so
        
        该命令用于需要符号表进行调试的环境使用，但是建议不要用so文件进行gdb调试、依旧会没有符号表、暂时还没搞定，建议用上面提到的带符号表的可执行文件来进行调试
        
        gcc -g -Og geb_url.c -fPIC -L./curl/lib -lcurl -Wl,-rpath ./curl/lib -Wl,-rpath-link ./curl/lib -shared -o FT_ES_Android_netease_sdk.so
        
        
    调试参考
    
        https://www.cnblogs.com/secondtonone1/p/5732938.html
    
    默认产生日志文件yyyy-mm--dd在./log下
    
    url发送结果文件yyyy-mm--dd在./res_data/success(error)下
    
    url发送结果打印在终端
    
    
编译curl源码：
    
    cd源码目录
    
    配置编译结果存放路径
    
    ./configure --prefix=/**
    
    make
    
    make install
    

已测试成功的环境：

    都不用自行下载libcurl源码进行编译，只要已经安装了curl命令即可
    
    编译so
    
    gcc geb_url.c -fPIC -lcurl -shared -o ***.so
    
    curl版本：
    
    curl 7.29.0 (x86_64-redhat-linux-gnu) libcurl/7.29.0 NSS/3.19.1 Basic ECC zlib/1.2.7 libidn/1.28 libssh2/1.4.3
    Protocols: dict file ftp ftps gopher http https imap imaps ldap ldaps pop3 pop3s rtsp scp sftp smtp smtps telnet tftp 
    Features: AsynchDNS GSS-Negotiate IDN IPv6 Largefile NTLM NTLM_WB SSL libz
    
    libcurl信息：
    
    libcurl/7.29.0 NSS/3.19.1 Basic ECC zlib/1.2.7 libidn/1.28 libssh2/1.4.3
    
    系统版本：
    
    CentOS Linux release 7.2.1511 (Core)
    
    Linux *** 3.10.0-327.el7.x86_64 #1 SMP Thu Nov 19 22:10:57 UTC 2015 x86_64 x86_64 x86_64 GNU/Linux
    
    
docker容器版本：

    目的是屏蔽掉底层linux和libcurl版本的差异性导致的一系列bug，统一采用测试通过版本
    
    docker版本：
    
    docker-18.03.0-ce.tgz
    
    镜像系统版本：
    
    centos:7.2.1511
    
    python版本：
    
    Python 2.7.5 (default, Nov 20 2015, 02:00:19) 
    
    [GCC 4.8.5 20150623 (Red Hat 4.8.5-4)] on linux2
    
    curl版本：
    
    curl 7.58.0 (x86_64-conda_cos6-linux-gnu) libcurl/7.58.0 OpenSSL/1.0.2n zlib/1.2.11
    Release-Date: 2018-01-24
    Protocols: dict file ftp ftps gopher http https imap imaps pop3 pop3s rtsp smb smbs smtp smtps telnet tftp 
    Features: AsynchDNS IPv6 Largefile NTLM NTLM_WB SSL libz TLS-SRP UnixSockets HTTPS-proxy
    
    