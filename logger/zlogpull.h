/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zlogproxy.h
 * Author: alexcon
 *
 * Created on April 11, 2017, 2:18 PM
 */

#ifndef ZLOGPULL_H
#define ZLOGPULL_H

#include <zmqpp/zmqpp.hpp>
#include <zmqpp/proxy.hpp>
#include <uuid/uuid.h>
#include <fmt/format.h>
#include <chrono>
#include <atomic>

#include "zlogworker.h"
namespace dmsz {
    namespace log {

        enum cmd{
            endpoint
        };
        class zlogpull {
        public:
            static zmqpp::context*  ctx;
        public:
            zlogpull();
            virtual ~zlogpull();            
        private:            
            void run();
            void route(zmqpp::message& msg) const;           
        private:
            zmqpp::context m_ctx;
            zmqpp::socket m_tcp;
        };
    }
}
#endif /* ZLOGPROXY_H */

