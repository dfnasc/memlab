#include <iostream>
#include <string>

#include "unistd.h"

int main(int argc, const char *argv[]) {

   std::string s = "This is my text";
   int i = 999;
   pid_t pid = getpid();


   while (true) {
      system("clear");
      std::cout << "---[ Memory Content ]---\n\n";
      std::cout << "PID: " << pid << std::endl
                << "Int: " << i << std::endl
                << "Str: " << s << std::endl << std::endl;

      sleep(5);
      i--;
   }
}
