#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include "util/xml_helper.h"
#include "muduo/base/Logging.h"
#include "cnn_crf.h"

using namespace bell;

bool CNN_CRF::load(std::string& cnn_config) {
    XmlDoc parser;
    XmlNode root = parser.parser(cnn_config.c_str());
}