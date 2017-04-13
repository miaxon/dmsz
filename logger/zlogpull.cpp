/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zlogproxy.cpp
 * Author: alexcon
 * 
 * Created on April 11, 2017, 2:18 PM
 */

#include "zlogpull.h"
namespace dmsz {
    namespace log {

        zlogpull::zlogpull(const zmqpp::endpoint_t& endpoint, int workers) :
        m_endpoint(endpoint),
        m_workers(workers),
        m_ctx() {
            m_ctx.set(zmqpp::context_option::io_threads, workers);
            std::thread t(std::bind(&dmsz::log::zlogpull::run, this));
            t.detach();
        }

        zlogpull::~zlogpull() {

        }

        void zlogpull::run() {
            zmqpp::socket router(m_ctx, zmqpp::socket_type::pull);
            router.bind(m_endpoint);
            while (true) {
                zmqpp::message msg;
                router.receive(msg);
                if (msg.parts())
                    log(msg);
            }
        }

        void zlogpull::log(zmqpp::message& msg) const {
            std::string key = msg.get(0);
            //std::string body = msg.get(1);
            std::cout << key << "\n"; //and a number: " << body << std::endl;
        }
    }
}
