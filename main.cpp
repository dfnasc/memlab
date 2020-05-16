#include <iostream>
#include <vector>

#include "process_memory.h"

int main(int argc, const char *argv[]) {

   process_memory mem(atol(argv[1]));

   std::vector<unsigned long> founds = mem.search("Casual");
   std::vector<unsigned long>::const_iterator it;

   for (it = founds.begin(); it != founds.end(); ++it) {
      std::cout << "found at: " << std::hex << *it << std::endl;
      //mem.write(*it, (int32_t)999);
      mem.write(*it, "Diego ");
   }

   return 0;
}
