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

        zlogproxy::zlogproxy(zmqpp::endpoint_t& endpoint, int nworkers) :
        m_ctx(),
        m_recv(m_ctx, zmqpp::socket_type::subscribe),
        m_endpoint(endpoint){
            std::thread t(std::bind(&dmsz::log::zlogproxy::run, this));
            t.detach();
        }

        zlogproxy::~zlogproxy() {
        }

        void zlogproxy::run() {
            std::cout << "proxy starting: " << m_endpoint << std::endl;
            try{
            m_recv.connect(m_endpoint);
            }
            catch(std::exception &e)
            {
                std::cout << e.what();
            }
            while (true) {
                zmqpp::message req;
                m_recv.receive(req, true);
                log(req);               

            }
        }

        void zlogproxy::log(zmqpp::message& msg) {
            std::string text;
            int number;
            msg >> text >> number;
            std::cout << "text: " << text << std::endl;
        }
    }
}
