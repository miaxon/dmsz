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
#include <future>
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
            zlogpull(long poll_timeout = 10);
            virtual ~zlogpull();
            static const std::string&
            inproc_endpoint()
            {
                return m_inp_endpoint;
            }
            void start(long pull_timeout = 10);
            void stop();
        private:
            std::thread spawn();
            bool run();
            void route(zmqpp::message& msg) const;
            void in_tcp();
            void in_ipc();
            void in_inp();
            void in_ctl();
            std::string uuid();
        private:
            zmqpp::context m_ctx;
            zmqpp::socket m_tcp;
            zmqpp::socket m_ipc;
            zmqpp::socket m_inp;
            zmqpp::socket m_ctl; // control channel
            volatile bool m_run;
            volatile long m_poll_timeout;
            std::future<bool> m_fut;
            zmqpp::reactor m_reactor;
            std::string m_tcp_endpoint;            
            std::string m_ipc_endpoint;
            std::string m_ctl_endpoint;
            static std::string m_inp_endpoint;
            
        };
    }
}
#endif /* ZLOGPROXY_H */

