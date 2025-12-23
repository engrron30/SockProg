SERVER_BIN = server
CLIENT_BIN = client

.PHONY: all clean
all:
	gcc server.c -o $(SERVER_BIN)
	gcc client.c -o $(CLIENT_BIN)

clean:
	rm -rf $(SERVER_BIN) $(CLIENT_BIN)
