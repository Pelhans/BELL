#include "muduo/base/AsyncLogging.h"
#include "muduo/base/Logging.h"
#include "muduo/base/Thread.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TcpServer.h"

#include "config_manager.h"
#include "model_manager.h"
#include "resource_init.h"
#include <boost/serialization/singleton.hpp>

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

  void start(const string &conf_file) {
    auto config_manager =
        boost::serialization::singleton<ConfigManager>::get_const_instance();
    if (!config_manager.init(conf_file, use_single_config)) {
      LOG_ERROR << "fail to init config manager";
      return;
    }
    bell_config = config_manager.getDefaultConfig();
    ResourceInit::async_init(bell_config);
    LOG_INFO << "==== Bell server 0. initServece done";
    LOG_INFO << "==== Load model ...";
    ResourceInit::async_init_wait();
    boost::serialization::singleton<bell_model::ModelMgr>::get_const_instance()
        .load_all();
    boost::serialization::singleton<bell_model::ModelMgr>::get_const_instance()
        .done_reg();
    LOG_INFO << "==== Bell server start....";
    server_.start();
  }

private:
  void onConnection(const TcpConnectionPtr &conn);
  void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time);

  EventLoop *loop_;
  TcpServer server_;

private:
  bell::BellConfig bell_config;
  bool use_single_config;
};
