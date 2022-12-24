#include "entity_link_server.h"

#include <functional>
#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <utility>

using namespace std;
using namespace muduo;
using namespace muduo::net;

int kRollSize = 500 * 1000 * 1000;

std::unique_ptr<muduo::AsyncLogging> g_asyncLog;

void asyncOutput(const char *msg, int len) { g_asyncLog->append(msg, len); }

void setLogging(const char *argv0) {
    muduo::Logger::setOutput(asyncOutput);
    char name[256];
    strncpy(name, argv0, 256);
    g_asyncLog.reset(new muduo::AsyncLogging(::basename(name), kRollSize));
    g_asyncLog->start();
}

int main(int argc, char *argv[]) {
    //  setLogging(argv[0]);

    struct option long_options[] = {
        {"conf", required_argument, 0, 'c'},
        {"help", no_argument, 0, 'h'},
    };
    int opt;

    string conf_file;
    while ((opt = getopt_long(argc, argv, "c:h", long_options, 0)) != -1) {
        switch (opt) {
            case 'c': conf_file = optarg; break;
            case 'h':
                cerr << "usage: bin --conf conf_file" << endl;
                exit(1);
                break;
            default:
                cerr << "unkown cmd option " << opt << endl;
                cout << "usage: bin --conf conf_file" << endl;
                exit(1);
                break;
        }
    }
    //  string conf_file =
    //  "/home/pelhans/project/BELL/deploy/conf/total_conf_base.xml";

    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
    LOG_INFO << "conf_file path: " << conf_file.c_str();
    EventLoop loop;
    InetAddress listenAddr(2023);
    BELLServer server(&loop, listenAddr);

    server.start(conf_file);

    loop.loop();
}
