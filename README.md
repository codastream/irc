# IRC

## Sources
- [RFC 1459 of 1993 - original](https://datatracker.ietf.org/doc/html/rfc1459)
- [RFC 2813 of 2000 - update for Server protocol](https://datatracker.ietf.org/doc/html/rfc2813)
- [Beej's guide to network programming](https://beej.us/guide/bgnet/html/)
- [Chirc - Similar project from Chicago university](http://chi.cs.uchicago.edu/chirc/index.html)

# OSI model

| Level | Layer        | Protocols       | Description                                                                                                         |
|-------|--------------|-----------------|---------------------------------------------------------------------------------------------------------------------|
| 7     | application  | HTTP, SMTP, FTP | closest to end user                                                                                                 |
| 6     | presentation |                 | configures data into a format suitable for next layer by means of translation, encryption / decryption, compression |
| 5     | session      |                 | manages connections and sessions between devices                                                                    |
| 4     | transport    | UDP, TCP        | handles data transmission between devices                                                                           |
| 3     | network      | IP, ICMP        | determines most efficient path for routing between networks                                                         |
| 2     | data link    | Ethernet        | handles node to node data transfer within the same network. Divided into logical link control and MAC sublayers     |
| 1     | physical     |                 | converts data <-> bits across physical network                                                                      |


# sockets

- socket can be in _passive_ (listening) or _active_ (sending) mode

# ports

- network byte order is big-endian

```c++
// create a TCP socket with IPv4
int server_fd = socket(AF_INET, SOCK_STREAM, 0);

// configure address
sockaddr_in addr;
std::memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_port = htons(6667);
addr.sin_addr.s_addr = INADDR_ANY;

// bind socket with address/port and put it in passive mode
bind(server_fd, (sockaddr*)&addr, sizeof(addr));

```

# signal handling
https://en.cppreference.com/w/c/program/signal


## More info
https://man7.org/linux/man-pages/man3/sockaddr.3type.html

# Design patterns

## Singleton
- Rationale : ensure the same instance of server is returned

## Observer
- Rationale : notify observers (connected clients to a channel)

## Strategy
- Rationale : handle messages accordingly to their nature

## Command
- Rationale : encapsulate a request
- Cons : overhead for small commands

## State
- Rationale : tracking connection state : unauthenticated -> registered -> in channel -> disconnected


# Inter-process communication with `epoll()`

## Advantages

- complexity of O(1) over the number of watched fds vs O(n) for  `select()` and `poll()`
- no need to resubmit fd seets at each loop

## Usage

- uses a watch list

```c++
// create an instance
int epfd = epoll_create1(0);

// add, modify or remove monitored fds
// when we look for read available fds, we subscribe to EPOLLIN
epoll_event ev;
ev.events = EPOLLIN;
ev.data.fd = client_fd;
epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);

// wait for events
epoll_event events[64];
int n = epoll_wait(epfd, events, 64, -1);

// handle a connection
int client_fd = accept(server_fd, NULL, NULL);
set_non_blocking(client_fd);
ev.events = EPOLLIN | EPOLLET;
ev.data.fd = client_fd;
epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);

```

- level-triggered mode is the default mode, we get notified as long as data is ready
- _edge-triggered_ is more efficient but requires non-blocking sockets

# Testing

## Using netcat

```bash
nc localhost <port>
```

## Using telnet

```bash
telnet localhost 6667
```

## Debugging

```bash
lsof -i :6667
netstat -tnp | grep 6667
```

## Checking an existing server output

```bash
/raw DEBUG ON
/quote PROTOCTL 
/quote MONITOR + *
```

## Using an irc client

- once IRC protocol responses are implemented

irsssi
weechat
