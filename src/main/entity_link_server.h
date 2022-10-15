#include "muduo/net/TcpServer.h"
#include "muduo/base/AsyncLogging.h"
#include "muduo/base/Logging.h"
#include "muduo/base/Thread.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"

#include <iostream>
#include <functional>
#include <utility>

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include "seg_handler.h"

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

  void start() {server_.start();}

private:
   void onConnection(const TcpConnectionPtr& conn);
  void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time);

  EventLoop* loop_;
  TcpServer server_;
};