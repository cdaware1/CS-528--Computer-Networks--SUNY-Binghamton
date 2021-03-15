Compilation and Execution:
- I have used C++ to implement my client program.
- When I compile my client and server code using make, it will generate two executables.
- My client program takes an argument in which user should include the server IP address.
- Hence, the user should run “./server” first in one terminal.
- Then user can check the server IP by using command ip addr show (Depends on the operating system you are using).
- Then in another terminal user should run as “./client 192.168.222.128” where 192.168.222.128 is server IP.
- If we enter IP address other than the server, it will result into packet loss.
- Hence, client will send the ping message to server and if it gets return message it will display Ping success and RTT otherwise it will display packet lost.
