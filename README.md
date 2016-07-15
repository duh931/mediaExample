# mediaExample

work.c 是一个使用mediaStreamer2的例子，仍然存在未解决的error.


##关于如何使用mediastreamer来编译自己的程序##

在编译完mediastreamer2以后，可以在/lib/local/include 文件， 在/lib/local/lib中找到动态库 （.so)文件。
假设有一文件 test.c 要用到头文件 mediastreamer/mscommon.h。编译时就需要：
gcc test.c -lmediastreamer_base -o test
来链接动态库。
动态库的选择取决于需要编译程序使用的头文件。具体函数的位置参考mediastreamer2的doxygen文档。
http://www.linphone.org/docs/mediastreamer2/index.html

# linphone-compile
此文档记录mediastreamer在linux (ubuntu 14.04.4 LTS) 上的编译.

编译需要的dependencies: 
--mbedtls
--ortp git://git.linphone.org/ortp.git
--bctoolbox  git://git.linphone.org/bctoolbox.git
--bcunit  git://git.linphone.org/bcunit.git
上述库分别在github下载，并遵照 mbedtls-->bcunit-->bctoolbox-->ortp-->mediastreamer2 的顺序编译和安装
--libspeex and libspeexdsp 这两个通过apt-get安装
