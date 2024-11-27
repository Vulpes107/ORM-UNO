#include <iostream>
#include <string.h>
#include "Client.hpp"

using namespace std;

int main() {
   Client client;

   if (!client.connectToServer()) {
      cerr << "connect failed" << endl;
      return 1;
   }
   
   const char *message = "cak cak rake rak tak";
   char buf[256];

   try {
      client.send(message, strlen(message)+1);
      client.receive(buf, 256);
      cout << buf << endl;
   } catch(const std::exception& e) {
      cerr << e.what() << endl;
   }
   
   return 0;
}