#include <Vextr/utils/Debug.hpp>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace vextr::utils {

static std::ofstream& getLogFile() {
    static std::ofstream logFile("vextr_debug.log", std::ios::app);
    return logFile;
}

static std::string timeNow() {
    using namespace std::chrono;
    auto now = system_clock::now();
    std::time_t t = system_clock::to_time_t(now);
    std::tm tm;
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void debugLog(const std::string& msg) {
    auto& logFile = getLogFile();
    logFile << "[" << timeNow() << "] - " << msg << "\n";
    logFile.flush();
}

void debugLog(const std::string& msg, const void* source) {
    auto& logFile = getLogFile();
    std::ostringstream src;
    src << source;
    logFile << "[" << timeNow() << "] - [" << src.str() << "] " << msg << "\n";
    logFile.flush();
}

} // vextr::utils
