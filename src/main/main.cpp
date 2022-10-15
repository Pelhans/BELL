#include <functional>
#include <utility>

#include <stdio.h>
#include <unistd.h>

#include "entity_link_handler.h"


int main(int argc, char* argv[])
{
  BELLServer server;
  server.start();
  std::cout<<"可以跑？？"<<std::endl;
  return 0;
}
