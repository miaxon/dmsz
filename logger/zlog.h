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

#include "zlogpull.h"


namespace dmsz {
    namespace log {

        enum FACILITY {
            STARTUP,
            CORE
        };

        enum LEVEL {
            INFO,
            WARNING,
            ERROR,
            FATAL
        };

        enum proto {
            tcp,
            ipc,
            inproc
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

            zlog() :
            m_ctx(dmsz::log::zlogpull::ctx),
            m_push(*m_ctx, zmqpp::socket_type::push),
            m_proto(dmsz::log::proto::inproc) {
                m_push.connect(dmsz::log::zlogpull::inproc_endpoint());
            }

            zlog(zmqpp::endpoint_t& endpoint) :
            m_ctx_internal(),
            m_ctx(&m_ctx_internal),
            m_push(*m_ctx, zmqpp::socket_type::push),
            m_proto(endpoint[0] == 'i'? dmsz::log::proto::ipc : dmsz::log::proto::tcp) {
                m_push.connect(endpoint);
            }

            virtual ~zlog() {
            };

            dmsz::log::proto
            proto() {
                return m_proto;
            }

            std::string
            protostring() {
                switch (m_proto) {
                    case tcp:
                        return "tcp";
                    case ipc:
                        return "ipc";
                    case inproc:
                        return "inproc";
                }
                return "undefined";
            }

            void
            info(std::string str) {
                std::unique_lock< LOCK > lock(*m_lock);
                using namespace std;
                using namespace chrono;
                auto now = system_clock::now();
                auto ms = duration_cast< milliseconds >(now.time_since_epoch());
                time_t unix_time = duration_cast< seconds >(ms).count();
                std::string text = fmt::format("[ {:%Y-%m-%d %H:%M:%S}] {}", *localtime(&unix_time), str);
                zmqpp::message msg;
                msg << text;
                m_push.send(msg);
            }
       
        private:
            zmqpp::context m_ctx_internal;
            zmqpp::context* m_ctx;
            zmqpp::socket m_push;
            dmsz::log::proto m_proto;
            std::shared_ptr< LOCK > m_lock{ std::make_shared< LOCK >()};
            


        };
        using zlog_st = zlog<null_lock_t>;
        using zlog_mt = zlog<std::mutex>;

    }// namespace log
}// namespace dmsz


#endif /* ZLOGGER_H */

