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
#include <signal.h>
#include <string.h>
#include "defs.h"
#include "param.h"
#include "netdt.h"
#include "arp.h"

void usage(char *);
void sigh(int);

int main(int argc, char **argv)
{
	extern struct param param;
#ifdef DEBUG
	int i;
	for (i = 0; i < argc; i++)
		PDEBUG("Command line argv[%d] = %s\n", i, argv[i]);
#endif
	if (argc < 2) usage(argv[0]);

	signal(SIGQUIT, sigh);
	signal(SIGTERM, sigh);
	signal(SIGINT, sigh);

	if (get_opt(argc, argv) == -1) usage(argv[0]);
	//PDEBUG("%s", param.iface);
	if (strncmp(argv[1], "arp", 3) == 0) {
		if ((param.flags & 0xFFFFFFFF) == 0xFFFFFFFF) arp_usage(argv[0]);
		return (arp_init(argc, argv));
	} else usage(argv[0]);
	
	return (0);
}

void usage(char *pname)
{
	fprintf(stderr, "The Network Discovery Tool " PROGRAM_VERSION " by " PROGRAM_AUTHOR "\n"
			"Usage: %s <type> <options>\n"
			"Types:\n"
			" arp  Network discovery using ARP\n"
			"Options:\n"
			" -h   Help screen\n"
			, pname);
	exit(0);
}

void sigh(int type)
{
	exit(1);
}
