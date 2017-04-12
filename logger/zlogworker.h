/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zlogworker.h
 * Author: alexcon
 *
 * Created on April 11, 2017, 2:35 PM
 */

#ifndef ZLOGWORKER_H
#define ZLOGWORKER_H

#include <zmqpp/zmqpp.hpp>
#include <zmqpp/proxy.hpp>
namespace dmsz {
    namespace log {

        class zlogworker {
        public:
            zlogworker(zmqpp::context& ctx, std::string& endpoint);
            virtual ~zlogworker();
            void work();
        private:
            zmqpp::socket_t m_socket;
            zmqpp::context_t &m_ctx;
            std::string m_endpoint;
            void log(zmqpp::message& msg);
        };
    }
}
#endif /* ZLOGWORKER_H */

