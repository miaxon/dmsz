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
        m_ctx(),
        m_zsock(m_ctx, zmqpp::socket_type::push) {
            uuid_t uuid;
            uuid_generate(uuid);
            char uuid_str[37];
            uuid_unparse_lower(uuid, uuid_str);
            m_zsock.set(zmqpp::socket_option::identity, std::string(uuid_str));
            m_zsock.connect(endpoint);
        }

        zlog::~zlog() {
            m_zsock.close();
        }

        void zlog::info(std::string str) {

            using namespace std;
            using namespace chrono;
            auto now = system_clock::now();
            auto ms = duration_cast< milliseconds >(now.time_since_epoch());
            time_t unix_time = duration_cast< seconds >(ms).count();
            std::string text = fmt::format("[ {:%Y-%m-%d %H:%M:%S}] {}", *localtime(&unix_time), str);
            zmqpp::message msg;
            msg  << text;
            m_zsock.send(msg, true);
        }
    }
}

