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

        zlogworker::zlogworker() {
        }

        zlogworker::~zlogworker() {
        }

        void zlogworker::work(zmqpp::context* ctx, const std::string& bind_string) {
            zmqpp::socket socket(*ctx, zmqpp::socket_type::subscribe);
            socket.connect(bind_string);
            try {
                while (true) {
                    zmqpp::message req;
                    socket.receive(req);
                    log(req);
                }
            } catch (std::exception &e) {
            }
        }

        void zlogworker::log(zmqpp::message& msg) {
            std::string text;
            int number;
            msg >> text >> number;
            std::cout << "text: " << text << std::endl;
        }
    }
}