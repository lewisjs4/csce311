# csce311

## ipc/socket_cli_srv.cc
- File can be made with **make socket-cli-srv** from the containing directory
- Run server in terminal with **./socket-cli-srv server** and it will block waiting for client connect
- Run client in separate terminal with **./socket-cli-srv client** and it will block reading STDIN.
  - Text entered into the client's terminal will be transmitted to the server and printed there along with data transmission info
