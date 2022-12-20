#include "muduo/net/TcpServer.h"
#include "muduo/base/AsyncLogging.h"
#include "muduo/base/Logging.h"
#include "muduo/base/Thread.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"

#include "config_manager.h"
#include "resource_init.h"

#include <iostream>
#include <functional>
#include <utility>

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

using namespace bell;
using namespace muduo;
using namespace muduo::net;

class BELLServer {
public:
  BELLServer(EventLoop* loop, const InetAddress& listenAddr)
    : loop_(loop),
      server_(loop, listenAddr, "BELLServer") {
    server_.setConnectionCallback(
        std::bind(&BELLServer::onConnection, this, _1));
    server_.setMessageCallback(
        std::bind(&BELLServer::onMessage, this, _1, _2, _3));
  }

  void start(const string& conf_file) {
    auto config_manager = singleton<ConfigManager>::getInstance();
    if (!config_manager -> init(conf_file, use_single_config)) {
      LOG_ERROR << "fail to init config manager";
      return false;
    }
    bell_config = config_manager->getDedaultConfig();
    ResourceInit::async_init(bell_config);
    LOG_INFO << "==== Bell server 0. initServece done";
    ResourceInit::async_init_wait();
    LOG_INFO << "==== Bell server start....";
    server_.start();
  }

private:
   void onConnection(const TcpConnectionPtr& conn);
  void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time);

  EventLoop* loop_;
  TcpServer server_;

private:
  bell::BellConfig bell_config;
  bool use_single_config;
};