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
#include <unistd.h>
#include <fcntl.h>
#include "zlogworker.h"

#define o(x) std::cout << x << std::endl
namespace dmsz {
    namespace log {

        enum cmd{
            endpoint
        };
        class zlogpull {
        public:
            static zmqpp::context*  ctx;
        public:
            zlogpull(long poll_timeout = zmqpp::poller::wait_forever);
            virtual ~zlogpull();
            static const std::string&
            inproc_endpoint()
            {
                return m_inp_endpoint;
            }
            void start(long pull_timeout = zmqpp::poller::wait_forever);
            void stop();
        private:
            std::thread spawn();
            bool run();
            void route(zmqpp::message& msg) const;
            void in_tcp();
            void in_ipc();
            void in_inp();
            void in_ctl();
            void poll_reset();
            void poll_cancel();
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
            std::string m_tcp_endpoint;            
            std::string m_ipc_endpoint;
            std::string m_ctl_endpoint;
            int m_pipe[2];
            static std::string m_inp_endpoint;
            
        };
    }
}
#endif /* ZLOGPROXY_H */

