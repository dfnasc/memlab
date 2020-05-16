#include "process_memory.h"

#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <memory>

process_memory::process_memory(pid_t pid) 
 : m_regions() {
    m_fd = -1;
    m_pid = pid;
    
    load_regions();
    open_mem();
}

process_memory::~process_memory() {
}

bool process_memory::valid() const {
    return m_regions.size() > 0;
}

std::vector<unsigned long>& process_memory::search(const std::string& s) {

    char *buf = nullptr;

    m_results.clear();

    for (auto addrs : m_regions) {

        size_t length = addrs.second - addrs.first;
        buf = new char[length];
        lseek(m_fd, addrs.first, SEEK_SET);
        read(m_fd, buf, length);

        for (auto i = 0; i < length; i++) {
            const char *p = &buf[i];

            if (!strncmp(p, s.c_str(), s.length())) {
                m_results.push_back(addrs.first+i);
            }
        }

        delete buf;
    }

    return m_results;
}
    
std::vector<unsigned long>& process_memory::search(int8_t val) {
    return m_results;
}

std::vector<unsigned long>& process_memory::search(int16_t val) {
    return m_results;
}

std::vector<unsigned long>& process_memory::search(int32_t val) {
    char *buf = nullptr;

    m_results.clear();

    for (auto addrs : m_regions) {

        size_t length = addrs.second - addrs.first;
        buf = new char[length];
        lseek(m_fd, addrs.first, SEEK_SET);
        read(m_fd, buf, length);

        for (auto i = 0; i < length; i++) {
            const int32_t *p = (int32_t*)&buf[i];

            if (*p == val) {
                m_results.push_back(addrs.first + i);
            }
        }

        delete buf;
    }

    return m_results;
}

std::vector<unsigned long>& process_memory::search(int64_t val) {
    return m_results;
}

std::string process_memory::read_string(long address, size_t length) {
    std::unique_ptr<char> p(new char[length]);

    ::read(m_fd, p.get(), length);

    return std::string(p.get(), length);
}

int8_t process_memory::read8(long address) {
    int8_t value;

    lseek(m_fd, address, SEEK_SET);
    ::read(m_fd, &value, sizeof(value));

    return value;
}
    
int16_t process_memory::read16(long address) {
    int16_t value;

    lseek(m_fd, address, SEEK_SET);
    ::read(m_fd, &value, sizeof(value));

    return value;
}

int32_t process_memory::read32(long address) {
    int32_t value;

    lseek(m_fd, address, SEEK_SET);
    ::read(m_fd, &value, sizeof(value));

    return value;
}

int64_t process_memory::read64(long address) {
    int64_t value;

    lseek(m_fd, address, SEEK_SET);
    ::read(m_fd, &value, sizeof(value));

    return value;
}

void process_memory::write(unsigned long address, const std::string& value) {
    lseek(m_fd, address, SEEK_SET);
    ::write(m_fd, value.c_str(), value.length()+1);
}

void process_memory::write(long address, int8_t value) {
    lseek(m_fd, address, SEEK_SET);
    ::write(m_fd, &value, sizeof(value));
}

void process_memory::write(long address, int16_t value) {
    lseek(m_fd, address, SEEK_SET);
    ::write(m_fd, &value, sizeof(value));
}

void process_memory::write(long address, int32_t value) {
    lseek(m_fd, address, SEEK_SET);
    ::write(m_fd, &value, sizeof(value));
}

void process_memory::write(long address, int64_t value) {
    lseek(m_fd, address, SEEK_SET);
    ::write(m_fd, &value, sizeof(value));
}

void process_memory::load_regions() {
    
    std::string s;
    std::stringstream ss(s);
    
    ss << "/proc/" << m_pid << "/maps";;

    std::ifstream is(ss.str());

    if (!is.is_open()) {
        throw "invalid pid file";
    }

    while(!is.eof()) {

        std::string range, beg, end;
        std::pair<std::string, std::string> addresses;

        std::getline(is, s);

        if (s.length() >= 25) {
            long beg, end;
            std::string addrs = s.substr(0, 25);
            std::istringstream iss(addrs);
            iss >> std::hex >> beg >> end;
            add_region((unsigned long)beg, (unsigned long)-end);
        }
    }

    is.close();
}

void process_memory::add_region(long beg, long end) {
    m_regions.push_back(std::pair<long,long>(beg, end));
}

void process_memory::open_mem() {

    std::string s;
    std::stringstream ss(s);

    ss << "/proc/" << m_pid << "/mem";

    m_fd = open(ss.str().c_str(), O_RDWR);
}