/* 
 * The Network Discovery Tool.
 * Copyright (C) 2005 Tadas Vilkeliskis aka vptr (vpointer@gmail.com)
 *
 * The Network Discovery Tool is free software; you can redistribute it and/or
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

#include <netinet/in.h>

struct network
{
	unsigned long start, end;
} network;

void net_calc_network(struct in_addr ip, struct in_addr netmask)
{
	network.start = ip.s_addr & netmask.s_addr; /* sitas isvis niekam nereikalingas, reikia sugalvoti kur i kisti */
	network.end = ip.s_addr | (~netmask.s_addr);
}

in_addr_t net_next_ip(struct in_addr curr_ip)
{
	unsigned long curr;
	curr = ntohl(curr_ip.s_addr);
	if (curr + 1 <= ntohl(network.end)) {
		curr++;
		return (htonl(curr));
	} else return (0);
}
