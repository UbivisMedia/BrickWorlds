#pragma once

namespace BrickWorlds {

struct Version {
    static constexpr int MAJOR = 0;
    static constexpr int MINOR = 1;
    static constexpr int PATCH = 0;
    
    static const char* GetVersionString() {
        return "0.1.0";
    }
};

} // namespace BrickWorlds
