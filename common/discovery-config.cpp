#include "discovery-config.h"
#include <fstream>
#include <map>
#include <pack/serialization.h>

namespace fty::disco {

fty::Expected<void> ConfigDiscovery::save(const std::string& path)
{
    return pack::yaml::serializeFile(path, *this, pack::Option::WithDefaults);
}

fty::Expected<void> ConfigDiscovery::load(const std::string& path)
{
    return pack::yaml::deserializeFile(path, *this);
}

std::ostream& operator<<(std::ostream& ss, ConfigDiscovery::Protocol::Type value){
    using Type = ConfigDiscovery::Protocol::Type;

    ss << [&]() {
        switch (value) {
            case Type::Powercom:
                return "nut_powercom";
            case Type::XmlPdc:
                return "nut_xml_pdc";
            case Type::Snmp:
                return "nut_snmp";
            case Type::Unknown:
                return "unknown";
        }
        return "unknown";
    }();
    return ss;
}
std::istream& operator>>(std::istream& ss, ConfigDiscovery::Protocol::Type& value){
    using Type = ConfigDiscovery::Protocol::Type;

    std::string strval;
    ss >> strval;
    if (strval == "nut_powercom") {
        value = Type::Powercom;
    } else if (strval == "nut_xml_pdc") {
        value = Type::XmlPdc;
    } else if (strval == "nut_snmp") {
        value = Type::Snmp;
    } else {
        value = Type::Unknown;
    }
    return ss;
}

std::ostream& operator<<(std::ostream& ss, ConfigDiscovery::Discovery::Type value)
{
    using Type = ConfigDiscovery::Discovery::Type;

    ss << [&]() {
        switch (value) {
            case Type::Local:
                return "localscan";
            case Type::Ip:
                return "ipscan";
            case Type::Multi:
                return "multiscan";
            case Type::Full:
                return "fullscan";
            case Type::Unknown:
                return "unknown";
        }
        return "unknown";
    }();
    return ss;
}

std::istream& operator>>(std::istream& ss, ConfigDiscovery::Discovery::Type& value)
{
    using Type = ConfigDiscovery::Discovery::Type;

    std::string strval;
    ss >> strval;
    if (strval == "localscan") {
        value = Type::Local;
    } else if (strval == "ipscan") {
        value = Type::Ip;
    } else if (strval == "multiscan") {
        value = Type::Multi;
    } else if (strval == "fullscan") {
        value = Type::Full;
    } else {
        value = Type::Unknown;
    }
    return ss;
}



} // namespace fty::disco
