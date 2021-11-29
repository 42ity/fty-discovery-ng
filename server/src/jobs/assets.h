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

#pragma once
#include "discovery-task.h"
#include <set>

// =====================================================================================================================

namespace fty::job {

struct DeviceInfo {
    int indexDevice;
    std::string& deviceSerial;
    std::string type;
};

/// Discover Assets from enpoint
/// Returns @ref commands::assets::Out (something like a list of Assets)
class Assets : public Task<Assets, commands::assets::In, commands::assets::Out>
{
public:
    using Task::Task;

    // Get assets list
    Expected<void> getAssets(const commands::assets::In& in, commands::assets::Out& out);

    // Add asset sensors
    void addSensors(const DeviceInfo& deviceInfo, int& indexSensor, pack::ObjectList<fty::commands::assets::Asset>& sensors, const std::map<std::string, std::string>& dump);

    /// Runs discover job.
    void run(const commands::assets::In& in, commands::assets::Out& out);
private:
    void parse(const std::string& cnt, commands::assets::Out& out);
    void addAssetVal(commands::assets::Asset& asset, const std::string& key, const std::string& val, bool readOnly = true);
    fty::Expected<std::string> getAssetVal(const commands::assets::Asset& asset, const std::string& key) const;
    void enrichAsset(commands::assets::Return& asset);

private:
    commands::assets::In m_params;
    std::set<std::string> m_discoveredSerialSet;
};

} // namespace fty::job

// =====================================================================================================================
