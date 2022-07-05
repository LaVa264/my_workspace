#!/bin/bash
for n in {1..100};
do 
    curl 127.0.0.83:92 127.0.0.83:92 127.0.0.83:92
    curl 127.0.0.83:92 -X POST
    echo $n
done