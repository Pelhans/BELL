#include "entity_link_server.h"
#include "seg_handler.h"
#include "lucene_recall.h"

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
    auto bevent = std::make_shared<bell::Event>();
    bevent->req.origin_query = msg;
    // 功能逻辑在这写
    executer(bevent);
    LOG_TRACE << conn->name() << " recv " << msg.size() << " bytes at "
              << time.toString();
    conn->send(msg);
}

int BELLServer::executer(std::shared_ptr<bell::Event> bevent) {
    CHECK_NULL(bevent, BELL_FAIL);
    WordSeg word_seg_handler;
    word_seg_handler.work(bevent);

    bell::event::SegResOutput &seg_output = bevent->seg_output;
    if (seg_output.seg_tag_list.size() > 0) {
        auto &ner_list = seg_output.seg_tag_list[0].ner_list;
        for (auto &entity : ner_list) {
            LOG_INFO << "entity.text: " << entity.text.c_str()
                     << " , term_begin: " << entity.term_begin
                     << " , term_end: " << entity.term_end;
        }
    }
    bell::LuceneRecall lucene_recall;
    lucene_recall.test();
    LOG_INFO << "lucene_recall done..";
    return BELL_SUCESS;
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