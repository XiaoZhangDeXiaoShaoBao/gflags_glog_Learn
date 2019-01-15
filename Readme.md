# glog
glog 默认对log分为4级： INFO,  WARNING,  ERROR,  FATAL.  打印log语句类似于C++中的stream，实际上LOG(INFO) 宏返回的是一个继承自std::ostrstream类的对象。

* 文件名各字段对应含义为：

<program name>.<hostname>.<user name>.log.<severity level>.<date>.<time>.<pid>

Log file created at: 2019/01/15 19:56:46
Running on machine: ww-pc
Log line format: [IWEF]mmdd hh:mm:ss.uuuuuu threadid file:line] msg
I0115 19:56:46.652129  9099 main.cpp:36] LOG level 0
W0115 19:56:46.652541  9099 main.cpp:37] LOG level 1
E0115 19:56:46.652675  9099 main.cpp:38] LOG level 2
F0115 19:56:46.652817  9099 main.cpp:39] LOG level 3


其中：
```
1），<program name> 其实对应google::InitGoogleLogging(argv[0])；中的argv[0]，即通过改变google::InitGoogleLogging的参数可以修改日志文件的名称。

2），每个级别的日志会输出到不同的文件中。并且高级别日志文件会同样输入到低级别的日志文件中。 即：FATAL的信息会同时记录在INFO，WARNING，ERROR，FATAL日志文件中。默认情况下，glog还会将会将FATAL的日志发送到stderr中。
```

## glog 编译

`g++ b.cpp -lglog -lpthread -o a.out`


`g++ a.cpp -lglog -lpthread -lgflags -o a.out`


---

# gflags

google开源的gflags是一套命令行参数解析工具，比getopt功能更强大，使用起来更加方便，gflags还支持从环境变量、配置文件读取参数（可用gflags代替配置文件）

## 定义参数 

使用flags需要包含头文件 
 `#include <glog/logging.h>` 
 主要支持的参数类型包括bool，int32, int64, uint64, double, string等，定义参数通过DEFINE_type宏实现。
 如下所示，分别定义了一个bool和一个string类型的参数，该宏的三个参数含义分别为 **_命令行参数名，参数默认值，以及参数的帮助信息 _**。
```
DEFINE_bool(big_menu, true, "Include 'advanced' options in the menu listing"); 
DEFINE_string(languages, "english,french,german", 
                 "comma-separated list of languages to offer in the 'lang' menu"); 
```
gflag不支持列表，用户通过灵活借助string参数实现，比如上述的languages参数，可以类型为string，但可看作是以逗号分割的参数列表。

## 访问参数 

当参数被定义后，通过FLAGS_name就可访问到对应的参数，比如上述定义的big_menu、languages两个参数就可通过FLAGS_big_menu、FLAGS_languages访问。
```
if (FLAGS_languages.find("english") != string::npos) 
     HandleEnglish(); 
```
以上的访问方式，仅在参数定义和访问在同一个文件（或是通过头文件包含）时，FLAGS_name才能访问到参数，**_如果要访问其他文件里定义的参数，则需要使用DECLARE_type_**。
比如在foo.cc中

`DEFINE_string(color, "red", "the color you want to use"); `

这是如果你需要在foo_test.cc中使用color这个参数，你需要加入DECLARE_string(color, "red", "the color you want to use");

 

## 参数检查 

定义参数后，可以给参数注册一个检查函数（validator），当从命令行指定参数或通过SetCommandLineOption()指定参数时，检查函数就会被调用，两个参数分别为命令行参数名，以及设置的参数值。
``` 
static bool ValidatePort(const char* flagname, int32 value) { 
   if (value > 0 && value < 32768)   // value is ok 
     return true; 
   printf("Invalid value for --%s: %d\n", flagname, (int)value); 
   return false; 
} 

DEFINE_int32(port, 0, "What port to listen on"); 
static const bool port_dummy = RegisterFlagValidator(&FLAGS_port, &ValidatePort); 
```
**_建议在定义参数后，立即注册检查函数_**。
RegisterFlagValidator()在检查函数注册成功时返回true；如果参数已经注册了检查函数，或者检查函数类型不匹配，返回false。

## 初始化参数

在引用程序的main()里通过 
`google::ParseCommandLineFlags(&argc, &argv, true); `
即完成对gflags参数的初始，其中第三个参数为remove_flag，如果为true，gflags会移除parse过的参数，否则gflags就会保留这些参数，但可能会对参数顺序进行调整。 
比如 "/bin/foo" "arg1" "-q" "arg2"  会被调整为 "/bin/foo", "-q", "arg1", "arg2"，这样更好理解。