#include <iostream>
#include <string.h>
#include <Client.hpp>

using namespace std;

int main() {
   Client client;
   string user_name;
   
   if (!client.connectToServer()) {
      cerr << "Connect failed" << endl;
      return 1;
   }

   cout << "Input username: " << endl;
   cin >> user_name;

   char buf[256];
   try {
      client.send(user_name.c_str(), user_name.size());
      client.receive(buf, 256);
      cout << buf << endl;   
   } catch(const std::exception& e) {
      cerr << e.what() << endl;
   }
   
   return 0;
}