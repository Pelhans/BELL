#include "entity_link_server.h"
#include "seg_handler.h"

#include <boost/serialization/singleton.hpp>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

void BELLServer::onConnection(const TcpConnectionPtr &conn) {
  LOG_TRACE << conn->peerAddress().toIpPort() << " -> "
            << conn->localAddress().toIpPort() << " is "
            << (conn->connected() ? "UP" : "DOWN");
}

void BELLServer::onMessage(const TcpConnectionPtr &conn, Buffer *buf,
                           Timestamp time) {
  string msg(buf->retrieveAllAsString());
  WordSeg word_seg_handler;
  word_seg_handler.work(msg);
  LOG_TRACE << conn->name() << " recv " << msg.size() << " bytes at "
            << time.toString();
  conn->send(msg);
}

void BELLServer::start(const string &conf_file) {
  auto config_manager =
      boost::serialization::singleton<ConfigManager>::get_const_instance();
  if (!config_manager.init(conf_file, use_single_config)) {
    LOG_ERROR << "fail to init config manager";
    return;
  }
  bell_config = config_manager.getDefaultConfig();
  ResourceInit::init(bell_config);
  LOG_INFO << "==== Bell server 0. initServece done";
  LOG_INFO << "==== Load model ...";
  boost::serialization::singleton<bell_model::ModelMgr>::get_const_instance()
      .load_all();
  boost::serialization::singleton<bell_model::ModelMgr>::get_const_instance()
      .done_reg();
  LOG_INFO << "==== Bell server start....";
  server_.start();
}