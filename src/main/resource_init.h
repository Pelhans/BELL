#pragma once

#include "bell_config.h"

#include <string>
#include <thread>

class ResourceInit {
public:
    static bool init(const bell::BellConfig& config);
//    static void async_init(const bell::BellConfig& config);
//    static void async_init_wait();

private:
    static bool init_func(const bell::BellConfig& config);

    static bool initWordSeg(const std::string& model_name, const std::string& cnn_crf_config);

// private:
//    static std::shared_ptr<std::thread> thd;
};
