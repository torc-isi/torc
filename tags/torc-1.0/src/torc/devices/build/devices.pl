# Torc - Copyright 2011-2013 University of Southern California.  All Rights Reserved.
# $HeadURL$
# $Id$

# This program is free software: you can redistribute it and/or modify it under the terms of the 
# GNU General Public License as published by the Free Software Foundation, either version 3 of the 
# License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
# the GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along with this program.  If 
# not, see <http://www.gnu.org/licenses/>.

if(1) {

@devices::development = qw/
	development1
	/;

@devices::virtex = qw/
	xcv50 xcv100 xcv150 xcv200 xcv300 xcv400 xcv600 xcv800 xcv1000
	/;

@devices::virtexe = qw/
	xcv50e xcv100e xcv200e xcv300e xcv400e xcv405e xcv600e xcv812e xcv1000e xcv1600e xcv2000e 
		xcv2600e xcv3200e
	/;

@devices::virtex2 = qw/
	xc2v40 xc2v80 xc2v250 xc2v500 xc2v1000 xc2v1500 xc2v2000 xc2v3000 xc2v4000 xc2v6000 
		xc2v8000
	/;

@devices::virtex2p = qw/
	xc2vp2 xc2vp4 xc2vp7 xc2vp20 xc2vp30 xc2vp40 xc2vp50 xc2vp70 xc2vp100
	xc2vpx20 xc2vpx70 
	/;
# these are not all available at Virginia Tech
#@devices::virtex2P = qw/
#	xc2vp2 xc2vp3 xc2vp4 xc2vp7 xc2vp10 xc2vp20 xc2vp30 xc2vp40 xc2vp50 xc2vp70 xc2vp100 
#		xc2vp125
#	/;

@devices::virtex4 = qw/
	xc4vfx12 xc4vfx20 xc4vfx40 xc4vfx60 xc4vfx100 xc4vfx140 
	xc4vlx15 xc4vlx25 xc4vlx40 xc4vlx60 xc4vlx80 xc4vlx100 xc4vlx160 xc4vlx200
	xc4vsx25 xc4vsx35 xc4vsx55 
 	/;

@devices::virtex5 = qw/
	xc5vfx30t xc5vfx70t xc5vfx100t xc5vfx130t xc5vfx200t 
	xc5vlx30 xc5vlx50 xc5vlx85 xc5vlx110 xc5vlx155 xc5vlx220 xc5vlx330 
	xc5vlx20t xc5vlx30t xc5vlx50t xc5vlx85t xc5vlx110t xc5vlx155t xc5vlx220t xc5vlx330t 
	xc5vsx35t xc5vsx50t xc5vsx95t xc5vsx240t 
	xc5vtx150t xc5vtx240t 
	/;

@devices::virtex6 = qw/
	xc6vcx75t xc6vcx130t xc6vcx195t xc6vcx240t
	xc6vhx250t xc6vhx255t xc6vhx380t xc6vhx565t
	xc6vlx75t xc6vlx130t xc6vlx195t xc6vlx240t xc6vlx365t xc6vlx550t xc6vlx760
	xc6vsx315t xc6vsx475t
	/;

# Artix7 devices under ISE 14.5
@devices::artix7 = qw/
	xc7a100t xc7a200t
	/;
# Artix7 devices under ISE 13.1
#@devices::artix7 = qw/
#	xc7a8 xc7a15 xc7a30t xc7a50t xc7a100t xc7a200t xc7a350t
#	/;

# Kintex7 devices under ISE 13.1 - 14.5: xc7k30t xc7k70t xc7k160t xc7k325t xc7k410t
@devices::kintex7 = qw/
	xc7k70t xc7k160t xc7k325t xc7k355t xc7k410t xc7k420t xc7k480t
	/;

# Virtex7 devices under ISE 14.5: xc7v285t xc7v450t xc7vx485t xc7v585t xc7v855t xc7v1500t xc7v2000t
@devices::virtex7 = qw/
	xc7v585t xc7v2000t xc7vh580t xc7vh870t xc7vx330t xc7vx415t xc7vx485t xc7vx550t xc7vx690t 
		xc7vx980t xc7vx1140t
	/;
# Virtex7 devices under ISE 13.1: xc7v285t xc7v450t xc7vx485t xc7v585t xc7v855t xc7v1500t xc7v2000t
#@devices::virtex7 = qw/
#	xc7vx330t xc7vx415t xc7vx485t xc7vx550t xc7v585t xc7vx690t xc7vx980t xc7vx1140t xc7v1500t 
#		xc7v2000t
#	/;

# Virtex7 devices under ISE 14.5: xc7v585t xc7v1500t xc7v2000t xc7vh580t xc7vh870t xc7vx330t xc7vx415t
#	xc7vx485t xc7vx550t xc7vx690t xc7vx980t xc7vx1140t
@devices::virtex7 = qw/
	xc7v585t xc7v1500t xc7v2000t xc7vh580t xc7vh870t xc7vx330t xc7vx415t xc7vx485t xc7vx550t 
		xc7vx690t xc7vx980t xc7vx1140t
	/;

@devices::zynq7000 = qw/
	xc7z010 xc7z020 xc7z030 xc7z045 
	/;

@devices::spartan3e = qw/
	xc3s100e xc3s250e xc3s500e xc3s1200e xc3s1600e
	/;

@devices::spartan6 = qw/
	xc6slx4 xc6slx9 xc6slx16 xc6slx25 xc6slx45 xc6slx75 xc6slx100 xc6slx150 
	xc6slx25t xc6slx45t xc6slx75t xc6slx100t xc6slx150t
	/;

#@devices::spartan6l = qw/
#	xc6slx4l xc6slx9l xc6slx16l xc6slx25l xc6slx45l xc6slx75l xc6slx100l xc6slx150l
#	/;

} else {

	@devices::development = qw//;

	@devices::virtex = qw//;

	@devices::virtexe = qw//;

	@devices::virtex2 = qw//;

	@devices::virtex2p = qw//;

	@devices::virtex4 = qw//;

	@devices::virtex5 = qw//;

	@devices::virtex6 = qw//;

	@devices::virtex6l = qw//;

	@devices::virtex7 = qw//;

	@devices::virtex7l = qw//;

	@devices::kintex7 = qw//;

	@devices::kintex7l = qw//;

	@devices::artix7 = qw//;

	@devices::zynq = qw//;

	@devices::spartan3e = qw//;

	@devices::spartan6 = qw//;

	@devices::spartan6l = qw//;

}
