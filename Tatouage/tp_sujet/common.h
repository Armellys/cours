/*
	My common header file - v. 0.3 - Jun. 09
 
	Copyright (c) 2009, Gaëtan Le Guelvouit <gaetan.leguelvouit@orange-ftgroup.com>
 
	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License as
	published by the Free Software Foundation; either version 2 of
	the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be
	useful, but WITHOUT ANY WARRANTY; without even the implied
	warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
	PURPOSE.  See the GNU General Public License for more details.
 
	You should have received a copy of the GNU General Public
	License along with this program; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
	MA 02111-1307, USA.
*/   

#ifndef _COMMON_H_
#define _COMMON_H_

//#define RELEASE 

#define ERR(x) 		{cerr << "ERROR: " << x << ".\n"; return -1;}
#define CARRE(x) 	((x)*(x))

#ifndef RELEASE

#define LOG(x) 	{clog << x; clog.flush();}
#define TRACE  	{clog << "-- " << __FILE__ << "::" << __LINE__ << "... "; clog.flush();}
#define TOK		{clog << "OK\n";}

#else 

#define LOG(x) 	{}
#define TRACE  	{}
#define TOK		{}  

#endif


#endif
