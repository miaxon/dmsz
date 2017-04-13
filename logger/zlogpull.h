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

        class zlogpull {
        public:
            zlogpull();
            virtual ~zlogpull();

            zmqpp::context& ctx() {
                return m_ctx;
            }

            const std::string& ipc() {
                return m_ipc_endpoint;
            }

            const std::string& inp() {
                return m_inp_endpoint;
            }

            const std::string& tcp() {
                return m_tcp_endpoint;
            }
        private:
            std::thread spawn();
            void run();
            void route(zmqpp::message& msg) const;
            void in_tcp();
            void in_ipc();
            void in_inp();
            std::string uuid();
        private:
            zmqpp::context m_ctx;
            zmqpp::socket m_tcp;
            zmqpp::socket m_ipc;
            zmqpp::socket m_inp;
            volatile bool m_run;
            std::thread m_thread;
            zmqpp::reactor m_reactor;
            std::string m_tcp_endpoint;
            std::string m_inp_endpoint;
            std::string m_ipc_endpoint;


        };
    }
}
#endif /* ZLOGPROXY_H */

