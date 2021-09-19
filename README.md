# PsyQuant Client/Server Sample App

### Project Structure
```
📦project
 ┣ build
 ┣ Client
 ┃ ┗ ClientMain.cpp
 ┣ common
 ┃ ┗ message.h
 ┣ Server
 ┃ ┣ include
 ┃ ┃ ┣ MessageReader.h
 ┃ ┃ ┗ SafeQueue.h
 ┃ ┗ src
 ┃   ┣ MessageReader.cpp
 ┃   ┗ SafeQueue.cpp
 ┣ Socket
 ┃ ┣ include
 ┃ ┃ ┣ Client.h
 ┃ ┃ ┣ Server.h
 ┃ ┃ ┗ Socket.h
 ┃ ┗ src
 ┃   ┣ Client.cpp
 ┃   ┣ Server.cpp
 ┃   ┗ Socket.cpp
 ┗ Test
   ┣ FileGenerator.cpp
   ┗ FIleReader.cpp

```

## Dependencies and Versions

1. CMake - 3.7 (Tested on 3.21.2)
2. C++ - 14
3. gcc - 5 (Tested on gcc 11.1.0)
4. ldd (GNU libc) 2.33

## Build Instructions

```
mkdir build
cd build
cmake ..
cmake --build .
```

## Running and testing

After a successful build all binaries are created within the `build` directory.

1. Create sample input file
```
cd (project_root)/build/Test
./FileGenerator <number_of_ticker_to_generate>
```
2. Run the server - Server binary can be found in (project_root)/build/Server
```
./Server 127.0.0.1 8080 < ../Test/md_in.txt
```
3. Run client(s) - Client binary can be found in (project_root)/build/Client
```
./Client 127.0.0.1 8080
```

## What's working
1. Multiple Client handling
2. Reading from stdin
3. Sending to all connected clients
4. Handle Client disconnect

## TODO
1. Heartbeat functionality
2. Use lockless ring buffer instead of Queue

## References/Credits

1. For skeleton Client/Sever code -> https://codereview.stackexchange.com/questions/125273/c-version-of-a-client-server-application - 
2. Thread Safe Queue - https://stackoverflow.com/questions/15278343/c11-thread-safe-queue
3. Helps looking at compiler output -> https://cppinsights.io/
4. Heartbeat server idea -> https://gist.github.com/arrieta/df4e6e354cc9f133df9f0433f9de3e14