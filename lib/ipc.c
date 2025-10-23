/*
 * Copyright (C) 2025 Sergio Gómez Del Real
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <poll.h>

#include "libemu_ipc.h"

int socket_fd;

int ipc_get_fd() {
	return socket_fd;
}

int ipc_send(const uint32_t *buf, uint32_t size) {
	int ret = send(socket_fd, buf, size, 0);
	if (ret == -1)
		perror("send()");
	return ret;
}

// this function allocates space for the reply payload.
// caller is responsible for freeing it.
int ipc_recv(uint32_t **out_buf, bool wait) {
	int ret;
	uint32_t magic;

	int flags = fcntl(socket_fd, F_GETFL, 0);
	if (wait)
		flags &= ~O_NONBLOCK;
	else
		flags |= O_NONBLOCK;
	if (fcntl(socket_fd, F_SETFL, flags) == -1) {
		perror("fcntl()");
	}

	ret = recv(socket_fd, &magic, sizeof(uint32_t), 0);
	if (ret == -1) {
		goto out;
	}
	assert(magic == MAGIC_NUMBER);

	// get type, subtype and size
	uint32_t hdr[3];
	ret = recv(socket_fd, hdr, 3*sizeof(uint32_t), 0);
	if (ret == -1) {
		perror("recv()");
		return -1;
	}
	uint32_t payload_size = hdr[2];
	*out_buf = malloc(sizeof(hdr) + payload_size);

	memcpy(*out_buf, hdr, sizeof(hdr));
	if (payload_size) {
		ret = recv(socket_fd, (*out_buf)+sizeof(hdr)/sizeof(uint32_t), payload_size, 0);
	}
out:
	return ret;
}

int ipc_wait_for_client() {
	struct pollfd client_poll = { .fd = socket_fd, .events = POLLIN };
	int ret;
	while (1) {
		if ((ret = poll(&client_poll, 1, -1)) == -1) {
			perror("poll()");
			continue;
		}
		ret = accept(socket_fd, NULL, NULL);
		if (ret == -1) {
			perror("accept()");
			goto out;
		}
		socket_fd = ret;
		break;
	}

out:
	return ret;
}

int ipc_init(bool is_server) {
	socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (socket_fd == -1) {
		perror("socket()");
		goto err1;
	}

	// socket path
	char *dir = getenv("XDG_RUNTIME_DIR");
	if (!dir) {
		dir = "/tmp";
	}
	struct sockaddr_un ipc_sockaddr = {};
	ipc_sockaddr.sun_family = AF_UNIX;
	snprintf(ipc_sockaddr.sun_path, sizeof(ipc_sockaddr.sun_path), "%s/libemu-.sock", dir);
	//snprintf(ipc_sockaddr.sun_path, sizeof(ipc_sockaddr.sun_path), "%s/libemu-%i.sock", dir, getpid());

	if (is_server) {
		unlink(ipc_sockaddr.sun_path);
		if (bind(socket_fd, (struct sockaddr *)&ipc_sockaddr, sizeof(ipc_sockaddr)) == -1) {
			perror("bind()");
			goto err2;
		}

		if (listen(socket_fd, 3) == -1) {
			perror("listen()");
			goto err2;
		}
	}
	else {
		if (connect(socket_fd, (struct sockaddr *)&ipc_sockaddr, sizeof(ipc_sockaddr)) == -1) {
			perror("connect()");
			goto err2;
		}
	}

	return 0;

err2:
	close(socket_fd);
err1:
	return -1;
}
