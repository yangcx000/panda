#include "config.h"

Config::Config(const char* config_path) {
    m_reader = new INIReader(config_path);
}

Config::~Config() {
}

int Config::ParseConfig() {
    if (m_reader->ParseError() < 0) {
        fprintf(stderr, "Error: open config file failed\n");
        return 1;
    }

    return 0;
}
