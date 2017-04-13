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
#include "zlog.h"
zmqpp::context* dmsz::log::zlogpull::ctx = nullptr;
std::string dmsz::log::zlogpull::m_inp_endpoint;
namespace dmsz {
    namespace log {

        zlogpull::zlogpull() :
        m_ctx(),
        m_tcp(m_ctx, zmqpp::socket_type::pull),
        m_ipc(m_ctx, zmqpp::socket_type::pull),
        m_inp(m_ctx, zmqpp::socket_type::pull),
        m_ctl(m_ctx, zmqpp::socket_type::reply),
        m_run(true),
        m_thread(spawn()),
        m_reactor()
        {
            if (!ctx)
                ctx = &m_ctx;
        }

        std::string
        zlogpull::uuid()
        {
            uuid_t uuid;
            uuid_generate(uuid);
            char uuid_str[37];
            uuid_unparse_lower(uuid, uuid_str);
            return std::string(uuid_str);
        }

        std::thread
        zlogpull::spawn()
        {
            std::thread t(std::bind(&dmsz::log::zlogpull::run, this));
            t.detach();
            return t;
        }

        zlogpull::~zlogpull()
        {
            m_run = false;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        void
        zlogpull::run()
        {
            m_inp_endpoint = fmt::format("inproc://{}", uuid());
            m_tcp_endpoint = "tcp://*:33353";
            m_ctl_endpoint = "tcp://*:33355";
            m_ipc_endpoint = fmt::format("ipc://{}", uuid());

            m_tcp.bind(m_tcp_endpoint);
            m_ipc.bind(m_ipc_endpoint);
            m_inp.bind(m_inp_endpoint);
            m_ctl.bind(m_ctl_endpoint);
            m_reactor.add(m_tcp, std::bind(&dmsz::log::zlogpull::in_tcp, this));
            m_reactor.add(m_ipc, std::bind(&dmsz::log::zlogpull::in_ipc, this));
            m_reactor.add(m_inp, std::bind(&dmsz::log::zlogpull::in_inp, this));
            m_reactor.add(m_ctl, std::bind(&dmsz::log::zlogpull::in_ctl, this));
            while (m_run && m_reactor.poll()) {
            }

        }

        void
        zlogpull::route(zmqpp::message& msg) const
        {
            std::string key = msg.get(0);
            //std::string body = msg.get(1);
            std::cout << key << "\n"; //and a number: " << body << std::endl;
        }

        void
        zlogpull::in_tcp()
        {
            if (!m_run) return;
            zmqpp::message msg;
            m_tcp.receive(msg);
            if (msg.parts())
                route(msg);
        }

        void
        zlogpull::in_ipc()
        {
            if (!m_run) return;
            zmqpp::message msg;
            m_ipc.receive(msg);
            if (msg.parts())
                route(msg);
        }

        void
        zlogpull::in_inp()
        {
            if (!m_run) return;
            zmqpp::message msg;
            m_inp.receive(msg);
            if (msg.parts())
                route(msg);
        }

        void
        zlogpull::in_ctl()
        {
            if (!m_run) return;
            zmqpp::message msg;
            m_ctl.receive(msg);
            if (!msg.parts()) return;
            zmqpp::message ret;
            int cmd;
            msg.get(cmd, 0);
            if(cmd == (int)dmsz::log::cmd::endpoint)
            {
                int proto;
                msg.get(proto, 1);
                if(proto == (int)dmsz::log::proto::ipc)
                    ret << m_ipc_endpoint;
                if(proto == (int)dmsz::log::proto::tcp)
                    ret << m_tcp_endpoint;
                m_ctl.send(ret);
            }

        }
    }
}
