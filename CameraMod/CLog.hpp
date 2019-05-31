#ifndef C_LOG_HPP
#define C_LOG_HPP
#include <fstream>

const std::string LOG_FILENAME = "cameramodlog.txt";

class CLog {
private:
    std::ofstream m_outputStream;
    CLog() { m_outputStream.open(LOG_FILENAME); }

public:
    static CLog* getLogger()
    {
        static CLog* logger = NULL;
        if (logger == NULL) {
            logger = new CLog();
        }
        return logger;
    }

    static std::ofstream& getStream() { return getLogger()->m_outputStream; }
};

#endif
