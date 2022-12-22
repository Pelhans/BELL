#include "entity_link_server.h"

#include <functional>
#include <utility>

#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

int kRollSize = 500*1000*1000;

std::unique_ptr<muduo::AsyncLogging> g_asyncLog;

void asyncOutput(const char* msg, int len) {
  g_asyncLog->append(msg, len);
}

void setLogging(const char* argv0) {
  muduo::Logger::setOutput(asyncOutput);
  char name[256];
  strncpy(name, argv0, 256);
  g_asyncLog.reset(new muduo::AsyncLogging(::basename(name), kRollSize));
  g_asyncLog->start();
}

int main(int argc, char* argv[]) {
  setLogging(argv[0]);
  string conf_file = "/home/pelhans/project/BELL/deploy/conf/total_conf_base.xml";

  LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
  EventLoop loop;
  InetAddress listenAddr(2007);
  BELLServer server(&loop, listenAddr);

  server.start(conf_file);

  loop.loop();
}