# vms-network

`vms-network` is a small C++17 networking library built around POSIX sockets.
It currently provides thin wrappers for addresses, packets, base sockets, UDP sockets, and TCP sockets.

## License

This project is licensed under **GNU LGPL v2.1**.
See [LICENSE](LICENSE).

## Current Components

- `NetAddress`
  - IPv4 endpoint wrapper (`address + port`)
  - conversion to/from `sockaddr_in`
  - simple validity/error state
- `NetPacket`
  - byte buffer wrapper with explicit used-size tracking
  - safe write API with bounds/overflow checks
- `NetSocket`
  - base RAII wrapper for socket descriptor lifecycle
  - init/close/bind/non-blocking helpers
- `UdpSocket`
  - UDP send/receive over `NetPacket`
- `TcpSocket`
  - TCP connect/send/receive over `NetPacket`
  - `send` returns failure on partial write

## Build

### Shared (default)

```bash
cmake -S . -B build
cmake --build build
```

### Static

```bash
cmake -S . -B build-static -DVMS_NETWORK_BUILD_SHARED=OFF
cmake --build build-static
```

## Tests

Enable tests:

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Current test suite includes unit tests for:

- `NetPacket`
- `NetAddress`
- `NetSocket`
- `UdpSocket`
- `TcpSocket`
- build verification as both `SHARED` and `STATIC`

## Coverage

Enable instrumentation and generate report:

```bash
cmake -S . -B build -DBUILD_TESTING=ON -DVMS_NETWORK_ENABLE_COVERAGE=ON
cmake --build build --target coverage
```

Outputs:

- `build/coverage.info`
- `build/coverage-html/index.html`
