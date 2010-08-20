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

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#define _GNU_SOURCE
#include <getopt.h>
#include "param.h"
 
struct param param;

int get_opt(int argc, char **argv)
{
	extern char *optarg;
	extern int optind, opterr, optopt;
	int c;
	
	optind = 2;
	
	param.packet_delay = 90;
	
	while ( (c = getopt(argc, argv, "i:t:h")) != -1) {
		switch (c) {
			case 'i':
				param.iface = optarg;
				break;
			case 't':
				param.packet_delay = strtol(optarg, (char**)NULL, 10);
				if (param.packet_delay == LONG_MIN || param.packet_delay == LONG_MAX) {
					fprintf(stderr, "Error: incorrect packet delay, using defaults.\n");
					param.packet_delay = 90;
				}
				break;
			case 'h':
				param.flags |= 0xFFFFFFFF; /* ijungiam paskutini bita */
				break;
			default:
				//fprintf(stderr, "Unknown option -%c\n", optopt);
				return (-1);
				break;
		}
	}
	
	if (optind - 2 == 0) {
		fprintf(stderr, "Error: too few arguments\n");
		return (-1);
	}
	
	return (0);
}
