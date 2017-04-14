#include <cstdlib>
#include <iostream>
#include <string>

#include <chrono>
#include <thread>

#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/unistd.h>
#include <sys/select.h>
#include <atomic>
#include <thread>
#include <vector>
#include <sstream>

#include <libdaemon/dfork.h>
#include <libdaemon/dsignal.h>
#include <libdaemon/dlog.h>
#include <libdaemon/dpid.h>
#include <libdaemon/dexec.h>


#include "def.h"
#include "hpp/optionparser.h"
#include "zlog.h"
#include "INIReader.h"
#include "zlogpull.h"
#include <zmqpp/context.hpp>

static std::string tcp_endpoint("tcp://127.0.0.1:33353");
static dmsz::log::zlogpull logpull;

//#define MULTI_THREAD

void test() {    

    using namespace std;
    using namespace std::chrono;

    unsigned int howmany = 1000000;
    vector<thread> threads;
    auto start = system_clock::now();


#if !defined(MULTI_THREAD)
    dmsz::log::zlog_st logger(tcp_endpoint);
    for (unsigned int i = 0; i < howmany; i++) {
        //Has to be customized for every logger
        INFO("asdasdasd");
    }
#else    
    int thread_count = 4;
    howmany /= thread_count;
    //dmsz::log::zlog_mt logger;
    for (int t = 0; t < thread_count; ++t) {
        threads.push_back(std::thread([&] {
            dmsz::log::zlog_st logger;
            for (unsigned int i = 0; i < howmany; i++) {
                //Has to be customized for every logger
                //stringstream ss;
                //ss << std::this_thread::get_id() << " Message #" << i;
                //INFO("#" + std::to_string(i));
                        //std::this_thread::sleep_for (std::chrono::nanoseconds(10));
            }
        }));
    }


    for (auto &t : threads) {
        t.join();
    };

    howmany *= thread_count;

#endif

    auto delta = system_clock::now() - start;
    auto delta_d = duration_cast<duration<double>> (delta).count();

    stringstream ss;
    ss << "Time = " << ((double) howmany / delta_d) << " per second, total time = " << delta_d;
    std::cout << ss.str() << std::endl;

    //Logger uninitialization if necessary 
}

int main(int argc, char** argv) {

    dmsz::log::zlog_st logger(tcp_endpoint);
    INFO("start");

    return 0;
}
