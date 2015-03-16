# 这是一个半同步/半异步的Tcp Server. #

## 一.目前是版本0.1.1，支持以下特性： ##

  * 1.使用了libevent库，支持大并发网络请求;
  * 2.网络操作与数据处理分离;
  * 3.使用线程池进行数据处理;
  * 4.目前支持tcp数据流的解包操作：
    * 4.1支持用户自定义的请求格式;
    * 4.2支持以\r\n结尾的行请求；
  * 5.使用time wheel实现了超时机制；
  * 6.添加对log的控制：是否打开log；是否输出到屏幕;
  * 7.支持设置允许的最大连接数;
  * 8.添加mac系统支持;
## 二.编译： ##

> ### 1. 依赖的库： ###
    * 1.1 boost 1.51.0 及其以上;
    * 1.2 ibevent-2.0.20;
    * 1.3 log4cplus;
    * 1.4 google tcmalloc;

> ### 2. 构建： ###
    * 2.1 使用cmake系统;
    * 2.2 目前编译了Debug版本和Release版本：
      * 2.2.1 在源代码目录下新建 build 目录;
      * 2.2.2 Debug编译：进入 build 目录，执行 cmake -DCMAKE\_BUILD\_TYPE=Debug ../；
      * 2.2.3 Release编译：进入 build 目录，执行 cmake -DCMAKE\_BUILD\_TYPE=Release ../；
      * 2.2.4 执行 make 命令, 生成可执行的测试文件;
    * 2.3 测试客户端为python文件;
      * 2.3.1 使用python 2.7版本；
      * 2.3.2 测试程序 test/testClient.py

## 三.历史版本： ##

> ### 1. 版本0.0.5： ###
    * 1.1 整理LightningServer类，隐藏实现细节，使头文件更简洁;
    * 1.2 完善CMakeList.txt, 增回release编译;
    * 1.3 添加include目录，用于放置用户使用的头文件。

> ### 2. 版本0.1.0： ###
    * 2.1 增加setSessionMaxCount（默认为-1,表示不作限制）接口到LightningServer类，允许用户设置当前最大连接数;
    * 2.2 添加了对accept返回EMFILE或ENFILE的处理。

> ### 3. 版本0.1.1： ###
    * 3.1 支持mac系统编译.