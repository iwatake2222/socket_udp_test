/* Copyright 2021 iwatake2222

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
/*** Include ***/
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
#include <chrono>
#include <thread>

#include "my_udp.h"

/*** Macro ***/

/*** Global variable ***/
bool do_exit = false;


/*** Function ***/
static std::string GetDateTimeText()
{
    char text[64];
    time_t time_now = time(NULL);
    struct tm* local_time = localtime(&time_now);
    strftime(text, sizeof(text), "%Y/%m/%d_%H:%M:%S", local_time);
    return std::string(text);
}


static void ThreadSender()
{
    MyUdp udp_send("127.0.0.1", 1234);
    MyUdp udp_recv("0.0.0.0", 5678, MyUdp::Mode::kModeRecvBlocking);
    for (int32_t cnt = 0; ; cnt++) {
        if (do_exit) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        char buf[1024];
        snprintf(buf, sizeof(buf), "message_%010d", cnt);
        udp_send.Send(buf, static_cast<int32_t>(strnlen(buf, sizeof(buf))));
        printf("[%s][ThreadSender] send = %s\n", GetDateTimeText().c_str(), buf);
        
        memset(buf, 0, sizeof(buf));
        udp_recv.Recv(buf, sizeof(buf));
        printf("[%s][ThreadSender] recv = %s\n", GetDateTimeText().c_str(), buf);
    }
}

static void ThreadEchoBacker()
{
    MyUdp udp_recv("0.0.0.0", 1234, MyUdp::Mode::kModeRecvBlocking);
    MyUdp udp_send("127.0.0.1", 5678);
    while (true) {
        if (do_exit) break;
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        udp_recv.Recv(buf, sizeof(buf));
        printf("[%s][ThreadEchoBacker] recv = %s\n", GetDateTimeText().c_str(), buf);

        udp_send.Send(buf, sizeof(buf));
    }
}


int main(int argc, char *argv[])
{
    std::thread thread_sender(ThreadSender);
    std::thread thread_echo(ThreadEchoBacker);
    
    (void)getchar();
    do_exit = true;
            
    thread_sender.join();
    thread_echo.join();

    return 0;
}
