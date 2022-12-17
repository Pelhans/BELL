#include "entity_link_server.h"
#include "seg_handler.h"

#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

void BELLServer::onConnection(const TcpConnectionPtr& conn) {
  LOG_TRACE << conn->peerAddress().toIpPort() << " -> "
            << conn->localAddress().toIpPort() << " is "
            << (conn->connected() ? "UP" : "DOWN");
}

void BELLServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time) {
  string msg(buf->retrieveAllAsString());
  WordSeg word_seg_handler;
  word_seg_handler.work(msg);
  LOG_TRACE << conn->name() << " recv " << msg.size() << " bytes at " << time.toString();
  conn->send(msg);
}