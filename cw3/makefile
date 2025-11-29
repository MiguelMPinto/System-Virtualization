CC = cc
CFLAGS = -std=c11 -Wall -Wextra -O2
LDFLAGS = -lsystemd -lpthread

CLIENT_SRC = tvschat/manager/client/src/tvschat.c
CLIENT_BIN = tvschat/manager/client/bin/tvschat

DAEMON_SRCS = tvschat/manager/daemon/src/tvschatd.c tvschat/manager/daemon/src/request_parser.c
DAEMON_BIN = tvschat/manager/daemon/bin/tvschatd

.PHONY: all clean install

all: $(CLIENT_BIN) $(DAEMON_BIN)

$(CLIENT_BIN): $(CLIENT_SRC)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $<

$(DAEMON_BIN): $(DAEMON_SRCS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $(DAEMON_SRCS) $(LDFLAGS)

clean:
	-rm -f $(CLIENT_BIN) $(DAEMON_BIN)

install: all
	@echo "Running install script..."
	./cw3/install-all.sh