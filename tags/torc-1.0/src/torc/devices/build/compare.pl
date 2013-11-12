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

	# keep ourselves honest
	use warnings FATAL => 'all';
	use strict;
	use constant false => 0;
	use constant true => 1;

	# import console feedback
	do "feedback.pl";

	# declare functions
	sub compare($$);

	# check for the special case of comparing all related devices
	if(scalar @ARGV == 1 && $ARGV[0] eq "all") {
		# compare common Spartan6 devices
		my @spartan6 = qw/xc6slx4 xc6slx9 xc6slx16 xc6slx25 xc6slx45 xc6slx75 xc6slx100 xc6slx150/;
		foreach my $device (@spartan6) {
			compare("$device.conns.xdlrc.gz", "$device" . "l" . ".conns.xdlrc.gz");
		}
		# compare common Virtex6 devices
		my @virtex6 = qw/xc6vlx75t xc6vlx130t xc6vlx195t xc6vlx240t xc6vlx365t xc6vlx550t xc6vlx760 
			xc6vsx315t xc6vsx475t/;
		foreach my $device (@virtex6) {
			compare("$device.conns.xdlrc.gz", "$device" . "l" . ".conns.xdlrc.gz");
		}
		# compare common Virtex7 devices
		my @virtex7 = qw/xc7v285t xc7v450t xc7v585t xc7v855t xc7v1500t xc7v2000t/;
		foreach my $device (@virtex7) {
			compare("$device.conns.xdlrc.gz", "$device" . "l" . ".conns.xdlrc.gz");
		}
		# compare common Kintex7 devices
		my @kintex7 = qw/xc7k30t xc7k70t xc7k160t xc7k325t xc7k410t/;
		foreach my $device (@kintex7) {
			compare("$device.conns.xdlrc.gz", "$device" . "l" . ".conns.xdlrc.gz");
		}
		exit;
	}

	# enforce proper usage
	if(scalar @ARGV != 2) {
		print "Usage: perl $0 [xdlrc.*.gz] [xdlrc.*.gz]\n";
		exit;
	}

	# compare the two specified files
	compare(shift, shift);

	# compare two files
	sub compare($$) {

		# extract the command line parameters
		my ($xdlrc_gz_file_name1, $xdlrc_gz_file_name2) = (shift, shift);
		print "Comparing $xdlrc_gz_file_name1 and $xdlrc_gz_file_name2 ...\n";

		# open pipes through gunzip to both files
		open(XDLRC1, "zcat $xdlrc_gz_file_name1|") or die "Unable to read $xdlrc_gz_file_name1: $!\n";
		open(XDLRC2, "zcat $xdlrc_gz_file_name2|") or die "Unable to read $xdlrc_gz_file_name2: $!\n";

		my $count = 0;
		while(true) {
			$count++;
			my $line1 = <XDLRC1>;
			my $line2 = <XDLRC2>;
			# check for end of files
			my $eof1 = !defined $line1;
			my $eof2 = !defined $line2;
			# if we hit the end of both files, we're in good shape
			if($eof1 && $eof2) {
				last;
			} elsif($eof1 || $eof2) {
				$feedback::out_ref->flush();
				print "Reached the end of " . ($eof1 ? $xdlrc_gz_file_name1 : $xdlrc_gz_file_name2) 
					. " before the end of " . ($eof1 ? $xdlrc_gz_file_name2 : $xdlrc_gz_file_name1) 
					. "\n";
			}
			if($count % 10000 == 0) { print feedback::out "    Comparing line $count"; }
			# ignore identical lines
			next unless $line1 ne $line2;
			$feedback::out_ref->flush();
			print "    $count: <<< $line1    $count: >>> $line2";
		}
		$feedback::out_ref->flush();
		print "    done\n";

	}

sub __suppress_warnings__() {
	print feedback::out;
	$feedback::out_ref->flush();
}

