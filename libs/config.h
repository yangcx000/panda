#ifndef CONFIG_H
#define CONFIG_H
#include "INIReader.h"

class Config {
    public:
        Config(const char* config_path);
        ~Config();

        int ParseConfig();
        std::string ip() {
            return m_ip;
        }

        unsigned short port() {
            return m_port;
        }

    private:
        std::string      m_ip;
        unsigned short   m_port;
        INIReader* m_reader;
};

#endif
