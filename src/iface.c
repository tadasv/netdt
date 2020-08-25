/* 
 * The Network Discovery Tool
 * Copyright (C) 2005 Tadas Vilkeliskis aka vptr (vpointer@gmail.com)
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

#include <stdio.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/ether.h>
#include "iface.h"
#include "defs.h"

struct ifreq iface;

struct interface
{
	int index,
	    mtu,
	    metric;
	short flags;
	struct in_addr ip, netmask, broadcast;
	struct ether_addr mac;
	
} interface;
	
void iface_init(int socket, char *name)
{
	PDEBUG("iface_init() called\n");
	memset(&iface, 0, sizeof(struct ifreq));
	strncpy(iface.ifr_name, name, strlen(name));
	
	if (ioctl(socket, SIOCGIFINDEX, &iface) == -1)
		interface.index = -1;
	else {
		interface.index = iface.ifr_ifindex;

		if (ioctl(socket, SIOCGIFADDR, &iface) != -1)
			//memcpy(&iface_ip, &(((struct sockaddr_in * )&iface.ifr_addr)->sin_addr), 4);
			interface.ip = ((struct sockaddr_in * )&iface.ifr_addr)->sin_addr;

		if (ioctl(socket, SIOCGIFNETMASK, &iface) != -1)
			//memcpy(&iface_nmask, &(((struct sockaddr_in * )&iface.ifr_netmask)->sin_addr), 4);
			interface.netmask = ((struct sockaddr_in * )&iface.ifr_netmask)->sin_addr;

		if (ioctl(socket, SIOCGIFBRDADDR, &iface) != -1)
			//memcpy(&iface_bcast, &(((struct sockaddr_in * )&iface.ifr_broadaddr)->sin_addr), 4);
			interface.broadcast = ((struct sockaddr_in * )&iface.ifr_broadaddr)->sin_addr;

		if (ioctl(socket, SIOCGIFHWADDR, &iface) != -1)
			memcpy((void*)interface.mac.ether_addr_octet, (void*)&iface.ifr_hwaddr.sa_data, 6);

		if (ioctl(socket, SIOCGIFMTU, &iface) != -1)
			interface.mtu = iface.ifr_mtu;
		
		if (ioctl(socket, SIOCGIFFLAGS, &iface) != -1)
			interface.flags = iface.ifr_flags;

		if (ioctl(socket, SIOCGIFMETRIC, &iface) != -1)
			interface.metric = iface.ifr_metric;
	}
	PDEBUG("iface_init() ended\n");
}

int iface_get_index()
{
	return (interface.index);
}

int iface_get_mtu()
{
	return (interface.mtu);
}

int iface_get_metric()
{
	return (interface.metric);
}

short iface_get_flags()
{
	return (interface.flags);
}

void iface_get_ip(struct in_addr *ip)
{
	(*ip) = interface.ip;
}

void iface_get_netmask(struct in_addr *netmask)
{
	(*netmask) = interface.netmask;
}

void iface_get_broadcast(struct in_addr *broadcast)
{
	(*broadcast) = interface.broadcast;
}

void iface_get_mac(struct ether_addr *mac)
{
	(*mac) = interface.mac;
}
