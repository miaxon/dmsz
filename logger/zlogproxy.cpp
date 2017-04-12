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

        zlogproxy::zlogproxy(const zmqpp::endpoint_t& endpoint, int workers) :
        m_endpoint(endpoint),
        m_workers(workers),
        m_ctx(){
            m_ctx.set(zmqpp::context_option::io_threads, 16);
            std::thread t(std::bind(&dmsz::log::zlogproxy::run, this));
            t.detach();
        }

        zlogproxy::~zlogproxy() {

        }

        void zlogproxy::run() {
            zmqpp::socket router(m_ctx, zmqpp::socket_type::pull);
            router.bind(m_endpoint);
            while(true)
            {
                zmqpp::message msg;
                router.receive(msg);
                if(msg.parts())
                    log(msg);
            }
            /*uuid_t uuid;
            uuid_generate(uuid);
            char uuid_str[37];
            uuid_unparse_lower(uuid, uuid_str);
            std::string route = fmt::format("inproc://{}", uuid_str);
            zmqpp::socket dealer(m_ctx, zmqpp::socket_type::dealer);
            dealer.bind(route);

            std::cout << "proxy starting: " << m_endpoint << std::endl;

            std::vector<zlogworker*> workers;
            std::vector<std::thread*> threads;
            for (int i = 0; i < m_workers; ++i) {
                workers.push_back(new zlogworker(m_ctx, route));
                threads.push_back(new std::thread(std::bind(&zlogworker::work, workers[i])));
                threads[i]->detach();
            }
            try {
                zmqpp::proxy(router, dealer);
            } catch (std::exception &e) {
            }
            for (int i = 0; i < m_workers; ++i) {
                delete workers[i];
                delete threads[i];
            }*/
        }

        void zlogproxy::log(zmqpp::message& msg) const {
            if (msg.parts()) {
                std::string key = msg.get(0);
                //std::string body = msg.get(1);
                std::cout << "Received text:\"" << key << "\n";//and a number: " << body << std::endl;
            }
        }
    }
}
