# P2P-Server
##Project Summary
This is a peer-to-peer file-sharing application. In this application, there is a p2pregistry - an intermediary program that handles the p2pservents which are both a client and a server. This application has three main operations: join, publish, and search. 
  * Join allows p2pservents to connect to the p2pregistry through a TCP socket and while it is connected the p2pservents will send “Hello” messages through a UDP socket every 60 seconds if not it is removed from the list of p2pservents. 
  * Publish will send the p2pservents list of files in its directory to the p2pregistry immediately after it is connected. 
  * Search will ask a p2pservent which file it would like to discover and if it is found in the published list of files it will return the first filename found and the port number of the p2pservent it was originally from.

##Usage Explanation
To use our application first use make on both the registry directory and servent directory. This will create executables in each directory. Note that if you want to run 2 clients, you need to put the servent files in two different folders. For the registry executable type “./server 8000” and for the servent executable type “./client 127.0.0.1 8000 (UDP_port_number).” Therefore, you can type in any UDP_port_number but two clients need to have two distinct UDP_port_number. For example, if the first client type “./client 127.0.0.1 8000 18000.” and the other should type “./client 127.0.0.1 8000 19000. From here the servent will ask the user if it would like to search for a file in the registry’s database, “collect.txt”.
