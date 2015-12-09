#ifndef CONFIG_H
#define CONFIG_H

class Config {
    public:
        Config(const std::string config_path) {
            m_reader = new INIReader(config_path.c_str());
        }

    private:
        std::string      m_ip;
        unsigned short   m_port;
        INIReader m_reader;
};

#endif
