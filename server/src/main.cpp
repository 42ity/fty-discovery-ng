/*  ====================================================================================================================
    Copyright (C) 2020 Eaton
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    ====================================================================================================================
*/

#include "config.h"
#include "daemon.h"
#include "discovery.h"
#include "jobs/impl/snmp.h"

#include <fty/command-line.h>
#include <fty_log.h>

int main(int argc, char** argv)
{
    std::string config = "conf/discovery.conf";
    bool        daemon = false;
    bool        help   = false;

    // clang-format off
    fty::CommandLine cmd("fty-discovery-ng", {
        {"--config", config, "Configuration file"},
        {"--daemon", daemon, "Daemonize this application"},
        {"--help",   help,   "Show this help"}
    });
    // clang-format on

    if (auto res = cmd.parse(argc, argv); !res) {
        std::cerr << res.error() << std::endl;
        std::cout << std::endl;
        std::cout << cmd.help() << std::endl;
        return EXIT_FAILURE;
    }

    if (help) {
        std::cout << cmd.help() << std::endl;
        return EXIT_SUCCESS;
    }

    fty::disco::Discovery discovery(config);
    if (!discovery.loadConfig()) {
        return EXIT_FAILURE;
    }

    fty::disco::impl::Snmp::instance().init(fty::disco::Config::instance().mibDatabase.value());

    ManageFtyLog::setInstanceFtylog(fty::disco::Config::instance().actorName.value(),
                                    FTY_COMMON_LOGGING_DEFAULT_CFG);

    if (daemon) {
        logDebug("Start discovery agent as daemon");
        fty::Daemon::daemonize();
    }

    if (auto res = discovery.init()) {
        discovery.run();
        discovery.shutdown();
    }
    else {
        logError("{}", res.error());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
