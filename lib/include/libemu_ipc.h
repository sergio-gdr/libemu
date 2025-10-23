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

#ifndef LIBEMU_IPC_H
#define LIBEMU_IPC_H

#include <stdint.h>

#define MAGIC_NUMBER 0x67726553

int ipc_init(bool is_server);
int ipc_recv(uint32_t **out_buf, bool wait);
int ipc_send(const uint32_t *buf, uint32_t size);
int ipc_wait_for_client();
int ipc_get_fd();

#endif
