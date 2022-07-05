#!/bin/bash
tcpkali --connections 400 --connect-rate=400 -r200 127.0.0.83:92 -em 'GET / HTTP/1.1\r\nHost:127.0.0.83:92\r\n\r\n' --duration=10

#tcpkali --connections 10 --connect-rate=5 -r5 127.0.0.83:92 -em 'GET / HTTP/1.1\r\nHost:127.0.0.83:92\r\n\r\n' --duration=10

