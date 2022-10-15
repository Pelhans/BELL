#include "entity_link_handler.h"
#include <unistd.h>

bool BELLServer::start() {
  // Sleep(50000);
  WordSeg word_seg_handler;
  std::string req = "周杰伦";
  word_seg_handler.work(req);
  std::cout << req << std::endl;
  return true;
}

