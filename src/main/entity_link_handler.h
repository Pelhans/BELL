#include <iostream>
#include <functional>
#include <utility>

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include "seg_handler.h"

class BELLServer {
 public:
  BELLServer() {};
  bool start();
};