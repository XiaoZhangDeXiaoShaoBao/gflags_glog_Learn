#include <gflags/gflags.h>
#include <glog/logging.h>
#include <iostream>
using namespace std;
  
//DEFINE_xxxxx(参数名， 默认值， --help时的说明文字) 定义命令行参数，
//注意是全局。
 
DEFINE_int32(iterations, 50,  
    "The number of iterations to run."); 

int main(int argc,char* argv[])
{  
	cout<<"Hello main()"<<endl;
	//将所有日志输出到文件和stderr(终端)
	FLAGS_alsologtostderr = 1;
 
	//FLAGS_log_dir设置日志输出目录。/home/ww/GoogleLog/glogExample/log
	FLAGS_log_dir = "/home/ww/GoogleLog/glogExample/log"; 
 
	//gflags::ParseCommandLineFlags()解析命令行参数
    gflags::ParseCommandLineFlags(&argc, &argv, true);  
		
	//访问命令行参数，要在参数名称前面加前缀FLAGS_
	cout<<"iterations: "<<FLAGS_iterations<<endl;
      


	//输出日志文件到日志目录
    google::InitGoogleLogging(argv[0]);
 
	//对应级别的日志打印在对应级别的日志文件中
	//日志级别：INFO, WARNING, ERROR, FATAL、分别对应级别
	//0, 1, 2, 3

	LOG(INFO)<<"LOG level 0";
    LOG(WARNING)<<"LOG level 1";
	LOG(ERROR)<<"LOG level 2";
    LOG(FATAL)<<"LOG level 3";
      
	gflags::ShutDownCommandLineFlags();
    //google::ShutdownGoogleLogging();

	cout<<"Goodbye main()"<<endl;
        return 0;
 
}
