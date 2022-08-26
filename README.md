# P2P-Server
## Project Summary
This is a peer-to-peer file-sharing application. In this application, there is a p2pregistry - an intermediary program that handles the p2pservents which are both a client and a server. This application has three main operations: join, publish, and search. 
  * Join allows p2pservents to connect to the p2pregistry through a TCP socket and while it is connected the p2pservents will send “Hello” messages through a UDP socket every 60 seconds if not it is removed from the list of p2pservents. 
  * Publish will send the p2pservents list of files in its directory to the p2pregistry immediately after it is connected. 
  * Search will ask a p2pservent which file it would like to discover and if it is found in the published list of files it will return the first filename found and the port number of the p2pservent it was originally from.

## Usage Explanation
  * Use _make_ on both the registry abd servent directories. Note: if you want to run 2 clients, you need to put the servent files in two different folders
  ```sh
   $ make
   ```
  * For the registry executable
  ```sh
   $ ./server 8000
   ```
  * For the servent executable. Note: two clients need to have two distinct UDP_port_number
  ```sh
   $ ./client 127.0.0.1 8000 (UDP_port_number)
   ```
