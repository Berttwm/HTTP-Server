# HTTP Server 1.0/1.1 - Bertrand 

This is a HTTP 1.0/1.1 server which currently supports only GET requests and services a 1 page (index.html) local address.

## Compilation
Using the make file provided generate the client and server binaries.
```sh
make -B all
```
If you would like to compile the server or client files individually, use the following two commands respectively.
```sh
make -B client
make -B server
```
## Running
To test the functionality of the HTTP server, there is 2 methods to choose from.
### Running method 1 (recommended): using a browser
Firstly run the server binary by using
```sh
./server
```
Then open your browser and enter in "**127.0.0.1:12346**" into the address bar and click enter.
` Note that the server currently runs on a fixed config of 127.0.0.1:12346. This is done because port 80 (HTTP port) is often being used by other services in most environments` 

You should see this HTML page upon successful transaction:
![image](https://user-images.githubusercontent.com/44689249/143078809-7e86538a-f0e9-49e8-be63-985a24fe1469.png)

This indicates that the server has successfully retrieved the index.html file from the server resources, and correctly returns a HTTP response that is understandable to a web client.

### Running method 2: using client binary
Firstly, run the server binary using
```sh
./server
```
Secondly, run the client binary using
```sh
./client
```
You should observe that the server is still waiting on more incoming connections while the client should display the html text of the server resource **index.html** file.

## Features:
This section will cover a few notable achievements of the HTTP server.

### 1) HTTP Request and Response
The server is able to parse meaningful information from a standard HTTP request and store important information like *version*, *uri*, and *headers*. 

### 2) TCP Keep-Alive
The server can successfully infer keep-alive connections and time to keep connections alive. 
`An issue faced was allocating the TCP_KEEPIDLE field for keeping server alive for the stipulated seconds, hence this is left commented out in HTTPServer.cpp`
`I was however unable to simulate an ongoing connection as I faced several issues with the blocking/non-blocking recv call within the HTTP_servers handler`

### 3) ThreadPool
To meet the time and large concurrent request stated within the requirements, I generated a thread pool which maximizes the number of threads base on your system, instantiate them only at the start and allocated them to a work pool with a queue of jobs.

#### Sequencing Explained:
Each time a new HTTP request comes in, it is added to the job queue. One thread (that is currently idle and not working on other tasks) will be allocated this job from the queue assuming that there is at least one idle thread. The thread will service the client until either the client closes the connection or time-out from the keep-alive is met. The thread will then return to the idle state and is ready to serve more jobs in the job queue.

#### Design Decision
The thread pool will only be instantiated during initialization and terminated when the server is closed, this way there is no overhead of creation and deletion of threads. Additionally, the this allows us to reuse threads that has finished without needing to assign new threads for each incoming job, allowing the threads to potentially service infinite requests.

## 4) Extensibility
Most C/C++ implementation of HTTP servers available on the internet is extremely small scale and do not capitalize on the concepts of OOP. My server offers a very light-weight extensibility of modules and components and heavily utilizes OOP concepts to keep the code base minimal. For instance, creating and instantiating sockets only requires one line. Another example is that grouped services are placed into libraries (Sockets, Servers, Clients) for easier look-ups.

### HTTP Server and beyond 
With this architecture design, it should be easier to extend the already working methods of socket and basic server services to other kinds of servers beyond HTTP.

Thank you for perusing, <p>Have a good day!</p>
-- Bertrand Tan
