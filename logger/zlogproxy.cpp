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

#include "zlogproxy.h"
namespace dmsz {
    namespace log {

        zlogproxy::zlogproxy(const zmqpp::endpoint_t& endpoint) :

        m_endpoint(endpoint) {

            std::thread t(std::bind(&dmsz::log::zlogproxy::run, this));
            t.detach();
        }

        zlogproxy::~zlogproxy() {
            
        }

        void zlogproxy::stop() {
            m_ready = false;
        }

        void zlogproxy::run() {
            m_ready = true;
            zmqpp::context ctx;
            zmqpp::socket sub(ctx, zmqpp::socket_type::subscribe);
            sub.connect(m_endpoint);
            sub.subscribe("A");
            std::cout << "proxy starting: " << m_endpoint << std::endl;
            while (m_ready) {
                zmqpp::message msg;
                sub.receive(msg);
                log(msg);
            }
            sub.unsubscribe(m_endpoint);
            sub.disconnect(m_endpoint);
            sub.close();
            ctx.terminate();
        }

        void zlogproxy::log(zmqpp::message& msg) const {
            if (msg.parts()) {
                std::string key = msg.get(0);
                std::string body = msg.get(1);
                std::cout << "Received text:\"" << key << "\" and a number: " << body << std::endl;
            }
        }
    }
}
