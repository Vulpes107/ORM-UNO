#include <iostream>
#include <string.h>
#include "Client.hpp"

int main() {
   using namespace std;
   Client client;

   int connect_error = client.connectToServer();
   if (connect_error == -1) {
      printf("connect failed\n");
      perror("ERROR");
      return 1;
   }
   const char *message = "cak cak rake rak tak";
   char buf[256];
   client.send(message, strlen(message));
   client.receive(buf, 256);
   cout<<buf<<endl;
   
   
   
   return 0;
}