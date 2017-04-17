/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cc
 * Author: alexcon
 *
 * Created on April 14, 2017, 4:04 PM
 */

#include <cstdlib>
#include "def.h"
#include "zlog.h"
#include "zlogpull.h"
using namespace std;
static string tcp_endpoint("tcp://127.0.0.1:33353");
static dmsz::log::zlogpull logpull;

/*
 * 
 */
void run(dmsz::log::zlog_m& logger) {
    for (int i = 0; i < 1000000; i++) {
        INFO(logger, "          MT " + std::to_string(i));
    }

}

int main(int argc, char** argv) {
    //getchar();
    dmsz::log::zlog_m logger1 = logpull.logger_m();
    dmsz::log::zlogm logger3(tcp_endpoint);
    dmsz::log::zlog_s logger2 = logpull.logger_s();
    thread t(std::bind(&run, logger1));
    t.detach();
    for (int i = 0; i < 1000000; i++) {
        logger1->info("MT " + std::to_string(i));
        logger3.info("   TCP" + std::to_string(i));
        logger2->info("   ST  " + std::to_string(i));
    }
    getchar();
    return 0;
}

