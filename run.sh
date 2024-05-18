#!/bin/bash

if [[ $1 == "server" ]]; then
  echo "running the server..."
  cd ./server
  ./build/main
elif [[ $1 == "client" ]]; then
  echo "running the client..."
  ./client/build/main
elif [[ $1 == "build" ]]; then
  echo "Compiling all files..."
  gcc client/main.c -o client/build/main
  gcc server/main.c -o server/build/main
else 
  echo "unknown parameters" $@
  exit 1
fi

