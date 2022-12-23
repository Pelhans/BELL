#include "muduo/base/AsyncLogging.h"
#include "muduo/base/Logging.h"
#include "muduo/base/Thread.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TcpServer.h"

#include "config_manager.h"
#include "model_manager.h"
#include "resource_init.h"

#include <functional>
#include <iostream>
#include <utility>

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

using namespace bell;
using namespace muduo;
using namespace muduo::net;

class BELLServer {
public:
  BELLServer(EventLoop *loop, const InetAddress &listenAddr)
      : loop_(loop), server_(loop, listenAddr, "BELLServer") {
    server_.setConnectionCallback(
        std::bind(&BELLServer::onConnection, this, _1));
    server_.setMessageCallback(
        std::bind(&BELLServer::onMessage, this, _1, _2, _3));
  }

  void start(const string &conf_file);

private:
  void onConnection(const TcpConnectionPtr &conn);
  void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time);

  EventLoop *loop_;
  TcpServer server_;

private:
  bell::BellConfig bell_config;
  bool use_single_config;
};
