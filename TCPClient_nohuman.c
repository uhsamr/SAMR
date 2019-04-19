#include <stdio.h>//compile: gcc TCPClient_windows.c -o TCPClient_windows.exe -lws2_32
#include <winsock.h> //Warning: This program does not perform any error handling.

int main(int argc, char *argv[])  
{             
  struct sockaddr_in address;
  WSADATA wsaData; //https://docs.microsoft.com/en-us/windows/desktop/api/winsock/ns-winsock-wsadata    
  char  data;

  WSAStartup(MAKEWORD(2, 0), &wsaData); //Initialize WinSock
  int client = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //Allocate TCP Socket

  memset(&address, 0, sizeof(address)); //clear socket address
  address.sin_family      = AF_INET;  //IPv4 address, sin_family is a short
  //address.sin_addr.s_addr = inet_addr("127.0.0.1");//set to (loopback) IP address
  address.sin_addr.s_addr = inet_addr("192.168.1.123"); //
  address.sin_port        = htons(9999); //make port in network byte order, sin_port is an u_short

  connect(client, (struct sockaddr *)&address, sizeof(address)); //(*@\clientBox{1+2)}@*)

  data = 2;
  send(client, &data, 1, 0);  //(*@\clientBox{3)}@*) send 1 byte of data to client, flags=0

  closesocket(client);  //(*@\clientBox{4)}@*)
  WSACleanup();  //Finalize WinSock
  return 0;
  }