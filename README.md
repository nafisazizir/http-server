# HTTP Server

Multithreaded simple HTTP server using `kqueue` for event notification, designed to handle a large number of concurrent connections. The server is optimized for **low latency** and **high throughput**, suitable for apps that require fast and efficient network communication.

## Design Choices

- Main thread for the main program
- One listener thread
- Five worker threads to handle clients
- Async I/O using `kqueue`. It was chosen to comply with macOS as the development platform. for Linux, it can use `epoll`.

## Limitations

This project is a simple HTTP server that only focuses on getting my hands dirty with networking, multithreading, and system performance. There are a lot of assumptions and limitations in this project. The server will only have a static response and only accept GET request. HTTP parser is not implemented yet - maybe in the future!

## Benchmark

Before optimizing the server into a multi-threaded, I implemented a simple single-threaded server and benchmarked the performance. Benchmarking done in Macbook M1 2020, each test ran 1 minute with 10 clients thread for 500 and 10000 concurrent connections.

### Before Optimization (Single-threaded)

```
Running 1m test @ http://127.0.0.1:8080/
  10 threads and 500 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     6.60ms    1.56ms  15.70ms   85.70%
    Req/Sec   835.06    848.39     2.57k    79.57%
  29771 requests in 1.00m, 2.21MB read
  Socket errors: connect 257, read 33625, write 228, timeout 0
Requests/sec:    495.85
Transfer/sec:     37.77KB

Running 1m test @ http://127.0.0.1:8080/
  10 threads and 10000 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     6.25ms    1.60ms  24.58ms   87.24%
    Req/Sec     1.00k     1.47k    6.98k    87.41%
  30237 requests in 1.00m, 2.25MB read
  Socket errors: connect 9757, read 33641, write 161, timeout 0
Requests/sec:    503.71
Transfer/sec:     38.37KB
```

### After Optimization (Multithreaded)

```
Running 1m test @ http://127.0.0.1:8080/
  10 threads and 500 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     2.85ms   10.40ms 202.79ms   96.32%
    Req/Sec    27.74k    15.74k   82.60k    52.71%
  13242584 requests in 1.00m, 0.94GB read
  Socket errors: connect 257, read 105, write 0, timeout 0
Requests/sec: 220337.02
Transfer/sec:     15.97MB

Running 1m test @ http://127.0.0.1:8080/
  10 threads and 10000 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     1.12ms    3.64ms 204.09ms   97.22%
    Req/Sec    43.75k    29.32k   90.19k    40.87%
  13061484 requests in 1.00m, 0.92GB read
  Socket errors: connect 9757, read 97, write 0, timeout 0
Requests/sec: 217588.18
Transfer/sec:     15.77MB
```
