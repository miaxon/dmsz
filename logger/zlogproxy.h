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

//#include "zlogworker.h"
namespace dmsz {
    namespace log {

        class zlogproxy {
        public:
            zlogproxy(const zmqpp::context& ctx, const zmqpp::endpoint_t& endpoint, int nworkers);
            virtual ~zlogproxy();
            void stop();
        private:
            bool m_ready;
            void run();
            std::string m_endpoint;
            zmqpp::socket m_recv;
            void log(std::string& msg) const;
        };
    }
}
#endif /* ZLOGPROXY_H */

