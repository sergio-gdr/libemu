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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libemu.h"

enum msg_field {
	MAGIC,
	TYPE,
	SUBTYPE,
	SIZE,
	PAYLOAD
};

// create a raw buffer from the structured msg.
int emu_send_msg(const struct msg *msg) {
	uint32_t *buf = malloc(msg->hdr.size + HDR_SIZE);
	buf[MAGIC] = MAGIC_NUMBER;
	buf[TYPE] = msg->hdr.type;
	switch (msg->hdr.type) {
		case TYPE_MONITOR:
			buf[SUBTYPE] = msg->hdr.subtype.monitor;
			break;
		case TYPE_CONTROL_FLOW:
			buf[SUBTYPE] = msg->hdr.subtype.control_flow;
			break;
		case TYPE_INSPECT:
			buf[SUBTYPE] = msg->hdr.subtype.inspect;
			break;
	}
	buf[SIZE] = msg->hdr.size;
	if (msg->hdr.size) {
		memcpy(buf+PAYLOAD, (void *)msg->payload, msg->hdr.size);
	}

	int ret = ipc_send(buf, msg->hdr.size + HDR_SIZE);
	free(buf);
	return ret;
}

int emu_recv_msg(struct msg *msg, bool wait) {
	uint32_t *raw_req = nullptr;
	int ret;
	if ((ret = ipc_recv(&raw_req, wait)) == -1)
		goto out;

	// msg type
	msg->hdr.type = raw_req[0];

	// msg subtype
	if (msg->hdr.type == TYPE_CONTROL_FLOW)
		msg->hdr.subtype.control_flow = raw_req[1];
	else if (msg->hdr.type == TYPE_INSPECT)
		msg->hdr.subtype.inspect = raw_req[1];
	else if (msg->hdr.type == TYPE_MONITOR)
		msg->hdr.subtype.monitor = raw_req[1];
	else {
		fprintf(stderr, "SUBTYPE");
		ret = -1;
		goto out;
	}

	// msg size
	msg->hdr.size = raw_req[2];

	if (msg->hdr.size) {
		msg->payload = malloc(msg->hdr.size);
		memcpy(msg->payload, raw_req+3, msg->hdr.size);
	}
out:
	free(raw_req);
	return ret;
}

int emu_wait_for_client() {
	return ipc_wait_for_client();
}

int emu_get_fd() {
	return ipc_get_fd();
}

int emu_init(bool is_server) {
	return ipc_init(is_server);
}
