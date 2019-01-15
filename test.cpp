#include <glog/logging.h>

int main(int argc,char* argv[])
{
    google::InitGoogleLogging(argv[0]); //初始化 glog
    LOG(INFO) << "Hello,GOOGLE!";
    LOG(INFO)   << "嘿嘿...";
    LOG(WARNING) << "呵呵...";
    LOG(INFO) << "Hello,GOOGLE!";
    LOG(ERROR) << "嘻嘻...";
    LOG(FATAL) << "哈哈...";
    google::ShutdownGoogleLogging();
    return 0;
}
