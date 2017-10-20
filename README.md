# TinyWeb

```
 _____ _           __        __   _     
|_   _(_)_ __  _   \ \      / /__| |__  
  | | | | '_ \| | | \ \ /\ / / _ \ '_ \ 
  | | | | | | | |_| |\ V  V /  __/ |_) |
  |_| |_|_| |_|\__, | \_/\_/ \___|_.__/ 
               |___/                    
```



```
                           _                 
 _ __ ___     __ _   ___  | |_    ___   _ __ 
| '_ ` _ \   / _` | / __| | __|  / _ \ | '__|
| | | | | | | (_| | \__ \ | |_  |  __/ | |   
|_| |_| |_|  \__,_| |___/  \__|  \___| |_|   

```
## diamond TinyWeb
                                
| version | type |
|--------|-------|



> # 一、TinyWeb配置方法
名称不区分大小写，数值区分大小写;#开头为注释行
|名称|含义|
|-|-|
|listen|监听端口|
|processpoll|进程池数量|
|Docs|html路径|
|HostName|网址|
|***日志配置***|
|LogLevel|日志等级|
|LogPath|日志路径|
|DebugFile||
|InfoFile||
|WarnFile||
|ErrorFile||
|FatalFile||






升级计划
- 提升程序为demon进程（demon程序标准输入输出如何处理?）
- 优化loggger，解决logger生命周期过短的问题
- client 如果没有足够的内存接受数据，connection会一直存在（不属于bug）
- Semphore
- SharedMemory
- parser 可以精确地识别出路径名，和url（需要更加健壮）(应用正则表达式验证正确性)
- Connection::shutdownWrite应该也要销毁部分资源(已修复)
- 一段时间内，多个http request可能使用一个TcpConnection
- 优化logger，处理没有打开文件的异常,存在严重bug，需要重新架构
bug
- server写完文件之后如果关闭写端口则不能监听connectionlost 事件
- web浏览器在server还没有shutdownWrite的时候不能呈现html页面
- 其实shutdown也属于关闭连接
- distingush Protocol & Connection
    - Protocol 用来处理用户逻辑，简单的维护用户上下文，不负责数据传输，
        通过简单的重载basic Protocol的共有函数来实现处理Connection读到
        Buffer中的数据。
    - Connection 用来与底层的IO-Multiplexing 进行交互，负责数据的传输，
        维护者数据缓冲，监听着各种连接事件
    - 简单来说：Connection负责数据传输，不处理用户逻辑；Protocol不处理数据传输，
     负责处理逻辑

- 优化Connection::send () 如果缓冲区没有数据直接写入socket

国庆学习计划
- 入门negix
- 完成muduo的相关功能
- 实现进程池
- 实现红黑树，set集合



> # 1.如何进行测试

server_test 监听80端口，600s后终止程序
```
make lib
make server_test
sudo ./server_test
```

client.py 作为客户端连接
```
python client.py 9090 139.199.13.50 80
```

> # 2.如何启动

- 直接启动，默认配置文件为```/TinyWeb.conf```，该文件不存在，或格式错误时均返回错误
```
sudo ./TinyWeb
```

- 启动时指定配置文件路径,该文件不存在，或格式错误时返回错误
```
sudo ./TinyWeb -c /home/li/TinyWeb.conf
```


------------------

> # 1.Configer的使用方法

- 获得Configer实例（配置项还未加载，需调用loadConfig()）
```
Configer &getConfigerInstance(const std::string file = "")
```

- 调整confige文件
```
setConfigerFile(const std::string &file)
```

- 装载配置文件（不需调用，setConfigerFile会装载配置文件）
```
loadConfig()
```

- 获取配置项数值
```
std::string getConfigValue(const std::string &);
```

- 注意

这样会出错：
```
Configer configer = Configer::getConfigerInstance();
```

必须用引用接受：
```
Configer &configer = Configer::getConfigerInstance();
```

推荐用法 1：
```
std::string a = "../TinyWeb.conf";
setConfigerFile(a);
if (loadConfig())
    std::cout << "load config successfull\n";
else
    std::cout << "load config failed\n";
Configer::getConfigerInstance().test();
cout << getConfigValue("loglevel");
```

使用方法 2：
```
Configer &configer = Configer::getConfigerInstance();
configer.setConfigerFile("../TinyWeb.conf");
if (configer.loadConfig())
    std::cout << "load config successfull\n";
else
    std::cout << "load config failed\n";
configer.test();
cout << configer.getConfigValue("loglevel");
```

```

```


> # 2. Protocol 使用方法

- 如果想要自定义协议类，只需要继承Protocol,并重载相关函数（见WebProtocol定义）

