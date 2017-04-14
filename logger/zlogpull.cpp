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

        zlogpull::zlogpull() :
        m_ctx(),
        m_tcp(m_ctx, zmqpp::socket_type::pull) {
            std::thread t(std::bind(&dmsz::log::zlogpull::run, this));
            t.detach();
        }

        zlogpull::~zlogpull() {
            zmqpp::socket s(m_ctx, zmqpp::socket_type::push);
            s.connect("tcp://127.0.0.1:33353");
            s.send(zmqpp::signal::stop);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        void
        zlogpull::run() {
            m_tcp.bind("tcp://*:33353");
            zmqpp::message msg;
            while (true) {                
                m_tcp.receive(msg);
                if (msg.is_signal())
                    break;
                route(msg);
            }
        }

        void
        zlogpull::route(zmqpp::message& msg) const {
            std::string key = msg.get(0);
            //std::string body = msg.get(1);
            std::cout << key << "\n"; //and a number: " << body << std::endl;
        }


    }
}
