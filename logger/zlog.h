/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zlogger.h
 * Author: alexcon
 *
 * Created on April 10, 2017, 1:49 PM
 */

#ifndef ZLOG_H
#define ZLOG_H

#include <cstdlib>
#include <iostream>
#include <chrono>
#include <mutex>
#include <uuid/uuid.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/time.h>
#include <zmqpp/zmqpp.hpp>
#include <zmqpp/proxy.hpp>

#include "macrodef.h"
#include "zlogpull.h"


namespace dmsz {
    namespace log {

        enum FACILITY {
            STARTUP,
            CORE
        };

        enum level {
            info,
            warning,
            error,
            fatal
        };

        struct null_lock_t {

            void
            lock() const {
            }

            bool
            try_lock() const {
                return true;
            }

            void
            unlock() const {
            }
        };

        template < typename LOCK >
        class zlog {
        public:

            zlog(zmqpp::endpoint_t& endpoint) :
            m_ctx(),
            m_push(m_ctx, zmqpp::socket_type::push),
            m_lock( std::make_shared< LOCK >()){
                m_push.connect(endpoint);
            }

            virtual ~zlog() {
            };
            
            void
            info(std::string str) {
                std::unique_lock< LOCK > lock(*m_lock);
                using namespace std;
                using namespace chrono;
                auto now = system_clock::now();
                auto ms = duration_cast< milliseconds >(now.time_since_epoch());
                time_t unix_time = duration_cast< seconds >(ms).count();
                std::string text = fmt::format("[ {:%Y-%m-%d %H:%M:%S}] {}", *localtime(&unix_time), str);
                try {
                    zmqpp::message msg;
                    msg << text;
                    m_push.send(msg);
                } catch (zmqpp::exception& e) {
                    std::cout << "poll exception :" << e.what() << std::endl;
                }
            }

        private:
            zmqpp::context m_ctx;
            zmqpp::socket m_push;
            std::shared_ptr< LOCK > m_lock;//{ std::make_shared< LOCK >()};
        };
        using zlog_st = zlog<null_lock_t>;
        using zlog_mt = zlog<std::mutex>;

    }// namespace log
}// namespace dmsz

#endif /* ZLOG_H */
