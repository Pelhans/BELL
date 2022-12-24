#include <iostream>
#include <string>
#include "common/event/word_seg_output.h"
#include "common/event/bell_handler_event.h"



class WordSeg {
public:
    int work(std::shared_ptr<bell::Event> bevent);
};