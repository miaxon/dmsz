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

        zlogproxy::zlogproxy(const zmqpp::context& ctx, const zmqpp::endpoint_t& endpoint, int nworkers) :
        m_recv(ctx, zmqpp::socket_type::subscribe),
        m_endpoint(endpoint){
            
            std::thread t(std::bind(&dmsz::log::zlogproxy::run, this));
            t.detach();
        }

        zlogproxy::~zlogproxy() {
            m_recv.unsubscribe(m_endpoint);
            m_recv.close();
        }
        void zlogproxy::stop(){
            m_ready = false;
        }
        void zlogproxy::run() {
            m_ready = true;
            try{
            m_recv.connect(m_endpoint);
            }
            catch(std::exception &e)
            {
                std::cout << e.what();
            }
            std::cout << "proxy starting: " << m_endpoint << std::endl;
            while (m_ready) {
                zmqpp::message req;
                std::string str;
                m_recv.receive(str);
                log(str);             
            }
            m_recv.disconnect(m_endpoint);            
        }

        void zlogproxy::log(std::string& msg) const {
            std::cout << "text: " << std::endl;
            //std::string text = msg.get<std::string>(0);
            std::cout << "text: " << msg << std::endl;
        }
    }
}
