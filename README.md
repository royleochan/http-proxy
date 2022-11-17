# Http Proxy

Implements a simple HTTP web proxy that passes requests and data between a web client and a web server with basic socket programming interface.
Concurrent requests for a single connection is implemented with multithreading.

## Extensions

1. Proxy performs image substitution if the second argument is set to 1. For all the requested images, the requested image is substituted with http://ocna0.d2.comp.nus.edu.sg:50000/change.jpg.

2. Proxy runs in the attacker mode if the third argument is set to 1. For any http request, proxy replies with a HTML page with the string “You are being attacked”.

## Telemetry
After fetching objects from the web server and connection to the server is closed, the proxy will print out the following on a single line:
  - URL, size of object in bytes

For example, the telemetry output should be a string format like this example: 
  - http://www.comp.nus.edu.sg, 13433670

## Prerequisites
- Target environment: linux
- Browser: firefox
- Supported HTTP versions: HTTP 1.0 and HTTP 1.1
- C++ version 9.4.0

## Running proxy
```
mkdir build && cd build
cmake ..
make
./proxy <port> <image-flag> <attack-flag>
```

## Output
```
Proxy listening on port 3001
http://ocna0.d2.comp.nus.edu.sg:50000/tc1/, 517
http://ocna0.d2.comp.nus.edu.sg:50000/tc2/, 37139
http://ocna0.d2.comp.nus.edu.sg:50000/tc3/, 539638
```
