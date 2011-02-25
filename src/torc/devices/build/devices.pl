# Torc - Copyright 2011 University of Southern California.  All Rights Reserved.
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
	xc3s100e
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

@devices::virtex6l = qw/
	xc6vlx75tl xc6vlx130tl xc6vlx195tl xc6vlx240tl xc6vlx365tl xc6vlx550tl xc6vlx760l 
	xc6vsx315tl xc6vsx475tl 
	/;

@devices::spartan3e = qw/
	xc3s100e xc3s250e xc3s500e xc3s1200e xc3s1600e
	/;

@devices::spartan6 = qw/
	xc6slx4 xc6slx9 xc6slx16 xc6slx25 xc6slx45 xc6slx75 xc6slx100 xc6slx150 
	xc6slx25t xc6slx45t xc6slx75t xc6slx100t xc6slx150t
	/;

@devices::spartan6l = qw/
	xc6slx4l xc6slx9l xc6slx16l xc6slx25l xc6slx45l xc6slx75l xc6slx100l xc6slx150l
	/;

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

	@devices::spartan3e = qw//;

	@devices::spartan6 = qw//;

	@devices::spartan6l = qw//;

}
