#ifndef PROC_MEM_H
#define PROC_MEM_H

#include "sys/types.h"

#include <unistd.h>
#include <vector>
#include <string>

class process_memory {
public:
    process_memory(pid_t pid);
    ~process_memory();

    bool valid() const;

    std::vector<unsigned long>& search(const std::string& s);
    std::vector<unsigned long>& search(int8_t val);
    std::vector<unsigned long>& search(int16_t val);
    std::vector<unsigned long>& search(int32_t val);
    std::vector<unsigned long>& search(int64_t val);

    std::string read_string(long address, size_t length);
    int8_t read8(long address);
    int16_t read16(long address);
    int32_t read32(long address);
    int64_t read64(long address);

    void write(unsigned long address, const std::string& value);
    void write(long address, int8_t value);
    void write(long address, int16_t value);
    void write(long address, int32_t value);
    void write(long address, int64_t value);

private:
    void load_regions();
    void open_mem();
    void add_region(long beg, long end);
private:
    int m_fd;
    pid_t m_pid;
    std::vector<std::pair<unsigned long,unsigned long>> m_regions;
    std::vector<unsigned long> m_results;
};

#endif // PROC_MEM_H