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

#ifndef ZLOGPROXY_H
#define ZLOGPROXY_H

#include <zmqpp/zmqpp.hpp>
#include <zmqpp/proxy.hpp>
#include <uuid/uuid.h>
#include <fmt/format.h>

#include "zlogworker.h"
namespace dmsz {
    namespace log {

        class zlogproxy {
        public:
            zlogproxy(const zmqpp::endpoint_t& endpoint, int workers = 4);
            virtual ~zlogproxy();
        private:
            void run();
            zmqpp::context m_ctx;
            std::string m_endpoint;
            int m_workers;
            void log(zmqpp::message& msg) const;
        };
    }
}
#endif /* ZLOGPROXY_H */

