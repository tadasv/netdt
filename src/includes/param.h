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

#ifndef __NETDT_PARAM_H__
#define __NETDT_PARAM_H__ 1

struct param
{
	char *iface;
	int flags;
	unsigned long packet_delay;
};

int get_opt(int, char **);

#endif
