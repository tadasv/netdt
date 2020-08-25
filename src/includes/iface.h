/* 
 * The Network Discovery Tool
 * Copyright (C) 2005 Virtual Pointer
 * 
 * The Ultimate Network Tool is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *                       
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA. 
 */

#ifndef __NETDT_IFACE_H__
#define __NETDT_IFACE_H__ 1

/*
 * /usr/include/bits/ioctls.h
 */

/* Initialize global interface variables, call first */
void iface_init(int, char *);
int iface_get_index();
int iface_get_mtu();
int iface_get_metric();
short iface_get_flags();
void iface_get_ip(struct in_addr *);
void iface_get_netmask(struct in_addr *);
void iface_get_broadcast(struct in_addr *);
void iface_get_mac(struct ether_addr *);
		
#endif
