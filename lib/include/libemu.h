/*
 * Copyright (C) 2025 Sergio Gómez Del Real <sgdr>
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

#ifndef LIBEMU_H
#define LIBEMU_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "libemu_ipc.h"
#include "libemu_list.h"
#include "libemu_cpu.h"
#include "libemu_ppu.h"

// type of command
// used for request and reply.
enum msg_type {
    TYPE_MONITOR,
    TYPE_CONTROL_FLOW,
    TYPE_INSPECT
};

// subtypes for the TYPE_MONITOR type
enum msg_type_monitor {
    MONITOR_STOP,
    MONITOR_RESUME
};

// subtypes for the TYPE_INSPECT type
enum msg_type_inspect {
    INSPECT_PRINT_ADDR,
    INSPECT_GET_CPU_REG,
    INSPECT_GET_PPU_REG,
    INSPECT_GET_INSTR_AT_ADDR,
    INSPECT_GET_OP_LEN
};

// subtypes for the TYPE_CONTROL_FLOW type
enum msg_type_control_flow {
    CONTROL_FLOW_NEXT,
    CONTROL_FLOW_CONTINUE,
    CONTROL_FLOW_BREAK,
    CONTROL_FLOW_DELETE,
    CONTROL_FLOW_UNTIL,
    CONTROL_FLOW_FINISH
};

// header size in bytes
#define HDR_SIZE 16
struct msg {
	// header:
	//  type, subtype and size of msg
	struct msg_hdr {
		enum msg_type type;
		union msg_subtype {
			enum msg_type_control_flow control_flow;
			enum msg_type_inspect inspect;
			enum msg_type_monitor monitor;
		} subtype;
		size_t size; // size of the payload, in bytes
	} hdr;
	void *payload;
};

int emu_send_msg(const struct msg *msg);
int emu_recv_msg(struct msg *msg, bool wait);
int emu_init(bool is_server);
int emu_wait_for_client();
int emu_get_fd();

#endif
