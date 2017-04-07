#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/unistd.h>
#include <sys/select.h>

#include <libdaemon/dfork.h>
#include <libdaemon/dsignal.h>
#include <libdaemon/dlog.h>
#include <libdaemon/dpid.h>
#include <libdaemon/dexec.h>


#include "def.h"
#include "hpp/optionparser.h"

enum optionIndex {
    UNKNOWN, HELP, DAEMON
};
const option::Descriptor usage[] ={
    {UNKNOWN, 0, "", "", option::Arg::None, "USAGE: dms [options]\n\n"
        "Options:"},
    {HELP, 0, "h", "help", option::Arg::None, "  --help, -h  \tPrint usage and exit."},
    {DAEMON, 0, "d", "daemon", option::Arg::None, "  --daemon, -d  \tRun as daemon."},
    {0, 0, 0, 0, 0, 0}
};

int main(int argc, char** argv) {
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

    if (options[DAEMON]) {
        std::cout << "run as daemon" << std::endl;
    }

    for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next()) {
        std::cout << "Unknown option: " << std::string(opt->name, opt->namelen) << "\n";
    }

    for (int i = 0; i < parse.nonOptionsCount(); ++i) {
        std::cout << "Non-option #" << i << ": " << parse.nonOption(i) << "\n";
    }
    std::cout << GIT_VERSION << std::endl;
    return 0;
}
