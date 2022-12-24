#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#define PORT 2023
using namespace std;

class Timer {
public:
    void start() { gettimeofday(&start_, NULL); }
    void end() {
        gettimeofday(&end_, NULL);
        cost_ = (int32_t)((end_.tv_sec - start_.tv_sec) * 1000000 +
                          end_.tv_usec - start_.tv_usec);
    }
    int32_t ucost() { return cost_; };

private:
    int32_t cost_;
    struct timeval start_;
    struct timeval end_;
};

int main(int argc, char *argv[]) {
    int sockFd = 0;
    char buffer[1024] = {0};
    int up_limit = 10000;
    int cur_req = 0;
    //char* helloFromClient = "hello from client";
    char* helloFromClient = argv[1];
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);
    address.sin_port = htons(PORT);
    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    

    connect(sockFd, (struct sockaddr*)&address, sizeof(address));
	Timer timer;
	timer.start();
    while(cur_req < up_limit) {
        cur_req += 1;
        send(sockFd, helloFromClient, strlen(helloFromClient), 0);
      //  printf("client sent\n");
        read(sockFd, buffer, 1024);
    //    printf("read message:%s\n", buffer);
    }
	timer.end();
	printf("请求 %d 次共耗时 %d us", up_limit, timer.ucost());
    return 0;
}
