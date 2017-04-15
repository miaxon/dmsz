#include <cstdlib>
#include <iostream>
#include <string>

#include <chrono>
#include <thread>

#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/unistd.h>
#include <sys/select.h>
#include <atomic>
#include <thread>
#include <vector>
#include <sstream>

#include <libdaemon/dfork.h>
#include <libdaemon/dsignal.h>
#include <libdaemon/dlog.h>
#include <libdaemon/dpid.h>
#include <libdaemon/dexec.h>


#include "def.h"
#include "hpp/optionparser.h"
#include "zlog.h"
#include "INIReader.h"
#include "zlogpull.h"
#include <zmqpp/context.hpp>

static std::string tcp_endpoint("tcp://127.0.0.1:33353");
static std::string ipc_endpoint("ipc://11111111");
static dmsz::log::zlogpull logpull(zmqpp::poller::wait_forever);
dmsz::log::zlog_st tcp_log(tcp_endpoint);
dmsz::log::zlog_st ipc_log(ipc_endpoint);
dmsz::log::zlog_st inp_log;

dmsz::log::zlog_mt tcp_logm(tcp_endpoint);
dmsz::log::zlog_mt ipc_logm(ipc_endpoint);
dmsz::log::zlog_mt inp_logm;

enum optionIndex {
    UNKNOWN, HELP, DAEMON
};
const option::Descriptor usage[] = {
    {UNKNOWN, 0, "", "", option::Arg::None, "USAGE: dmsz [options]\n\n"
        "Options:"},
    {HELP, 0, "h", "help", option::Arg::None, "  --help, -h  \tPrint usage and exit."},
    {DAEMON, 0, "d", "daemon", option::Arg::None, "  --daemon, -d  \tRun as daemon."},
    {0, 0, 0, 0, 0, 0}
};
#define MULTI_THREAD

void
test()
{

    using namespace std;
    using namespace std::chrono;

    unsigned int howmany = 1000000;
    vector<thread> threads;
    auto start = system_clock::now();


#if !defined(MULTI_THREAD)

    for (unsigned int i = 0; i < howmany; i++) {
        //Has to be customized for every logger
        logger.info(logger.protostring() + " #" + std::to_string(i));
        loggerm.info(loggerm.protostring() + " #" + std::to_string(i));
        ipc_logm.info(ipc_logm.protostring() + " #" + std::to_string(i));
    }
#else    
    int thread_count = 4;
    howmany /= thread_count;
    for (int t = 0; t < thread_count; ++t) {
        threads.push_back(std::thread([&] {
            dmsz::log::zlog_st logger;
            for (unsigned int i = 0; i < howmany; i++) {
                //Has to be customized for every logger
                //stringstream ss;
                //ss << std::this_thread::get_id() << " Message #" << i;
                logger.info("#" + std::to_string(i));
                        //std::this_thread::sleep_for (std::chrono::nanoseconds(10));
            }
        }));
    }


    for (auto &t : threads) {
        t.join();
    };

    howmany *= thread_count;

#endif

    auto delta = system_clock::now() - start;
    auto delta_d = duration_cast<duration<double>> (delta).count();

    stringstream ss;
    ss << "Time = " << ((double) howmany / delta_d) << " per second, total time = " << delta_d;
    std::cout << ss.str() << std::endl;

    //Logger uninitialization if necessary 
}

int
main(int argc, char** argv)
{

    //getchar();
    //test();
    //getchar();
    dmsz::log::zlog_st logger;
    //std::getchar();

    //logger.info("");
    logger.info("sdsdsd");
    //logger.info("sdsdsd");
    //logger.info("sdsdsd");
    //getchar();
   logpull.stop();
   //getchar();
    logpull.start();
    //logger.info("");
    //logger.info("sdsdsd");
    logger.info("sdsdsd");
    //logger.info("sdsdsd");
    return 0;
}
/*dmsz::log::zlog logger(log_endpoint);
std::getchar();

logger.info("");
logger.info("sdsdsd");
logger.info("sdsdsd");
logger.info("sdsdsd");

INIReader reader("dms.conf");
    
if (reader.ParseError() < 0) {
    std::cout << "Can't load 'dms.conf'\n";
    return 1;
}
std::cout << "Config loaded from 'test.ini': version="
          << reader.GetInteger("protocol", "version", -1) << ", name="
          << reader.Get("user", "name", "UNKNOWN") << ", email="
          << reader.Get("user", "email", "UNKNOWN") << ", pi="
          << reader.GetReal("user", "pi", -1) << ", active="
          << reader.GetBoolean("user", "active", true) << "\n";
argc -= (argc > 0);
argv += (argc > 0); // skip program name argv[0] if present
option::Stats stats(usage, argc, argv);
std::vector<option::Option> options(stats.options_max);
std::vector<option::Option> buffer(stats.buffer_max);
option::Parser parse(usage, argc, argv, &options[0], &buffer[0]);

if (parse.error())
    return 1;

if (options[HELP] || argc == 0) {
    option::printUsage(std::cout, usage);
    return 0;
}


for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next()) {
    std::cout << "Unknown option: " << std::string(opt->name, opt->namelen) << "\n";
}

for (int i = 0; i < parse.nonOptionsCount(); ++i) {
    std::cout << "Non-option #" << i << ": " << parse.nonOption(i) << "\n";
}
std::cout << GIT_VERSION << std::endl;

std::getchar();
return 0;*/
//}
