#include <fstream>
#include <string>
#include <iostream>
#include <netdb.h>     // For getaddrinfo
#include <unistd.h>    // For close() on Unix-like systems
#include <cstring>     // For memset
#include <arpa/inet.h> // For inet_ntop()

using namespace std;

int main()
{
    // Setting if stream to open file
    std::ifstream file("dnsmap.txt");

    // Checking if file exists
    if (!file)
    {
        cout << "Error opening file!" << endl;
        return 1;
    }

    // Creating string subdomain
    string subdomain;

    // While loop to read line of file and attach it to subdomain
    while (getline(file, subdomain))
    {
        // Prints out subdomains in file and attachs to the host
        cout << "Subdomain: " << subdomain << endl;
        string fullDomain = subdomain + ".adelaide.edu.au";

        // Using hint to specify how we want DNS to lookup and
        // res will store results
        struct addrinfo hints, *res;

        // Clearing hints to remove left over memory
        memset(&hints, 0, sizeof(hints));
        // Telling functions to look for IPv4 addr
        hints.ai_family = AF_INET;   
        // Telling function getaddrinfo() that we 
        // want to use TCP   
        hints.ai_socktype = SOCK_STREAM; // TCP

        // Perfoming DNS lookup
        int status = getaddrinfo(fullDomain.c_str(), NULL, &hints, &res);

        // If-else statement, 0 is success
        if (status == 0)
        {
            // If successful, print the resolved IP addr
            // Putting the result address to IPv4 type
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
            // Buffer created to store the converted IP address
            char ipStr[INET_ADDRSTRLEN];
            // Convert the binary IP address to a readable string
            inet_ntop(AF_INET, &(ipv4->sin_addr), ipStr, INET_ADDRSTRLEN);

            cout << "Resolved IP: " << ipStr << endl;
        }
        else
        {
            cout << "Failed to resolve: " << fullDomain << endl;
        }
        // Free memory
        freeaddrinfo(res);
    }
      // close if not needed
      file.close();
    return 0;
}