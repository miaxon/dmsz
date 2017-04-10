/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zlogger.h
 * Author: alexcon
 *
 * Created on April 10, 2017, 1:49 PM
 */

#ifndef ZLOG_H
#define ZLOG_H

#include <cstdlib>
#include <iostream>
#include <chrono>

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/time.h>

namespace dmsz {
    namespace log {

        enum FACILITY {
            STARTUP,
            CORE
        };

        enum LEVEL {
            INFO,
            WARNING,
            ERROR,
            FATAL
        };

        class zlog {
        public:
            zlog();
            zlog(const zlog&);
            virtual ~zlog();
            void info(std::string);
        private:

        };
    }
}


#endif /* ZLOGGER_H */

