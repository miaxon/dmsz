/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zlogger.cpp
 * Author: alexcon
 * 
 * Created on April 10, 2017, 1:49 PM
 */
#include "zlog.h"
namespace dmsz {
    namespace log {

        zlog::zlog(const zmqpp::endpoint_t& endpoint) :
        m_pub(m_ctx, zmqpp::socket_type::publish) {
            m_pub.bind(endpoint);
        }

        zlog::~zlog() {
        }

        void zlog::info(std::string str ) {

            using namespace std;
            using namespace chrono;
            auto now = system_clock::now();
            auto ms = duration_cast< milliseconds >(now.time_since_epoch());
            time_t unix_time = duration_cast< seconds >(ms).count();
            zmqpp::message msg;
            msg << "A" << fmt::format(
                    "[ {:%Y-%m-%d %H:%M:%S}] {}",
                    *localtime(&unix_time), str);
            m_pub.send(msg, true);
        }
    }
}

