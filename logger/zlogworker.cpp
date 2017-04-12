/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zlogworker.cpp
 * Author: alexcon
 * 
 * Created on April 11, 2017, 2:35 PM
 */

#include "zlogworker.h"
namespace dmsz {
    namespace log {

        zlogworker::zlogworker(zmqpp::context& ctx, std::string& endpoint) :
        m_ctx(ctx),
        m_endpoint(endpoint),
        m_socket(ctx, zmqpp::socket_type::dealer) {

        }

        zlogworker::~zlogworker() {
            m_socket.close();
        }

        void zlogworker::work() {
            m_socket.connect(m_endpoint);
            while (true) {
                zmqpp::message msg;
                m_socket.receive(msg);
                if (msg.parts()) {
                    log(msg);
                }
            }

        }

        void zlogworker::log(zmqpp::message& msg) {
            std::string identity = msg.get<std::string>(0);
            std::string text = msg.get<std::string>(1);
            //msg >> identity >> text;
            std::cout << "text: " << text << " " << identity << std::endl;
        }
    }
}