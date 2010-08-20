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
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ether.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netpacket/packet.h>

#include <unistd.h>
#include <fcntl.h>

#include "netdt.h"
#include "defs.h"
#include "param.h"
#include "iface.h"
#include "network.h"

void arp_generate_packet(struct in_addr, struct in_addr, struct ether_addr, u_int8_t *);

int arp_init(int argc, char **argv)
{
	PDEBUG("arp_init called\n");
	
	extern struct param param;
	struct in_addr sip, tip, netmask, tmp;
	int sock_fd;
	struct sockaddr_ll saddr_ll;
	socklen_t sa_size = sizeof(struct sockaddr_ll);
	struct ether_addr mac;
	struct ether_arp *arp_header;
	u_int8_t packet[sizeof(struct ether_header) + sizeof(struct ether_arp)];
	//printf("%d\n", sizeof(packet)); exit(0);
	
	/* ETH_ALEN, ARPHDR_ETHER, ETHERTYPE_ARP */

	if ( (sock_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) == -1) {
		PDEBUG("ERROR: socket() failed\n");
		fprintf(stderr, "Error: can't create socket\n");
		exit(1);
	}
	if (fcntl(sock_fd, F_SETFL, O_NONBLOCK) == -1) {
		PDEBUG("ERROR: fcntl() failed\n");
		fprintf(stderr, "Error: can't make socket non-blocking\n");
		close(sock_fd);
		exit(1);
	}
	
	iface_init(sock_fd, param.iface);

	if ( (saddr_ll.sll_ifindex = iface_get_index()) == -1) {
		fprintf(stderr, "Error: no such device %s\n", param.iface);
		close(sock_fd);
		exit(1);
	}

	/*
	XXX
	po situ veiksmu reiketu atlikti 
	patikrinima ar yra teisingas IP adresas
	*/
	iface_get_ip(&sip);
	iface_get_netmask(&netmask);
	iface_get_mac(&mac);
	
	saddr_ll.sll_family = PF_PACKET;
	saddr_ll.sll_halen = ETH_ALEN;
	memset(&saddr_ll.sll_addr, 0, ETH_ALEN);
	
	net_calc_network(sip, netmask);
	tip.s_addr = sip.s_addr & netmask.s_addr;
	arp_header = (struct ether_arp*)(packet + sizeof(struct ether_header));
	
	fprintf(stdout, "Discovering network with %s/%s\n\n", inet_ntoa(sip), ether_ntoa(&mac));
	
	while ( (tip.s_addr = net_next_ip(tip)) != 0) {
		//printf("%s\n", inet_ntoa(tip));
		memset(packet, 0, sizeof(packet));
		arp_generate_packet(sip, tip, mac, packet);
		sendto(sock_fd, &packet, 42, 0, (struct sockaddr*)&saddr_ll, sizeof(struct sockaddr_ll));
		usleep(param.packet_delay * 100);
		if (recvfrom(sock_fd, &packet, 42, 0, (struct sockaddr*)&saddr_ll, &sa_size) == 42) {
			if (arp_header->arp_op == htons(ARPOP_REPLY)) {
				memcpy((void*)&tmp, (void*)&arp_header->arp_spa, 4);
				fprintf(stdout, "%-15s %s\n", inet_ntoa(tmp),
						ether_ntoa((struct ether_addr*)&arp_header->arp_sha));
			}
		}
	}
	return (0);
}

void arp_generate_packet(struct in_addr sip, struct in_addr tip, struct ether_addr smac, u_int8_t *packet)
{
	struct ether_header *eth_header;
	struct ether_arp *arp_header;
	
	eth_header = (struct ether_header*)packet;
	memset((void*)&eth_header->ether_dhost, 0xff, ETH_ALEN);	/* broadcast */
	memcpy((void*)&eth_header->ether_shost, (void*)&smac.ether_addr_octet, ETH_ALEN);
	eth_header->ether_type = htons(ETHERTYPE_ARP);
	
	arp_header = (struct ether_arp*)(packet + sizeof(struct ether_header));
	arp_header->arp_hrd = htons(ARPHRD_ETHER);
	arp_header->arp_pro = htons(ETHERTYPE_IP);
	arp_header->arp_hln = ETH_ALEN;
	arp_header->arp_pln = 4; /* reikia susiziureti ar nera kokios konstantos */
	arp_header->arp_op = htons(ARPOP_REQUEST);
	memcpy((void*)arp_header->arp_sha, (void*)&smac.ether_addr_octet, ETH_ALEN);
	memcpy((void*)arp_header->arp_spa, (void*)&sip.s_addr, 4);
	memcpy((void*)arp_header->arp_tpa, (void*)&tip.s_addr, 4);
}

void arp_usage(char *pname)
{
	fprintf(stderr, "The Network Discovery Tool " PROGRAM_VERSION " by " PROGRAM_AUTHOR "\n"
			"Usage: %s arp <options>\n"
					"Options:\n"
					" -i <if>   Network interface\n"
					" -t <time> Delay between packets\n"
					" -h        Help screen\n"
					, pname);
	exit(0);
}
