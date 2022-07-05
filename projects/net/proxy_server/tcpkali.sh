#!/bin/bash
tcpkali --connections 30 --connect-rate=30 -r50 127.0.0.83:92 -em 'GET / HTTP/1.1\r\nHost:127.0.0.83:92\r\n\r\n' --duration=10

