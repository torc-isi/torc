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

# Parts of this code were inspired by and/or copied from ADB build code:
#   http://scholar.lib.vt.edu/theses/available/etd-09112002-143335

	# declare database format
	@::format_number = (1, 0, 1, 0);
	# format number 1.0.0.0: initial release
	# format number 1.0.1.0: 6/1/2011: extends segment tile anchor count from 16 to 32 bits
	$::build_number = 5;
	# build number 5: 5/18/2013 renamed Zynq7000; building ISE 14.5 Virtex7/Kintex7/Artix7/Zynq
	# build number 4: 1/20/2012 no code changes; building ISE 13.4 Virtex7/Kintex7/Artix7/Zynq7
	# build number 3: 6/1/2011 added Virtex7 and Kintex7, with requisite format 1.0.1
	# build number 2: 5/16/2011 removed routethroughs from regular arc list
	# build number 1: initial release

	# keep ourselves honest
	use warnings FATAL => 'all';
	use strict;
	use constant false => 0;
	use constant true => 1;
	use constant INPUT => 2;
	use constant OUTPUT => 4;
	use constant INTERNAL => 0;
	use constant UNBONDED => 1;
	use constant BONDED => 2;

	# import the devices
	do "devices.pl";
	# import utilities
	do "utils.pl";
	# import console feedback
	do "feedback.pl";

	# declare globals
	@::tile_types = ();
	@::tiles = ();
	%::tile_type_name_to_tile_type_index = ();
	@::tile_type_index_to_tile_type_name = ();
	%::tile_name_to_tile_index = ();
	%::tile_name_to_tile_type_index = ();
	%::tile_type_wire_name_to_wire_index = ();
	@::tile_type_wire_index_to_wire_name = ();
	%::tile_type_index_count = ();
	%::tile_type_regular_arcs = ();
	%::tile_type_irregular_arcs = ();
	%::tile_type_routethrough_arcs = ();
	%::tile_type_unbuffered_arcs = ();
	%::primitive_pins = ();
	%::primitive_type_name_to_primitive_type_index = ();
	@::primitive_type_index_to_primitive_type_name = ();
	%::primitive_elements = ();
	$::version_number = undef;
	@::version_number = ();
	$::version_string = undef;
	%::device_speeds = ();
	%::device_packages = ();

 
	# order numbers
	sub order_numeric() {
		# return the comparison
		return $a <=> $b;
	}

	# order device names
	sub order_devices() {
		my $cmp = undef;
		# extract the subfields from the first device
		$a =~ /^(.*?)(\d+)([Tt]?)(\D*)$/;
		my ($prefix1, $number1, $t1, $suffix1) = ($1, $2, $3, $4);
		# extract the subfields from the second device
		$b =~ /^(.*?)(\d+)([Tt]?)(\D*)$/;
		my ($prefix2, $number2, $t2, $suffix2) = ($1, $2, $3, $4);
		# order by architecture and family
		return $cmp if $cmp = $prefix1 cmp $prefix2;
		# handle the t suffix in V5 and V6 devices
		return $cmp if $cmp = $t1 cmp $t2;
		# and then by suffix if applicable
		return $cmp if $cmp = $suffix1 cmp $suffix2;
		# and finally by device
		return $number1 <=> $number2;
	}

	# order segment wires
	sub order_segment() {
		my $cmp = undef;
		# extract the subfields from the wires
		my ($wire_index_a, $tile_index_a) = split "@", $a;
		my ($wire_index_b, $tile_index_b) = split "@", $b;
		# order by tile index
		return $cmp if $cmp = $tile_index_a <=> $tile_index_b;
		# and then by wire index
		return $wire_index_a <=> $wire_index_b;
	}

	# order arcs
	sub order_arcs() {
		my $cmp = undef;
		# extract the subfields from the arcs
		my ($wire_name_1_a, $wire_name_2_a) = split ">", $a;
		my ($wire_name_1_b, $wire_name_2_b) = split ">", $b;
		# order by source wire name
		return $cmp if $cmp = $wire_name_1_a cmp $wire_name_1_b;
		# and then by sink wire name
		return $wire_name_2_a cmp $wire_name_2_b;
	}

	# order irregular arcs
	sub order_irregular_arcs() {
		my $cmp = undef;
		# extract the subfields from the arcs
		my ($source_wire_index_a, $sink_wire_index_a) = split ",", $a;
		my ($source_wire_index_b, $sink_wire_index_b) = split ",", $b;
		# order by source wire index
		return $cmp if $cmp = $source_wire_index_a <=> $source_wire_index_b;
		# and then by sink wire index
		return $sink_wire_index_a <=> $sink_wire_index_b;
	}

	# order pin definitions
	sub order_pins() {
		# extract the subfields of the pins
		my ($pin_name_a, $pin_direction_a) = split ":", $a;
		my ($pin_name_b, $pin_direction_b) = split ":", $b;
		# order by pin name
		return $pin_name_a cmp $pin_name_b;
	}

	# order primitive_def connections
	sub order_conns() {
		my $cmp = undef;
		# extract the subfield of the connection
		my ($element_a, $pin_a) = split ":", $a;
		my ($element_b, $pin_b) = split ":", $b;
		# order first by elements
		return $cmp if $cmp = $element_a cmp $element_b;
		return $pin_a cmp $pin_b;
	}

	# look for every wire name in every tile type in the architecture
	sub preprocess_architecture($@) {

		# extract the parameters
		my $architecture = shift;
		my @devices = @_;

		# initialize variables
		my $tile_type_index = undef;
		my %tile_type_index = ();
		my %tile_type_device = ();
		my @tile_type_name = ();
		my %tile_type_index_count = ();
		my %tile_type_wire_index = ();
		my %tile_type_all_arcs = ();
		my %tile_type_routethrough_arcs = ();
		my %tile_type_irregular_arcs = ();
		my %tile_type_unbuffered_arcs = ();
		my %primitive_pins = ();
		my %primitive_conns = ();
		my %primitive_elements = ();
		my %device_tiles = ();

		# open a log file
		my $log_file_name = "$architecture.pre.log";
		open(LOG, ">$log_file_name") or die "Unable to write $log_file_name: $!\n";

		# analyze the first device for primitive definitions
		{
			# open the xdlrc file
			my $device = $devices[0];
			# open the xdlrc file
			my $device_file_name = "$device.conns.xdlrc.gz";
			open(XDLRC, "zcat $device_file_name|") or die "Unable to read $device_file_name: $!\n";
			$| = 1; print "Preprocessing $architecture primitives ...  "; $| = 0;

			# reset the variables
			my $defs = false;
			my ($primitive_count) = (undef);
			my ($primitive_name, $primitive_pin_count, $primitive_element_count) = (undef, undef, undef);
			my ($primitive_pin_element, $primitive_pin_name, $primitive_pin_direction) = (undef, undef, undef);
			my ($primitive_element_name, $primitive_element_pin_count, $primitive_element_bel) = (undef, undef, undef);
			my ($primitive_element_pin_name, $primitive_element_pin_direction) = (undef, undef);
			my @primitive_element_values = ();
			my @primitive_pins = ();
			my @primitive_element_pins = ();
			my (@conns, %conns) = ((), ());
			my $line = undef;

			# extract the version information
			$line = <XDLRC>;
			$line =~ /^# (\S+) "(.+)"$/;
			($::version_number, $::version_string) = ($1, $2);
			@::version_number = split /\./, $::version_number;

			# iterate over the lines
			while($line = <XDLRC>) {

				# ignore everything until we find the primitive_defs construct
				next unless $defs or $line =~ /^\s*\(primitive_defs (\d+)\s*$/;
				$defs = true;

				# ignore anything other than primitive definitions
				next unless $line =~ /^\s*(\((primitive_defs|primitive_def|pin|element|cfg|conn) |\))/;

				# process primitive_defs declaration
				if($line =~ /^\s*\(primitive_defs (\d+)\s*$/) {
					$primitive_count = $1;

				# process primitive_def declarations
				} elsif($line =~ /^\s*\(primitive_def (\S+) (\d+) (\d+)\s*$/) {
					# extract the variables
					($primitive_name, $primitive_pin_count, $primitive_element_count) = ($1, $2, $3);
					#print "$primitive_name\n";
					@primitive_pins = ();
					@conns = ();
					%conns = ();
					$primitive_pins{$primitive_name} = [];
					$primitive_conns{$primitive_name} = [];
					$primitive_elements{$primitive_name} = {};

				# process primitive_def pin declarations
				} elsif($primitive_pin_count && $line =~ /^\s*\(pin (\S+) (\S+) (input|output)\)\s*$/) {
					# extract the variables
					($primitive_pin_element, $primitive_pin_name, $primitive_pin_direction) = ($1, $2, $3);
					push @primitive_pins, "$primitive_pin_name:$primitive_pin_direction";
					#print "    $primitive_pin_name: $primitive_pin_direction ($primitive_pin_count)\n";
					# update our pin count
					$primitive_pin_count--;

				# process primitive_def element declarations
				} elsif($line =~ /^\s*\(element (\S+) (\d+)\s*(# BEL)?\s*$/) {
					# extract the variables
					($primitive_element_name, $primitive_element_pin_count) = ($1, $2);
					$primitive_element_bel = (defined ($3) && $3 eq "# BEL") ? true : false;
					@primitive_element_values = ();
					# prepare the element variables
					@primitive_element_pins = ();
					$primitive_elements{$primitive_name}{$primitive_element_name}{"bel"} = $primitive_element_bel;
					$primitive_elements{$primitive_name}{$primitive_element_name}{"cfg"} = "";
					$primitive_elements{$primitive_name}{$primitive_element_name}{"pin"} = "";

				# process primitive_def element pin declarations
				} elsif(!$primitive_pin_count && $line =~ /^\s*\(pin (\S+) (input|output)\)\s*$/) {
					# extract the variables
					($primitive_element_pin_name, $primitive_element_pin_direction) = ($1, $2);
					push @primitive_element_pins, "$primitive_element_pin_name:$primitive_element_pin_direction";

				# process primitive_def element cfg declarations: these are values that the name element can take on
				} elsif($line =~ /^\s*\(cfg \s*(.*?)\s*\)\s*$/) {
					# extract the variables
					@primitive_element_values = split /\s+/, $1;
					$primitive_elements{$primitive_name}{$primitive_element_name}{"cfg"} = join ",", @primitive_element_values;

				# process primitive_def element conn declarations: these are wires between element pins
				} elsif($line =~ /^\s*\(conn (\S+) (\S+) (<==|==>) (\S+) (\S+)\)\s*$/) {
					# extract the variables
					my ($primitive_conn_source_element, $primitive_conn_source_pin, $primitive_conn_direction, 
						$primitive_conn_sink_element, $primitive_conn_sink_pin) = ($1, $2, $3, $4, $5);
					if($primitive_conn_direction eq "<==") {
						($primitive_conn_source_element, $primitive_conn_sink_element) = 
							($primitive_conn_sink_element, $primitive_conn_source_element);
						($primitive_conn_source_pin, $primitive_conn_sink_pin) = 
							($primitive_conn_sink_pin, $primitive_conn_source_pin);
					}
					$conns{"$primitive_conn_source_element:$primitive_conn_source_pin"}
						{"$primitive_conn_sink_element:$primitive_conn_sink_pin"} = undef;

				} elsif($line =~ /^\s*\)\s*$/) {
					if($primitive_element_count) {
						# close the element
						$primitive_elements{$primitive_name}{$primitive_element_name}{"pin"} = join ",", @primitive_element_pins;
						$primitive_element_count--;
					} elsif($primitive_count) {
						# close the primitive
						@conns = ();
						foreach my $conn (sort order_conns keys %conns) {
							my @sinks = sort order_conns keys %{$conns{$conn}};
							push @conns, (join ":", ($conn, @sinks));
						}
						@{$primitive_pins{$primitive_name}} = @primitive_pins;
						@{$primitive_conns{$primitive_name}} = @conns;
						$primitive_count--;
					}

				} else {
					die "Failed to process line \"$line\".\n";

				}

			}

			print "done: " . (scalar keys %primitive_pins) . " primitives\n";

		}

		# iterate through the devices
		my $device_num = 1;
		my $device_count = scalar @devices;
		foreach my $device (@devices) {

			# open the xdlrc file
			my $device_file_name = "$device.conns.xdlrc.gz";
			open(XDLRC, "zcat $device_file_name|") or die "Unable to read $device_file_name: $!\n";
			print "Preprocessing $device ($device_num of $device_count) ...  ";
			$device_num++;

			# reset the variables
			my ($row_count, $col_count, $tile_count) = (undef, undef, undef);
			my @tiles = ();
			my ($tile_row, $tile_col, $tile_name, $tile_type_name, $tile_site_count) = (undef, undef, undef, undef, undef);
			my ($wire_name, $wire_connection_count) = (undef, undef);
			my ($site_name, $site_type_name, $site_bonding, $site_pin_count) = (undef, undef, undef, undef);
			my ($wire_indexes, $wire_index) = (undef, undef);
			my ($tile_index) = (-1);
			my %unaccounted_arcs = ();
			my $new_tile_type = false;
			my $unaccounted_arcs = ""; 
			$device_tiles{$device} = \@tiles;
			my $line = undef;

			# extract the version information
			$line = <XDLRC>;
			$line =~ /^# (\S+) "(.+)"$/;
			my($version_number, $version_string) = ($1, $2);
			if($version_number ne $::version_number) {
				print LOG "WARNING: $device data version $version_number ($version_string) differs from prior version $::version_number ($::version_string).\n";
				print STDERR "WARNING: $device data version $version_number ($version_string) differs from prior version $::version_number ($::version_string).\n";
			}

			# iterate over the lines
			while($line = <XDLRC>) {

				# ignore anything other than tile or wire declarations
				next unless $line =~ /^\s*\((tiles|tile|wire|pip|tile_summary) /;
#die "Currently in debugging mode.  (No site counts at the end of tile declarations.)  Please change code back to normal mode.\n" if $line =~ /^\s*\(tile (\d+) (\d+) (\w+) (\w+) (\d+)?\s*$/;

				# process tiles declarations
				if($line =~ /^\s*\(tiles (\d+) (\d+)\s*$/) {
					# extract the variables
					($row_count, $col_count) = ($1, $2);
					$tile_count = $row_count * $col_count;

				# process tile declarations
				} elsif($line =~ /^\s*\(tile (\d+) (\d+) (\w+) (\w+) (\d+)?\s*$/) {
#} elsif($line =~ /^\s*\(tile (\d+) (\d+) (\w+) (\w+)\s*$/) {
					# extract the variables
					($tile_row, $tile_col, $tile_name, $tile_type_name, $tile_site_count) = ($1, $2, $3, $4, $5);
$tile_site_count = 0 unless defined $tile_site_count;
					$tile_index++;
					push @tiles, "$tile_index,$tile_row,$tile_col,$tile_name,$tile_type_name,$tile_site_count";
#print $tiles[-1] . "\n";
					# look up the tile type index
					if(exists $tile_type_index{$tile_type_name}) {
						$new_tile_type = false;
						$unaccounted_arcs = join ",", keys %{$tile_type_all_arcs{$tile_type_name}};
						#$tile_type_index = $tile_type_index{$tile_type_name};
						$tile_type_index_count{$tile_type_name}++;
					# create a new tile type index
					} else {
						$new_tile_type = true;
						#$tile_type_index = scalar keys %tile_type_index;
						#$tile_type_index{$tile_type_name} = $tile_type_index;
						$tile_type_index_count{$tile_type_name} = 1;
						$tile_type_wire_index{$tile_type_name} = {};
					}
					$tile_type_index{$tile_type_name} = undef;
					$wire_indexes = \$tile_type_wire_index{$tile_type_name};
					$tile_type_device{$tile_type_name}{$device} = undef;
					print feedback::out "$tile_index of $tile_count: $tile_type_name \"$tile_name\"";

				# process wire declarations
				} elsif($line =~ /^\s*\(wire (\w+) (\d+)\s*\)?\s*$/) {
					# extract the variables
					($wire_name, $wire_connection_count) = ($1, $2);
					# look up the wire index
					if(exists ${$$wire_indexes}{$wire_name}) {
						$wire_index = ${$$wire_indexes}{$wire_name};
					} else {
						$wire_index = scalar keys %{$$wire_indexes};
						${$$wire_indexes}{$wire_name} = $wire_index;
					}

				# process pip declarations
				} elsif($line =~ /^\s*\(pip (\w+) (\w+) (==|=>|=-|->) (\w+)( \(_ROUTETHROUGH-(\S+)-(\S+) (\S+)\))?\)\s*$/) {
					# ==   Bidirectional, unbuffered
					# =>   Bidirectional, buffered in one direction
					# =-   Bidirectional, buffered in both directions
					# ->   Directional, buffered
					# extract the variables
					my ($pip_tile_name, $wire_name_1, $direction, $wire_name_2) = ($1, $2, $3, $4);
					my ($is_routethrough, $rt_source_pin, $rt_sink_pin, $rt_primitive_def) = ($5, $6, $7, $8);
					die "Pip tile name $pip_tile_name does not match current tile name $tile_name.\n" unless $pip_tile_name eq $tile_name;
					# determine the arc directionality
					die "Unsupported pip direction $direction for \"$line\".\n" if $direction eq "=>";
					#die "Unsupported pip direction $direction for \"$line\".\n" if $direction eq "==";
if(!$::reported_unbuffered_bidirectional && $direction eq "==") {
	$::reported_unbuffered_bidirectional = true;
	print "\nEncountered type '==' bidirectional unbuffered pips: $tile_name ($tile_type_name) pip $wire_name_1$direction$wire_name_2\n";
}
					# process things differently depending upon whether we've seen this tile type before
					my $forward_arc = "$wire_name_1>$wire_name_2";
					my $reverse_arc = "$wire_name_2>$wire_name_1";
					my ($defined_forward, $defined_reverse) = (true, $direction =~ /^(=-|==)$/);
					if(!$new_tile_type) {
						# remove this arc from the tile's list
						my $found_forward = $unaccounted_arcs =~ s/\b$forward_arc\b,?//;
						my $found_reverse = $unaccounted_arcs =~ s/\b$reverse_arc\b,?// if $defined_reverse;
						# if the forward arc was defined, but we didn't find it, flag it as irregular
						$tile_type_irregular_arcs{$tile_type_name}{$forward_arc} = undef if $defined_forward && !$found_forward;
						# if the reverse arc was defined, but we didn't find it, flag it as irregular
						$tile_type_irregular_arcs{$tile_type_name}{$reverse_arc} = undef if $defined_reverse && !$found_reverse;
					}
					# remember all original arcs
					$tile_type_all_arcs{$tile_type_name}{$forward_arc} = undef if $defined_forward;
					$tile_type_all_arcs{$tile_type_name}{$reverse_arc} = undef if $defined_reverse;
					# remember all routethrough arcs
					$tile_type_routethrough_arcs{$tile_type_name}{$forward_arc} = undef if $defined_forward && $is_routethrough;
					$tile_type_routethrough_arcs{$tile_type_name}{$reverse_arc} = undef if $defined_reverse && $is_routethrough;
					# note any unbuffered arcs
					if($direction eq "==") {
						$tile_type_unbuffered_arcs{$tile_type_name}{$forward_arc} = undef if $defined_forward;
						$tile_type_unbuffered_arcs{$tile_type_name}{$reverse_arc} = undef if $defined_reverse;
						print LOG "$device: $tile_name ($tile_type_name) pip $wire_name_1$direction$wire_name_2\n";
					}
					
				# process tile summaries
				} elsif($line =~ /^\s*\(tile_summary /) {
					# determine which arcs were not accounted for
					while($unaccounted_arcs =~ s/^([^,]+),//) {
#print "$tile_type_name: unaccounted $1\n";#$unaccounted_arcs\n";
#<STDIN>;
						$tile_type_irregular_arcs{$tile_type_name}{$1} = undef;
					}

				# catch any lines that were skipped because of bad regular expressions
				} elsif($line =~ /^\s*\((tiles|tile|wire|pip|tile_summary) /) {
					# this has to be treated as a fatal error
					die "Failed to process line \"$line\".";

				}

			}
			$feedback::out_ref->flush();

			print "done: " . (scalar keys %tile_type_index) . " total tile types.\n";
			close XDLRC;
		}

#		# determine which arcs were irregular
#		foreach my $tile_type_name (sort keys %tile_type_irregular_arcs) {
#			print "$tile_type_name:\n";
#			foreach my $irregular_arc (sort keys %{$tile_type_irregular_arcs{$tile_type_name}}) {
#				print "    $irregular_arc\n";
#			}
#		}

		# assign tile type indexes
		my $index = 0;
		foreach my $tile_type_name (sort keys %tile_type_index) {
			#my @devices = sort order_devices keys %{$tile_type_device{$tile_type_name}};
			#print "    $index: $tile_type_name ($tile_type_index_count{$tile_type_name}): " . (join ", ", @devices) . "\n";
			$tile_type_name[$index] = $tile_type_name;
			$tile_type_index{$tile_type_name} = $index;
			$index++;
		}

		# replace the tile type names in the tile map with tile type indexes
		foreach my $device (sort order_devices keys %device_tiles) {
			# look up the tilemap
			foreach my $tile_entry (@{$device_tiles{$device}}) {
				my ($tile_index, $tile_row, $tile_col, $tile_name, $tile_type_name, $tile_site_count) = split ",", $tile_entry;
				my $tile_type_index = $tile_type_index{$tile_type_name};
				$tile_entry = join ",", $tile_index, $tile_row, $tile_col, $tile_name, $tile_type_index, $tile_site_count;
			}
		}

		# write out the device digests
		foreach my $device (sort order_devices keys %device_tiles) {
			my $device_digest_file_name = "$device.digest";
			open(DEVICE, ">$device_digest_file_name") or die "Unable to write $device_digest_file_name: $!\n";
			foreach my $tile_entry (@{$device_tiles{$device}}) {
				print DEVICE "$tile_entry\n";
			}
			close DEVICE;
		}

		# write out the architecture digest
		my $architecture_digest_file_name = "$architecture.digest";
		open(ARCH, ">$architecture_digest_file_name") or die "Unable to write $architecture_digest_file_name: $!\n";
		# write the version information
		print ARCH "$::version_number,$::version_string\n";
		print ARCH "\n";
		# write the primitive pin information
		foreach my $primitive_def (sort keys %primitive_pins) {
			# look up the primitive's pins
			print ARCH "$primitive_def;";
			my @primitive_pins = sort order_pins @{$primitive_pins{$primitive_def}};
			my @primitive_conns = sort order_conns @{$primitive_conns{$primitive_def}};
			print ARCH "" . (join ",", @primitive_pins) . ";" . (join ",", @primitive_conns) . ";";
			foreach my $element_name (sort keys %{$primitive_elements{$primitive_def}}) {
				my $bel = $primitive_elements{$primitive_def}{$element_name}{"bel"};
				my $cfgs = $primitive_elements{$primitive_def}{$element_name}{"cfg"};
				my $pins = $primitive_elements{$primitive_def}{$element_name}{"pin"};
				print ARCH "$element_name|$bel|$pins|$cfgs;";
			}
			print ARCH "\n";
		}
		# write a spacer
		print ARCH "\n";
		# write the tile type information
		for($index = 0; $index < scalar @tile_type_name; $index++) {
			# look up the tile type information
			my $tile_type_name = $tile_type_name[$index];
			my $all_arcs_ref = $tile_type_all_arcs{$tile_type_name};
			my $irregular_arcs_ref = $tile_type_irregular_arcs{$tile_type_name};
			my $routethrough_arcs_ref = $tile_type_routethrough_arcs{$tile_type_name};
			my $wire_index_ref = $tile_type_wire_index{$tile_type_name};
			my @devices = sort order_devices keys %{$tile_type_device{$tile_type_name}};
			my @wire_names = sort keys %{$tile_type_wire_index{$tile_type_name}};
			# reorder the wire index mapping, since we've just sorted the wire names
			for(my $i = 0; $i < scalar @wire_names; $i++) { $$wire_index_ref{$wire_names[$i]} = $i; }
			# write the tile type header and the wire names
			print "    $index: $tile_type_name (" . (scalar @wire_names) . " wires)\n";
			print ARCH "$index,$tile_type_name;" . (join ",", @wire_names) . ";";
			# write the regular arcs
			my @arcs = ();
			foreach my $arc (sort order_arcs keys %$all_arcs_ref) {
				# extract the wire names and convert to wire indexes
				# these arcs will already be in order, because we sorted by name, and the indexes were assigned in alphabetical order
				my ($source_wire_name, $sink_wire_name) = split ">", $arc;
#print "    $tile_type_name: $source_wire_name > $sink_wire_name\n";
				my ($source_wire_index, $sink_wire_index) = ($$wire_index_ref{$source_wire_name}, $$wire_index_ref{$sink_wire_name});
				push @arcs, "$source_wire_index>$sink_wire_index";
			}
			print ARCH (join ",", @arcs) . ";";
			# write the irregular arcs
			print ARCH (join ",", sort order_arcs keys %$irregular_arcs_ref) . ";";
			# write the routethrough arcs
			my @routethrough_arcs = ();
			foreach my $arc (sort order_arcs keys %$routethrough_arcs_ref) {
				# extract the wire names and convert to wire indexes
				# these arcs will already be in order, because we sorted by name, and the indexes were assigned in alphabetical order
				my ($source_wire_name, $sink_wire_name) = split ">", $arc;
				my ($source_wire_index, $sink_wire_index) = ($$wire_index_ref{$source_wire_name}, $$wire_index_ref{$sink_wire_name});
				push @routethrough_arcs, "$source_wire_index>$sink_wire_index";
			}
			print ARCH (join ",", @routethrough_arcs) . ";";
			#foreach my $wire_name (sort keys %wire_index) {
			#	print ARCH "$wire_name:$wire_index{$wire_name},";
			#}
			print ARCH "\n";
		}
		close ARCH;

		close LOG;
	}


	# take in an ordered array, and return a hash that maps array entries to indexes
	sub array_to_sequenced_hash(@) {
		my @hash = ();
		my $index = 0;
		foreach my $name (@_) {
			push @hash, ($name, $index++);
		}
		return @hash;
	}

	# read the digest information for the architecture
	sub read_architecture_digest($) {
		my $architecture = shift;
		# clear out all the variables
		@::tile_types = ();
		@::tile_type_index_to_tile_type_name = ();
		%::tile_type_name_to_tile_type_index = ();
		@::tile_type_wire_index = ();
		%::tile_type_wire_name_to_wire_index = ();
		@::tile_type_wire_index_to_wire_name = ();
		@::tile_type_regular_arcs = ();
		@::tile_type_irregular_arcs = ();
		@::tile_type_routethrough_arcs = ();
		%::tile_type_wire_index_attributes = ();
		%::primitive_pins = ();
		%::primitive_type_name_to_primitive_type_index = ();
		@::primitive_type_index_to_primitive_type_name = ();
		%::primitive_elements = ();
		%::pinwire_map = ("" => 0);
		@::pinwire_map = ("");
		# read the architecture digest
		my $architecture_digest_file_name = "$architecture.digest";
		print "Reading $architecture_digest_file_name ...  ";
		open(ARCH, $architecture_digest_file_name) or die "Unable to read $architecture_digest_file_name: $!\n";
		my @lines = <ARCH>;
		chomp @lines;
		# read the version information
		($::version_number, $::version_string) = split ",", (shift @lines);
		@::version_number = split /\./, $::version_number;
		shift @lines;
		# extract the tile type info
		my $pin_mode = true;
		my $primitive_type_index = 0;
		foreach my $line (@lines) {
			if(length $line == 0) {
				$pin_mode = false;
			} elsif($pin_mode) {
				my ($primitive_type_name, $primitive_type_pins, $primitive_type_conns, @primitive_type_elements) = split ";", $line;
				@{$::primitive_pins{$primitive_type_name}} = split ",", $primitive_type_pins;
				@{$::primitive_conns{$primitive_type_name}} = split ",", $primitive_type_conns;
				$::primitive_type_index_to_primitive_type_name[$primitive_type_index] = $primitive_type_name;
				$::primitive_type_name_to_primitive_type_index{$primitive_type_name} = $primitive_type_index;
				$primitive_type_index++;
				@{$::primitive_elements{$primitive_type_name}} = @primitive_type_elements;
#print "$primitive_type_name\n";
#foreach my $block (@primitive_type_elements) {
#	print "\t$block\n";
#}
			} else {
				push @::tile_types, $line;
				my ($tile_type_header, $tile_type_wires, $tile_type_regular_arcs, $tile_type_irregular_arcs, 
					$tile_type_routethrough_arcs) = split ";", $line;
				my ($tile_type_index, $tile_type_name) = split ",", $tile_type_header;
				my @tile_type_wires = split ",", $tile_type_wires;
				my %tile_type_regular_arcs = utils::array_to_hash(split ",", $tile_type_regular_arcs);
				my %tile_type_irregular_arcs = utils::array_to_hash(split ",", $tile_type_irregular_arcs);
				my %tile_type_routethrough_arcs = utils::array_to_hash(split ",", $tile_type_routethrough_arcs);
				$::tile_type_index_to_tile_type_name[$tile_type_index] = $tile_type_name;
				$::tile_type_name_to_tile_type_index{$tile_type_name} = $tile_type_index;
				$::tile_type_wire_index[$tile_type_index] = \@tile_type_wires;
				%{$::tile_type_wire_name_to_wire_index{$tile_type_index}} = array_to_sequenced_hash(@tile_type_wires);
				$::tile_type_wire_index_to_wire_name[$tile_type_index] = \@tile_type_wires;
				$::tile_type_regular_arcs[$tile_type_index] = \%tile_type_regular_arcs;
				$::tile_type_irregular_arcs[$tile_type_index] = \%tile_type_irregular_arcs;
				$::tile_type_routethrough_arcs[$tile_type_index] = \%tile_type_routethrough_arcs;
				#print "" . (join ",", %{$::tile_type_wire_name_to_wire_index{$tile_type_index}}) . "\n";
			}
		}
		close ARCH;
		print "done.\n";
	}

	# read the package digest information for the architecture
	sub read_package_digest($) {
		my $architecture = shift;
		# clear out all the variables
		%::device_speeds = ();
		%::device_packages = ();
		# read the architecture digest
		my $package_digest_file_name = "$architecture.packages.digest";
		print "Reading $package_digest_file_name ...  ";
		open(PACK, $package_digest_file_name) or die "Unable to read $package_digest_file_name: $!\n";
		my @lines = <PACK>;
		chomp @lines;
		# extract the device packages
		foreach my $line (@lines) {
			my ($device, $speeds, @packages) = split ":", $line;
			$::device_speeds{$device} = $speeds;
			foreach my $package (@packages) {
				my ($package, @mappings) = split ";", $package;
				foreach my $mapping (@mappings) {
					my ($site_index, $site_name, $site_bonding_string) = split ",", $mapping;
					my $site_bonding = INTERNAL;
					$site_bonding = UNBONDED if $site_bonding_string eq "unbonded";
					$site_bonding = BONDED if $site_bonding_string eq "bonded";
					push @{$::device_packages{$device}{$package}}, "$site_index,$site_name,$site_bonding";
				}
			}
		}
		close PACK;
		print "done.\n";
	}

	# read the digest information for the device
	sub read_device_digest($) {
		my $device = shift;
		# clear out all the variables
		@::tiles = ();
		%::tile_name_to_tile_index = ();
		%::tile_name_to_tile_type_index = ();
		# read the device digest
		my $device_digest_file_name = "$device.digest";
		print "Reading $device_digest_file_name ...  ";
		open(DEVICE, $device_digest_file_name) or die "Unable to read $device_digest_file_name: $!\n";
		my @lines = <DEVICE>;
		chomp @lines;
		# extract the tile info (i.e. the tile map)
		foreach my $line (@lines) {
			push @::tiles, $line;
			my ($tile_index, $tile_row, $tile_col, $tile_name, $tile_type_index, $tile_site_count) = split ",", $line;
			$::tile_name_to_tile_index{$tile_name} = $tile_index;
			$::tile_name_to_tile_type_index{$tile_name} = $tile_type_index;
		}
		close DEVICE;
		print "done.\n";
	}

	# compact a segment
	sub compact_segment(@) {
		# declare the compacted segment
		my @compact_segment = ();
		# sort the segment wires
		my @segment = sort order_segment @_;
		# extract the anchor tile index
		my ($anchor_wire_index, $anchor_tile_index) = split "@", $segment[0];
		# construct the compacted segment
		foreach my $tilewire (@segment) {
			# extract the components
			my ($wire_index, $tile_index) = split "@", $tilewire;
			push @compact_segment, "$wire_index@" . ($tile_index - $anchor_tile_index);
		}
		# return the anchor tile index and the compacted segment
		return ($anchor_tile_index, join ",", @compact_segment);
	}

	# extract device segments
	sub extract_segments($$$$) {

		# extract the parameters
		my $architecture = shift;
		my $device = shift;
		my $device_num = shift;
		my $device_count = shift;

		# open the xdlrc file
		my $device_file_name = "$device.conns.xdlrc.gz";
		my $log_file_name = "$device.log";
		my $digest_file_name = "../$device";
		open(LOG, ">$log_file_name") or die "Unable to write $log_file_name: $!\n";
		open(DIGEST, ">$digest_file_name") or die "Unable to write $digest_file_name: $!\n";
		open(XDLRC, "zcat $device_file_name|") or die "Unable to read $device_file_name: $!\n";
		print "Processing $device ($device_num of $device_count) ...  ";

		# initialize variables
		my $line = undef;
		my ($row_count, $col_count) = (undef, undef);
		my $tile_name = undef;
		my $tile_index = undef;
		my $tile_type_name = undef;
		my $tile_type_index = undef;
		my @tile_unaccounted_wires = ();
		my ($site_name, $site_type_name, $site_bonding, $site_pin_count) = (undef, undef, undef, undef);
		my $primitive_type_index = undef;
		my $primitive_site_index = undef;
		my $site_type_index = undef;
		my %pinwires = ();
		my %unaccounted_wires = ();
		my $connection_count = undef;
		my @segment = ();
		my %segment = ();
		my %compact_segment = ("" => "");
		my @compact_segments = ("");
		my %original_compact_segment = ();
		my $trivial_segments = 0;
		my $device_wire_count = 0;
		my $wire_name_to_wire_index_ref = undef;
		my $wire_index_attributes_ref = undef;
#		my $arc_ref = undef;
		my $regular_arcs_ref = undef;
		my $irregular_arcs_ref = undef;
		my $routethrough_arcs_ref = undef;
		my @tile_irregular_arcs = ();
		my %irregular_arcs = ();
		my $tile_count = scalar @::tiles;
		my @primitive_sites = ();

		# extract the version information
		$line = <XDLRC>;
		$line =~ /^# (\S+) "(.+)"$/;
		($::version_number, $::version_string) = ($1, $2);
		@::version_number = split /\./, $::version_number;

		# iterate over the lines
		while($line = <XDLRC>) {
			
			# ignore anything other than wire or connection declarations
			next unless $line =~ /^\s*\((tiles|tile|primitive_site|pinwire|wire|conn|pip|tile_summary) /;
#die "Currently in debugging mode.  (No site counts at the end of tile declarations.)  Please change code back to normal mode.\n" if $line =~ /^\s*\(tile (\d+) (\d+) (\w+) (\w+) (\d+)?\s*$/;

			# process tiles declarations
			if($line =~ /^\s*\(tiles (\d+) (\d+)\s*$/) {
				# extract the variables
				($row_count, $col_count) = ($1, $2);
				
			# process tiles declarations
			} elsif($line =~ /^\s*\(tile (\d+) (\d+) (\w+) (\w+) (\d+)\s*$/) {
#} elsif($line =~ /^\s*\(tile (\d+) (\d+) (\w+) (\w+)\s*$/) {
				# extract the variables
				($tile_name, $tile_type_name) = ($3, $4);
				my ($tile_row, $tile_col, $tile_site_count) = ($1, $2, $5);
				$tile_site_count = 0 unless defined $tile_site_count;
				# look up the tile index and tile type index
				$tile_index = $::tile_name_to_tile_index{$tile_name};
				$tile_type_index = $::tile_name_to_tile_type_index{$tile_name};
				$wire_name_to_wire_index_ref = $::tile_type_wire_name_to_wire_index{$tile_type_index};
				$wire_index_attributes_ref = \%{$::tile_type_wire_index_attributes{$tile_type_index}};
#				$arc_ref = \%{$::tile_type_arc{$tile_type_index}};
				$::tile_type_index_count{$tile_type_index}++;
				$regular_arcs_ref = $::tile_type_regular_arcs[$tile_type_index];
				$irregular_arcs_ref = $::tile_type_irregular_arcs[$tile_type_index];
				$routethrough_arcs_ref = $::tile_type_routethrough_arcs[$tile_type_index];
				%irregular_arcs = ();
				%unaccounted_wires = ();
				my $last_wire = -1 + scalar keys %{$wire_name_to_wire_index_ref};
				foreach my $wire_index (0..$last_wire) {
					$unaccounted_wires{$wire_index} = undef;
				}
				$tile_irregular_arcs[$tile_index] = [];
				print LOG "$tile_index: $tile_name ($tile_type_index: $tile_type_name)\n";
				print feedback::out "$tile_index of $tile_count: $tile_type_name \"$tile_name\"";
#print "$tile_name ($tile_type_name): " . (join ", ", sort order_arcs keys %$irregular_arcs_ref) . "\n";
#<STDIN>;
#print "wires: " . (join ", ", sort keys %$wire_name_to_wire_index_ref) . "\n";
#print "wires: " . (join ", ", sort keys %{$::tile_type_wire_name_to_wire_index{$tile_type_index}}) . "\n";
#print "" . (join ",", sort keys %{$::tile_type_wire_name_to_wire_index{$tile_type_index}}) . "\n";

			# process primitive_site declarations (use \S+ for name, because Virtex/-E can look like CLB_R11C18.S1
			} elsif($line =~ /^\s*\(primitive_site (\S+) (\w+) (\w+) (\d+)\)?\s*$/) {
				# extract the variables
				my $site_bonding_string;
				($site_name, $site_type_name, $site_bonding_string, $site_pin_count) = ($1, $2, $3, $4);
				$site_bonding = INTERNAL if $site_bonding_string eq "internal";
				$site_bonding = UNBONDED if $site_bonding_string eq "unbonded";
				$site_bonding = BONDED if $site_bonding_string eq "bonded";
				%pinwires = ();
				# look up the primitive site type
				$primitive_type_index = $::primitive_type_name_to_primitive_type_index{$site_type_name};
				next if $site_pin_count;
				# remember the primitive site
				push @primitive_sites, "$site_name,$primitive_type_index,$tile_index,$site_bonding,0";

			# process pinwire statements
			} elsif($site_pin_count && $line =~ /^\s*\(pinwire (\w+) (\w+) (\w+)\s*\)\s*$/) {
				# extract the variables
				my ($pinwire_name, $pinwire_type, $wire_name) = ($1, $2, $3);
				my $wire_index = $::tile_type_wire_name_to_wire_index{$tile_type_index}{$wire_name};
				if($pinwire_type eq "input") {
					$$wire_index_attributes_ref{$wire_index} |= INPUT;
				} elsif($pinwire_type eq "output") {
					$$wire_index_attributes_ref{$wire_index} |= OUTPUT;
				}
				# remember the pinwires
				$pinwires{$pinwire_name} = $wire_index;
				$site_pin_count--;
				next if $site_pin_count;
				# set up the pinwire name to wire index mapping
				my @pinwires = ();
				foreach my $primitive_type_pin (@{$::primitive_pins{$site_type_name}}) {
					# extract the pinwire data
					my ($pinwire, $direction) = split ":", $primitive_type_pin;
					# look up the pinwire index
					$wire_index = $pinwires{$pinwire};
					$wire_index = -1 unless defined $wire_index;
					# build the pinwire set
					push @pinwires, $wire_index;
				}
				# express and look up the pinwire set, or create it if it's new
				my $pinwires = join ",", @pinwires;
				my $pinwire_map = $::pinwire_map{$pinwires};
				unless(defined $pinwire_map) {
					$pinwire_map = scalar keys %::pinwire_map;
					$::pinwire_map{$pinwires} = $pinwire_map;
					$::pinwire_map[$pinwire_map] = $pinwires;
				}
				# remember the primitive site
				push @primitive_sites, "$site_name,$primitive_type_index,$tile_index,$site_bonding,$pinwire_map";

			# process wire statements
			} elsif($line =~ /^\s*\(wire (\w+) (\d+)\s*\)?\s*$/) {
				# clear the segment
				@segment = ();
				# extract the variables
				my $wire_name = $1;
				$connection_count = $2;
				# count trivial segments
				$trivial_segments++ if $connection_count == 0;
				# count device wires
				$device_wire_count++;
				# look up the wire index
				my $wire_index = $::tile_type_wire_name_to_wire_index{$tile_type_index}{$wire_name};
				# push the tilewire into the segment
				push @segment, "$wire_index\@$tile_index";
				# remove this wire
				delete $unaccounted_wires{$wire_index};

			# process connection statements
			} elsif($line =~ /^\s*\(conn (\w+) (\w+)\s*\)\s*$/) {
				# extract the variables
				my ($conn_tile_name, $conn_wire_name) = ($1, $2);
				# look up the tile index and tile type index
				my $conn_tile_index = $::tile_name_to_tile_index{$conn_tile_name};
				my $conn_tile_type_index = $::tile_name_to_tile_type_index{$conn_tile_name};
				my $conn_wire_index = $::tile_type_wire_name_to_wire_index{$conn_tile_type_index}{$conn_wire_name};
				push @segment, "$conn_wire_index\@$conn_tile_index";
				# process the segment when we've hit the final connection
				next unless --$connection_count == 0;
				# count the number of times we see the exact same segment
				my $segment = join ",", sort order_segment @segment;
				if(exists $segment{$segment}) {
					$segment{$segment}++;
					#print LOG "$segment{$segment}-plicate $segment\n";
					# we already have this exact segment (and hence the corresponding compacted segment); move along
					next;
				} else {
					$segment{$segment} = 1;
					#print LOG "new $segment\n";
				}
				# compact the segment
				my ($anchor_tile_index, $compact_segment) = compact_segment(@segment);
				# determine whether any wire in the segment is already associated with an existing segment
#				my $mismatch = 0;
my $found = false;
				foreach my $tilewire (@segment) {
					my $original_compact_segment = $original_compact_segment{$tilewire};
					# determine whether there is a mismatch
					my $mismatch = defined $original_compact_segment && $original_compact_segment ne $compact_segment;
					# determine whether the existing compact segment is a superset of the current one
					my $superset = true;
					if($mismatch) {
						foreach my $offsetwire (split ",", $compact_segment) {
							if($original_compact_segment !~ /\b$offsetwire\b/) {
								$superset = false;
								last;
							}
						}
					}
					if($mismatch && !$superset) {
#print LOG "=====\n" unless $found;
#print LOG "        $compact_segment{$original_compact_segment}\n";
						my ($wire_index, $tile_index) = split "@", $tilewire;
						# look up the instances of the segment reference
						my @original_tile_anchors = split ",", $compact_segment{$original_compact_segment};
						# iterate over the anchors and look for this tilewire in the segment reference
						foreach my $original_tile_anchor (@original_tile_anchors) {
							# determine the appropriate anchor for the current tilewire
							my $tile_offset = $tile_index - $original_tile_anchor;
							# determine whether this instance contains the current tilewire
							if($original_compact_segment =~ /\b$wire_index\@$tile_offset\b/) {
#print LOG "        found original compact segment anchored at $original_tile_anchor in $compact_segment{$original_compact_segment}\n";
print LOG "    merging $original_tile_anchor:$original_compact_segment and $anchor_tile_index:$compact_segment ";
								# expand the original compact segment
								my @exhaustive_segment = ();
								foreach my $original_tilewire (split ",", $original_compact_segment) {
									# extract the relative tilewires
									my ($original_wire_index, $original_tile_offset) = split "@", $original_tilewire;
									# and push the anchor tilewire into the original segment
									push @exhaustive_segment, "$original_wire_index\@" . ($original_tile_offset + $original_tile_anchor);
								}
								# now add all of the current segment
								push @exhaustive_segment, @segment;
								# and condense it back down
								my %exhaustive_segment = utils::array_to_hash(@exhaustive_segment);
								my @complete_segment = sort order_segment keys %exhaustive_segment;
#print LOG "        " . (join ",", @complete_segment) . "\n";
								# now replace the current segment, and compact it again
								@segment = @complete_segment;
								($anchor_tile_index, $compact_segment) = compact_segment(@segment);
								# remove the original anchor
#print LOG "        removing original anchor $anchor_tile_index from $compact_segment{$original_compact_segment}\n";
								utils::remove_array_entry($original_tile_anchor, \@original_tile_anchors);
#print LOG "        anchors are now " . (join ",", @original_tile_anchors) . "\n";
								# delete the original compact segment (we've already saved the tile anchors)
								delete $compact_segment{$original_compact_segment};
								$compact_segment{$compact_segment} = (join ",", @original_tile_anchors) if scalar @original_tile_anchors;
#print LOG "        compact_segment $compact_segment is now " . (exists $compact_segment{$compact_segment} ? $compact_segment{$compact_segment} : "undefined") . "\n";
								# remove the compact segment itself
#print LOG "        removing compact segment $original_compact_segment\n";
								utils::remove_array_entry($original_compact_segment, \@compact_segments);
								# and be sure to point from the segment wires to the new compact segment
								foreach my $tilewire (@segment) {
									$original_compact_segment{$tilewire} = $compact_segment;
								}
								# mark the merge as successful
								$found = true;
								last;
							} else {
#print LOG "        didn't find $wire_index\@$tile_offset in $original_compact_segment with anchor $original_tile_anchor in $compact_segment{$original_compact_segment}\n";
							}
						}
print LOG "    WARNING: failed to find original compact segment\n" unless $found;
print LOG "into $anchor_tile_index:$compact_segment\n" if $found;
						last if $found;
					}
					last if $found;
				}
				# add the anchor tile to the compact segment instance list
				if(exists $compact_segment{$compact_segment}) {
#if($compact_segment{$compact_segment} =~ /\b$anchor_tile_index\b/) {
#	print LOG "WARNING: reinserting $anchor_tile_index into $compact_segment{$compact_segment} for $compact_segment\n";
#}
					$compact_segment{$compact_segment} .= ",$anchor_tile_index";
					#print LOG "$compact_segment{$compact_segment}:$compact_segment\n";
#print LOG "NOTE: updating $compact_segment{$compact_segment}:$compact_segment\n" if $found;
				} else {
					$compact_segment{$compact_segment} = $anchor_tile_index;
					push @compact_segments, $compact_segment;
#print LOG "NOTE: defining $compact_segment{$compact_segment}:$compact_segment\n" if $found;
					#print LOG "$anchor_tile_index:$compact_segment\n";
					# associate each segment tilewire with this compact segment
					foreach my $tilewire (@segment) {
						$original_compact_segment{$tilewire} = $compact_segment;
					}
				}

			# process pip statements
			} elsif($line =~ /^\s*\(pip (\w+) (\w+) (==|=>|=-|->) (\w+)( \(_ROUTETHROUGH-(\S+)-(\S+) (\S+)\))?\)\s*$/) {
				# ==   Bidirectional, unbuffered
				# =>   Bidirectional, buffered in one direction
				# =-   Bidirectional, buffered in both directions
				# ->   Directional, buffered
				# extract the variables
				my ($pip_tile_name, $wire_name_1, $direction, $wire_name_2) = ($1, $2, $3, $4);
				die "Pip tile name $pip_tile_name does not match current tile name $tile_name.\n" unless $pip_tile_name eq $tile_name;
				# determine the arc directionality
				die "Unsupported pip direction $direction for \"$line\".\n" if $direction eq "=>";
				#die "Unsupported pip direction $direction for \"$line\".\n" if $direction =~ /(==|=>)/;
				# define and detect the arcs
				my $forward_arc = "$wire_name_1>$wire_name_2";
				my $reverse_arc = "$wire_name_2>$wire_name_1";
				my ($defined_forward, $defined_reverse) = (true, $direction =~ /^(=-|==)$/);
				# determine whether any of these are irregular
				if($defined_forward && exists $$irregular_arcs_ref{$forward_arc}) {
					my $wire_index_1 = $$wire_name_to_wire_index_ref{$wire_name_1};
					my $wire_index_2 = $$wire_name_to_wire_index_ref{$wire_name_2};
					my $irregular_arc = "$wire_index_1,$wire_index_2";
					unless(exists $irregular_arcs{$irregular_arc}) {
						push @{$tile_irregular_arcs[$tile_index]}, $irregular_arc;
						print LOG "    irregular arc: $wire_name_1 > $wire_name_2 ($wire_index_1 > $wire_index_2)\n";
						$irregular_arcs{$irregular_arc} = undef;
					}
				}
				if($defined_reverse && exists $$irregular_arcs_ref{$reverse_arc}) {
					my $wire_index_1 = $$wire_name_to_wire_index_ref{$wire_name_1};
					my $wire_index_2 = $$wire_name_to_wire_index_ref{$wire_name_2};
					my $irregular_arc = "$wire_index_2,$wire_index_1";
					unless(exists $irregular_arcs{$irregular_arc}) {
						push @{$tile_irregular_arcs[$tile_index]}, $irregular_arc;
						print LOG "    irregular arc: $wire_name_2 > $wire_name_1 ($wire_index_2 > $wire_index_1)\n";
						$irregular_arcs{$irregular_arc} = undef;
					}
				}

			# process tile summaries
			} elsif($line =~ /^\s*\(tile_summary /) {
				# determine which tile wires were not accounted for
				my @unaccounted_wires = sort order_numeric keys %unaccounted_wires;
				$tile_unaccounted_wires[$tile_index] = \@unaccounted_wires;
				foreach my $unaccounted_wire (@unaccounted_wires) {
					print LOG "    undefined wire: $::tile_type_wire_index[$tile_type_index][$unaccounted_wire] ($unaccounted_wire)\n";
				}

			# we appropriately avoid processing primitive_def conn statements in this stage
			} elsif($line =~ /^\s*\(conn (\S+) (\S+) (<==|==>) (\S+) (\S+)\)\s*$/) {

			# catch any lines that were skipped because of bad regular expressions
			} elsif($line =~ /^\s*\((tiles|tile|primitive_site|pinwire|wire|conn|pip|tile_summary) /) {
				# this has to be treated as a fatal error
				die "Failed to process line \"$line\".";

			}
			
		}
		$feedback::out_ref->flush();

		# count the number of (non-unique, non-trivial) segments that we actually saw
		my $cumulative_segment_count = 0;
		foreach my $segment (keys %segment) {
			$cumulative_segment_count += $segment{$segment};
		}
		# write statistics
		print "\n";
		print LOG "# " . $device_wire_count . " device_wire_count\n";
		print LOG "# " . $cumulative_segment_count . " non-trivial cumulative segments\n";
		#print "    # " . $cumulative_segment_count . " cumulative segments\n";
		print LOG "# " . ($trivial_segments + scalar keys %segment) . " unique segments ($trivial_segments trivial + " 
			. (scalar keys %segment) . " real)\n";
		print "    # " . ($trivial_segments + scalar keys %segment) . " unique segments ($trivial_segments trivial + " 
			. (scalar keys %segment) . " real)\n";
		print LOG "# " . (scalar keys %compact_segment) . " compacted segments\n";
		print "    # " . (scalar keys %compact_segment) . " compacted segments\n";
		print LOG "# " . (scalar @primitive_sites) . " sites\n";
		print "    # " . (scalar @primitive_sites) . " sites\n";
		print LOG "\n";

		# write the database header
		print DIGEST "TORC";
		print DIGEST pack "C4", (@::format_number); # database format number
		print DIGEST pack "N", $::build_number; # database build number

		# write the version header
		print DIGEST ">>>>Version >>>>";
		print DIGEST pack "C4", (@::version_number); # ISE version number
		print DIGEST pack "na*", (length $::version_string, $::version_string); # ISE version string

		# write the device family
		print DIGEST ">>>> Family >>>>";
		print DIGEST pack "na*", length $architecture, $architecture;

		# write the speed grades
		print LOG "speeds:\n";
		print DIGEST ">>>> Speeds >>>>";
		# write the speed grade count
		my @speeds = split ",", $::device_speeds{$device};
		print DIGEST pack "n", scalar @speeds;
		foreach my $speed (@speeds) {
			print DIGEST pack "na*", (length $speed, $speed);
		}
		print LOG "$::device_speeds{$device}\n";

		# write the packages
		print LOG "packages:\n";
		print DIGEST ">>>>Packages>>>>";
		# write the package count
		my $packages = $::device_packages{$device};
		print DIGEST pack "n", scalar keys %$packages;
		foreach my $package (sort keys %$packages) {
			my $entries = ${$packages}{$package};
			# write the package name and site count
			print DIGEST pack "na*", (length $package, $package);
			print DIGEST pack "N", scalar @$entries;
			foreach my $entry (@$entries) {
				# write the site index, site name, and bonding flag
				my ($site_index, $site_name, $site_bonding) = split ",", $entry;
				print DIGEST pack "N", $site_index;
				print DIGEST pack "n", $site_bonding;
				print DIGEST pack "na*", (length $site_name, $site_name);
			}
		}

		# write the section header
		print DIGEST ">>>>TileMap >>>>";
		# write the tile count, row count, and column count
		print DIGEST pack "Nnn", $tile_count, $row_count, $col_count;
		# iterate through the tiles
		foreach my $tile (@::tiles) {
			# look up the tile information
			my ($tile_index, $tile_row, $tile_col, $tile_name, $tile_type_index, $tile_site_count) = split ",", $tile;
			# write the info to the digest
			print DIGEST pack "nnnna*", $tile_type_index, $tile_row, $tile_col, length $tile_name, $tile_name;
		}

		# write out the extra tile wire information
		print DIGEST ">>>>TileSegs>>>>";
		print DIGEST (pack "N", scalar @::tiles);
		# iterate through the tiles
		foreach my $tile (@::tiles) {
			# look up the tile information
			my ($tile_index, $tile_row, $tile_col, $tile_name, $tile_type_index, $tile_site_count) = split ",", $tile;
			# look up the tile wires
			$wire_name_to_wire_index_ref = $::tile_type_wire_name_to_wire_index{$tile_type_index};
			my $tile_type_wire_count = scalar keys %$wire_name_to_wire_index_ref;
			# look up the unaccounted wires
#print "irregular arcs undefined for $tile\n" unless defined $tile_unaccounted_wires[$tile_index];
			my @unaccounted_wires = @{$tile_unaccounted_wires[$tile_index]};
			# write the total and unaccounted wire counts for this tile
			print DIGEST (pack "nn", ($tile_type_wire_count, scalar @unaccounted_wires));
			# write each of the unaccounted wires
			print DIGEST (pack "n*", @unaccounted_wires);
		}

		# write out the segment information
		print DIGEST ">>>>Segments>>>>";
		print DIGEST (pack "N", scalar @compact_segments);
		# iterate through the compacted segments
		my $index = 0;
		foreach my $compact_segment (@compact_segments) {
			# extract each wire index and tile offset separately
			my @wires_and_tiles = split /[,@]/, $compact_segment;
			# extract the tile anchors
			my @tile_anchors = sort order_numeric utils::remove_array_duplicates(split ",", $compact_segment{$compact_segment});
			$compact_segment{$compact_segment} = join ",", @tile_anchors;
			# log this information
			print LOG "#$index: $compact_segment{$compact_segment}:$compact_segment\n";
			# write the relative wires that make up the segment
			print DIGEST (pack "n", (scalar @wires_and_tiles) / 2);
			while(scalar @wires_and_tiles) { print DIGEST (pack "nN", shift @wires_and_tiles, shift @wires_and_tiles); }
			# write the tile anchors where this compact segment is instantiated
			print DIGEST (pack "NN*", ((scalar @tile_anchors, @tile_anchors)));
			$index++;
		}

		# write out the irregular arcs
		print DIGEST ">>>>IrrgArcs>>>>";
		print DIGEST (pack "N", scalar @::tiles);
		# iterate through the tiles
		foreach my $tile (@::tiles) {
			# look up the tile information
			my ($tile_index, $tile_row, $tile_col, $tile_name, $tile_type_index, $tile_site_count) = split ",", $tile;
			# look up the tile's irregular arcs
			my @irregular_arcs = sort order_irregular_arcs @{$tile_irregular_arcs[$tile_index]};
			# write the irregular arc count for this tile
			print DIGEST (pack "n", scalar @irregular_arcs);
			# write each of the irregular arcs
			foreach my $irregular_arc (@irregular_arcs) {
				print DIGEST (pack "nn", (split ",", $irregular_arc));
			}
		}

		# write out the primitive sites
		print DIGEST ">>>>  Sites >>>>";
		print DIGEST (pack "N", scalar @primitive_sites);
		# iterate through the primitive sites
		foreach my $site (@primitive_sites) {
			# look up the site information
			my ($site_name, $primitive_type_index, $tile_index, $site_bonding, $pinwire_map) = split ",", $site;
			# write the site information
			print DIGEST pack "na*", (length $site_name, $site_name);
			print DIGEST pack "nNnn", ($primitive_type_index, $tile_index, $site_bonding, $pinwire_map);
			print LOG "$site_name,$primitive_type_index,$tile_index,$site_bonding,$pinwire_map\n";
		}

		# iterate through the tile types and write the arcs
		for($index = 0; $index < scalar keys %::tile_type_index_count; $index++) {
			# look up the tile type and the number of times that it appears
		}

		# close everything out
		print "    done.\n";
		close XDLRC;
		close DIGEST;
		close LOG;

		# generate the compressed database
		my $database_file_name = "$digest_file_name.db";
		print `gzip -c --best $digest_file_name > $database_file_name`;
		print `rm $digest_file_name`;
	}

	# write the architecture database
	sub write_architecture($) {

		# extract the parameters
		my $architecture = shift;

		# open the xdlrc file
		my $log_file_name = "$architecture.log";
		my $digest_file_name = "../$architecture";
		open(LOG, ">$log_file_name") or die "Unable to write $log_file_name: $!\n";
		open(DIGEST, ">$digest_file_name") or die "Unable to write $digest_file_name: $!\n";
		print "Processing $architecture ...  ";

		# write the database header
		print DIGEST "TORC";
		print DIGEST pack "C4", (@::format_number); # database format number
		print DIGEST pack "N", $::build_number; # database build number

		# write the version header
		print DIGEST ">>>>Version >>>>";
		print DIGEST pack "C4", (@::version_number); # ISE version number
		print DIGEST pack "na*", (length $::version_string, $::version_string); # ISE version string

		# write the tile types

		# write the section header
		print DIGEST ">>>>TileType>>>>";
		# write the tile type count
		my $tile_type_count = scalar @::tile_type_index_to_tile_type_name;
		print DIGEST pack "n", $tile_type_count;
		# loop through the tile types
		for(my $tile_type_index = 0; $tile_type_index < $tile_type_count; $tile_type_index++) {
			# look up the tile type name
			my $tile_type_name = $::tile_type_index_to_tile_type_name[$tile_type_index];
			# write the name to the digest
			print DIGEST pack "na*", (length $tile_type_name, $tile_type_name);
		}

		# write the tile nodes

		# write the section header
		print DIGEST ">>>>TileNode>>>>";
		# write the tile type count
		print DIGEST pack "n", $tile_type_count;
		# loop through the tile types
		for(my $tile_type_index = 0; $tile_type_index < $tile_type_count; $tile_type_index++) {
			# look up the tile type name
			my $tile_type_name = $::tile_type_index_to_tile_type_name[$tile_type_index];
#print "$tile_type_name: \n";
			# look up the tile type data
			my $tile_type_wire_index = $::tile_type_wire_index[$tile_type_index];
			my $tile_type_wire_name_to_wire_index = $::tile_type_wire_name_to_wire_index{$tile_type_index};
			my $tile_type_wire_index_to_wire_name = $::tile_type_wire_index_to_wire_name{$tile_type_index};
			my $regular_arcs = $::tile_type_regular_arcs[$tile_type_index];
			my $irregular_arcs = $::tile_type_irregular_arcs[$tile_type_index];
			my $routethrough_arcs = $::tile_type_routethrough_arcs[$tile_type_index];
			my $attributes_ref = $::tile_type_wire_index_attributes{$tile_type_index};
			# collect the irregular arcs
			my %irregular_arc = ();
			foreach my $irregular_arc (keys %$irregular_arcs) {
				my ($source_wire_name, $sink_wire_name) = split ">", $irregular_arc;
				my $source_wire_index = $$tile_type_wire_name_to_wire_index{$source_wire_name};
				my $sink_wire_index = $$tile_type_wire_name_to_wire_index{$sink_wire_name};
				$irregular_arc{$source_wire_index}{$sink_wire_index} = undef;
			}
			# collect the routethrough arcs
			my %routethrough_arc = ();
			foreach my $routethrough_arc (keys %$routethrough_arcs) {
				my ($source_wire_index, $sink_wire_index) = split ">", $routethrough_arc;
				if(exists $irregular_arc{$source_wire_index}{$sink_wire_index}) {
my $message = "    WARNING: found irregular routethrough arc "
	. $$tile_type_wire_index_to_wire_name{$source_wire_index} . ">" 
	. $$tile_type_wire_index_to_wire_name{$sink_wire_index} . "\n";
print LOG $message;
print STDERR $message;
					delete $irregular_arc{$source_wire_index}{$sink_wire_index};
					next;
				}
				$routethrough_arc{$source_wire_index}{$sink_wire_index} = undef;
			}
			# collect the regular arcs
			my %regular_arc = ();
			foreach my $regular_arc (keys %$regular_arcs) {
				my ($source_wire_index, $sink_wire_index) = split ">", $regular_arc;
				next if exists $irregular_arc{$source_wire_index}{$sink_wire_index};
				next if exists $routethrough_arc{$source_wire_index}{$sink_wire_index};
				$regular_arc{$source_wire_index}{$sink_wire_index} = undef;
			}
			# write the wire count
			my $wire_count = scalar @$tile_type_wire_index;
			print DIGEST pack "n", $wire_count;
			# loop through the nodes in the tile type
			for(my $node_index = 0; $node_index < $wire_count; $node_index++) {
				# look up the node name
				my $node_name = $$tile_type_wire_index[$node_index];
				# write the node attributes
				my $node_attributes = $$attributes_ref{$node_index} || 0;
#print "\t$node_name: $node_attributes\n" if $node_attributes;
				print DIGEST pack "n", $node_attributes;
				# write the node name
				print DIGEST pack "na*", (length $node_name, $node_name);
				# write the fake arcs
				print DIGEST pack "n", 0;
				# write the regular arcs
				my @regular_arcs = sort keys %{$regular_arc{$node_index}};
				print DIGEST pack "nn*", (scalar @regular_arcs, @regular_arcs);
				# write the irregular arcs
				my @irregular_arcs = sort keys %{$irregular_arc{$node_index}};
				print DIGEST pack "nn*", (scalar @irregular_arcs, @irregular_arcs);
				# write the routethrough arcs
				my @routethrough_arcs = sort keys %{$routethrough_arc{$node_index}};
				print DIGEST pack "nn*", (scalar @routethrough_arcs, @routethrough_arcs);
#print "\t$node_name: " . (join ", ", @irregular_arcs) . "\n";
			}
		}
#print "\n";

		# write the primitive site definitions

		# write the section header
		print LOG "primitives:\n";
		print DIGEST ">>>>PrimDefs>>>>";
		# write the site definition count
		print DIGEST pack "n", scalar keys %::primitive_pins;
		# iterate through the site definitions
		my $index = 0;
		foreach my $primitive_type_name (@::primitive_type_index_to_primitive_type_name) {
			# write the site type name
			print DIGEST pack "na*", (length $primitive_type_name, $primitive_type_name);
			print LOG $index++ . " \"$primitive_type_name\":\n";
			# look up the pin information
			my @pins = @{$::primitive_pins{$primitive_type_name}};
			# write the pin count
			print DIGEST pack "N", scalar @pins;
			# iterate through the pins
			my @log_output = ();
			foreach my $pin (@pins) {
				# extract the pin information
				my $pin_direction;
				my ($pin_name, $pin_direction_string) = split ":", $pin;
				$pin_direction = INPUT if $pin_direction_string =~ /input/i;
				$pin_direction = OUTPUT if $pin_direction_string =~ /output/i;
				# write the pin information
				print DIGEST pack "nna*", ($pin_direction, length $pin_name, $pin_name);
				push @log_output, "$pin_name:" . ($pin_direction == INPUT ? "input" : "output");
			}
			print LOG "\t" . (join ",", @log_output) . "\n";
			# look up the element information
			my @elements = @{$::primitive_elements{$primitive_type_name}};
			my %element_name_to_index = ();
			my %element_pin_name_to_index = ();
			# write the element count
			print DIGEST pack "N", scalar @elements;
			# iterate through the elements
			my $element_index = 0;
			foreach my $block (@elements) {
				my ($element, $bel, $pins, $cfgs) = split /\|/, $block;
				my @pins = split ",", $pins;
				my @cfgs = split ",", $cfgs;
				$element_name_to_index{$element} = $element_index++;
				# write the element name and BEL status
				print DIGEST pack "na*n", (length $element, $element, $bel);
				print LOG "\t$element" . ($bel ? " #BEL" : "") . ": $cfgs\n";
				# write the element pin count
				print DIGEST pack "N", scalar @pins;
				# iterate through the pins
				@log_output = ();
				my $element_pin_index = 0;
				foreach my $pin (@pins) {
					# extract the pin information
					my $pin_direction;
					my ($pin_name, $pin_direction_string) = split ":", $pin;
					$element_pin_name_to_index{$element}{$pin_name} = $element_pin_index++;
					$pin_direction = INPUT if $pin_direction_string =~ /input/i;
					$pin_direction = OUTPUT if $pin_direction_string =~ /output/i;
					# write the pin information
					print DIGEST pack "nna*", ($pin_direction, length $pin_name, $pin_name);
					push @log_output, "$pin_name:" . ($pin_direction == INPUT ? "input" : "output");
				}
				print LOG "\t\t" . (join ",", @log_output) . "\n";
				# write the element config value count
				print DIGEST pack "N", scalar @cfgs;
				# iterate through the config values
				@log_output = ();
				foreach my $cfg (@cfgs) {
					print DIGEST pack "na*", (length $cfg, $cfg);
					push @log_output, "$cfg";
				}
			}
			# look up the connection information
			my @conns = @{$::primitive_conns{$primitive_type_name}};
			# write the connection count
			print DIGEST pack "N", scalar @conns;
			# iterate through the connections
			foreach my $conn (@conns) {
				my @items = split ":", $conn;
				print LOG "\t$conn\n";
				# write the connection source
				print DIGEST pack "n", 1; # we're assuming a single source at present
				my ($element_name, $pin_name) = (shift @items, shift @items);
				print DIGEST pack "N", ($element_name_to_index{$element_name});
				print DIGEST pack "N", ($element_pin_name_to_index{$element_name}{$pin_name});
				# write the connection sinks
				print DIGEST pack "n", (scalar @items) / 2;
				while(scalar @items) {
					($element_name, $pin_name) = (shift @items, shift @items);
					print DIGEST pack "N", ($element_name_to_index{$element_name});
					print DIGEST pack "N", ($element_pin_name_to_index{$element_name}{$pin_name});
				}
			}
		}

		# write the primitive pinwire maps

		# write the section header
		print LOG "pin maps:\n";
		print DIGEST ">>>>Pin Maps>>>>";
		# write the pinwire map count
		print DIGEST pack "n", scalar keys %::pinwire_map;
		# iterate through the pinwire maps
		$index = 0;
		foreach my $pinwire_map (@::pinwire_map) {
			# extract the pinwires
			my @map_pinwires = split ",", $pinwire_map;
			print DIGEST pack "Nn*", (scalar @map_pinwires, @map_pinwires);
			print LOG $index++ . ":$pinwire_map\n";
		}

		# close everything out
		print "    done.\n";
		close DIGEST;
		close LOG;

		# generate the compressed database
		my $database_file_name = "$digest_file_name.db";
		print `gzip -c --best $digest_file_name > $database_file_name`;
		print `rm $digest_file_name`;
	}

#	sub process_arcs($) {
#		my $architecture = shift;
#
#		print "    Checking arcs ...  ";
#		# iterate through the tile types and check the arcs
#		for(my $tile_type_index = 0; $tile_type_index < scalar keys %::tile_type_index_count; $tile_type_index++) {
#			# iterate over all of the arcs
#			my $arc_ref = $::tile_type_arc{$tile_type_index};
#			foreach my $source_wire_index (sort keys %$arc_ref) {
#				my $sinks_ref = $$arc_ref{$source_wire_index};
#				foreach my $sink_wire_index (sort keys %$sinks_ref) {
#					# compare the number of times the arc appears to the number of times the tile type appears
#					if($$sinks_ref{$sink_wire_index} != $::tile_type_index_count{$tile_type_index}) {
#						
#
#
#						print "$::tile_type_index_to_tile_type_name[$tile_type_index] ($tile_type_index) arc mismatch: "
#						. "tile type instance count $::tile_type_index_count{$tile_type_index} does not match "
#						. "arc $::tile_type_wire_index[$tile_type_index][$source_wire_index] -> "
#						. "$::tile_type_wire_index[$tile_type_index][$sink_wire_index] instantiation count "
#						. "$$sinks_ref{$sink_wire_index}\n";
#						print ARCH_LOG "$::tile_type_index_to_tile_type_name[$tile_type_index] ($tile_type_index) arc mismatch: "
#						. "tile type instance count $::tile_type_index_count{$tile_type_index} does not match "
#						. "arc $::tile_type_wire_index[$tile_type_index][$source_wire_index] -> "
#						. "$::tile_type_wire_index[$tile_type_index][$sink_wire_index] instantiation count "
#						. "$$sinks_ref{$sink_wire_index}\n";
#						#						print ARCH_LOG "arc mismatch: $::tile_types[$tile_type_index] ($::tile_type_index_count{$tile_type_index}) != $source_wire_index -> $sink_wire_index ($$sinks_ref{$sink_wire_index})\n";
#					} else {
##						print ARCH_LOG "arc: $::tile_types[$tile_type_index] ($::tile_type_index_count{$tile_type_index}) == $source_wire_index -> $sink_wire_index ($$sinks_ref{$sink_wire_index})\n";
#					}
#				}
#			}
#		}
#		print "done.\n";
#		
#	}

	# process all of the wiring and pips for the specified architecture and devices
	sub process_wiring($@) {
		my $architecture = shift;
		my @devices = @_;

		# reset variables
		%::tile_type_index_count = ();
		#%::tile_type_arc = ();

		# open the architecture log file
		my $architecture_log_file_name = "$architecture.log";
		open(ARCH_LOG, ">$architecture_log_file_name") or die "Unable to write $architecture_log_file_name: $!\n";

		# read the package digest
		read_package_digest($architecture);

		# read the architecture digest
		read_architecture_digest($architecture);

		# extract the tile type information
#		extract_tile_types($architecture);

		# iterate through the devices
		my $device_num = 1;
		my $device_count = scalar @devices;
		foreach my $device (@devices) {

			# read the device digest
			read_device_digest($device);

			# now iterate through the device and extract the wires and segments
			extract_segments($architecture, $device, $device_num++, $device_count);

		}

		# write the architecture information
		write_architecture($architecture);

		# process the arcs for the architecture
		#process_arcs($architecture);

		# close the architecture log
		close ARCH_LOG;

	}

	# generate XDLRC with exhaustive connectivity for the specified architecture and devices
	# this requires ISE partgen and xdl with support for the requested architecture and devices
	sub generate_xdlrc($@) {
		my $architecture = shift;
		my @devices = @_;

		# determine the version of the tools in use
		my $command0 = "xdl --version";
		my $result = `$command0`;
		$result =~ /^(Release ([\.\d]+) -.*)$/m;
		my ($version_string, $version_number) = ($1, $2);
		my $version = "# $version_number \"$version_string\"\n";


		# iterate over all requested devices that are supported by ISE
		my $partgen_devices = `partgen -i`;
		foreach my $device (@_) {
			next unless $partgen_devices =~ /^$device\s+/m;
			print "Processing $device...\n";

			# derive the device name that xdl expects (this may not be strictly necessary)
			my $xilinx_device;
			($xilinx_device = lc $device) =~ s/^xc/x/;

			# create the exported and compressed file names
			my $xdlrc_file_name = "$device.conns.xdlrc";
			my $xdlrc_raw_file_name = "$xdlrc_file_name.raw";
			my $xdlrc_version_file_name = "$xdlrc_file_name.version";
			my $xdlrc_gz_file_name = "$xdlrc_file_name.gz";

			# skip this device if it has already been built
			next if -e $xdlrc_gz_file_name;

			# generate the XDLRC
			my $command1 = "xdl -report -pips -all_conns $xilinx_device $xdlrc_raw_file_name "
				. ">& /dev/null";
			$command1 =~ s/ >.*// if $ENV{"HOST"} eq "tag-b"; # what's wrong with tag-b?
			print "    $command1\n";
			`$command1`;
			# insert the version information into the XDLRC
			my $meta_version = quotemeta $version;
			my $command2 
				= "echo $meta_version > $xdlrc_version_file_name;"
				. "cat $xdlrc_version_file_name $xdlrc_raw_file_name > $xdlrc_file_name;"
				. "rm $xdlrc_version_file_name $xdlrc_raw_file_name";
			`$command2`;
			# gzip the XDLRC
			my $command3 = "gzip -9 $xdlrc_file_name";
			print "    $command3\n";
			`$command3`;
		}

	}

	# preprocess packages for the specified architecture and devices
	# this requires ISE partgen and xdl with support for the requested architecture and devices
	sub preprocess_packages($@) {
		my $architecture = shift;
		my @devices = @_;

		# determine the version of the tools in use
		my $command0 = "xdl --version";
		my $result = `$command0`;
		$result =~ /^(Release ([\.\d]+) -.*)$/m;
		my ($version_string, $version_number) = ($1, $2);
		my $version = "# $version_number \"$version_string\"\n";

		# extract all device, speed grade, and package information from partgen
		my %devices;
		my $partgen_devices = `partgen -i`;
		$partgen_devices =~ /^Copyright [^\n]+\n(.*)/ms;
		my @entries = split /^(?=\S)/m, $1;
		foreach my $entry (@entries) {
			next unless $entry =~ /^(\S+)\s+SPEEDS:\s+([^\n]+)\s+(.*?)\s+$/s;
			my ($device, $speeds, $packages) = ($1, $2, $3);
			$speeds =~ s/\s*\(Minimum speed data available\)\s*//;
			$speeds =~ s/\s+/,/g;
			$packages =~ s/\s+/,/g;
			$devices{$device} = "$packages:$speeds";
		}

		# iterate over all requested devices that are supported by ISE
		my $digest = "";
		foreach my $device (@_) {
			next unless $partgen_devices =~ /^$device\s+/m;
			my $devices = $devices{$device};
			my ($packages, $speeds) = split ":", $devices;
			my @packages = split ",", $packages;
			$digest .= "$device:$speeds:";

			# iterate over all the available packages
			foreach my $package (@packages) {
				next unless $partgen_devices =~ /^$device\s+/m;
				print "Processing $device$package...\n";

				# derive the device name that xdl expects (this may not be strictly necessary)
				my $xilinx_device;
				($xilinx_device = lc $device) =~ s/^xc/x/;

				# create the exported and compressed file names
				my $xdlrc_file_name = "$device$package.xdlrc";
				my $xdlrc_gz_file_name = "$xdlrc_file_name.gz";

				if(!-e $xdlrc_gz_file_name) {
					# generate the XDLRC
					my $command1 = "xdl -report $xilinx_device$package $xdlrc_file_name >& /dev/null";
					$command1 =~ s/ >.*// if $ENV{"HOST"} eq "tag-b"; # what's wrong with tag-b?
					print "    $command1\n";
					`$command1`;
					# inject the version information
					`sed -i -e '1i$version' $xdlrc_file_name`;
					# gzip the XDLRC
					my $command2 = "gzip -9 $xdlrc_file_name";
					print "    $command2\n";
					`$command2`;
				}

				# identify all of the primitive sites in the xdlrc
				my %sites = ();
				my $command3 = "zgrep primitive_site $xdlrc_gz_file_name";
				my $primitive_sites = `$command3`;
				my $site_index = 0;
				foreach my $primitive_site (split /\n/, $primitive_sites) {
					$primitive_site =~ /primitive_site (\S+) (\S+) (\S+\b)/;
					my ($site_name, $site_type_name, $bonding) = ($1, $2, $3);
					if($bonding eq "bonded" or $bonding eq "unbonded") {
						$sites{$site_index} = "$site_name,$bonding";
					}
					$site_index++;
				}

				# add this package to the digest
				$digest .= "$package;";
				foreach my $site_index (sort { $a <=> $b } keys %sites) {
					$digest .= "$site_index,$sites{$site_index};";
				}
				chop $digest;
				$digest .= ":";
			}
			chop $digest;
			$digest .= "\n";

		}

		# write the package digest
		if(length $digest) {
			my $package_digest_file_name = "$architecture" . ".packages.digest";
			open(PACKAGES, ">$package_digest_file_name") or die "Unable to write $package_digest_file_name: $!\n";
			print PACKAGES $digest;
			close PACKAGES;
		}

	}

	# extract the command line arguments
	my ($xdlrc, $packages, $preprocess, $process, $development, $tagm12, $isirrex, $virtex, 
		$virtexe, $virtex2, $virtex2p, $virtex4, $virtex5, $virtex6, $virtex6l, $virtex7, 
		$virtex7l, $kintex7, $kintex7l, $artix7, $zynq7000, $spartan3e, $spartan6, 
		$spartan6l) 
		= (false, false, false, false, false, false, false, false, false, false, false, 
		false, false, false, false, false, false, false, false);
	foreach my $arg (@ARGV) {
		$xdlrc = true if $arg =~ /\bxdlrc\b/i;
		$packages = true if $arg =~ /\bpackages\b/i;
		$preprocess = true if $arg =~ /\bpreprocess\b/i;
		$process = true if $arg =~ /\bprocess\b/i;
		$development = true if $arg =~ /\bdevelopment\b/i;
		$tagm12 = true if $arg =~ /\btagm12\b/i;
		$isirrex = true if $arg =~ /\bisirrex\b/i;
		$virtex = true if $arg =~ /\bvirtex\b/i;
		$virtexe = true if $arg =~ /\bvirtexe\b/i;
		$virtex2 = true if $arg =~ /\bvirtex2\b/i;
		$virtex2p = true if $arg =~ /\bvirtex2p\b/i;
		$virtex4 = true if $arg =~ /\bvirtex4\b/i;
		$virtex5 = true if $arg =~ /\bvirtex5\b/i;
		$virtex6 = true if $arg =~ /\bvirtex6\b/i;
		$virtex6l = true if $arg =~ /\bvirtex6l\b/i;
		$virtex7 = true if $arg =~ /\bvirtex7\b/i;
		$virtex7l = true if $arg =~ /\bvirtex7l\b/i;
		$kintex7 = true if $arg =~ /\bkintex7\b/i;
		$kintex7l = true if $arg =~ /\bkintex7l\b/i;
		$artix7 = true if $arg =~ /\bartix7\b/i;
		$zynq7000 = true if $arg =~ /\bzynq7000\b/i;
		$spartan3e = true if $arg =~ /\bspartan3e\b/i;
		$spartan6 = true if $arg =~ /\bspartan6\b/i;
		$spartan6l = true if $arg =~ /\bspartan6l\b/i;
	}

	# generate device XDLRC as requested
	generate_xdlrc("Development", @devices::development) if $xdlrc && $development;
	generate_xdlrc("Virtex", @devices::virtex) if $xdlrc && $virtex;
	generate_xdlrc("VirtexE", @devices::virtexe) if $xdlrc && $virtexe;
	generate_xdlrc("Virtex2", @devices::virtex2) if $xdlrc && $virtex2;
	generate_xdlrc("Virtex2P", @devices::virtex2p) if $xdlrc && $virtex2p;
	generate_xdlrc("Virtex4", @devices::virtex4) if $xdlrc && $virtex4;
	generate_xdlrc("Virtex5", @devices::virtex5) if $xdlrc && $virtex5;
	generate_xdlrc("Virtex6", @devices::virtex6) if $xdlrc && $virtex6;
	generate_xdlrc("Virtex6L", @devices::virtex6l) if $xdlrc && $virtex6l;
	generate_xdlrc("Virtex7", @devices::virtex7) if $xdlrc && $virtex7;
	generate_xdlrc("Virtex7L", @devices::virtex7l) if $xdlrc && $virtex7l;
	generate_xdlrc("Kintex7", @devices::kintex7) if $xdlrc && $kintex7;
	generate_xdlrc("Kintex7L", @devices::kintex7l) if $xdlrc && $kintex7l;
	generate_xdlrc("Artix7", @devices::artix7) if $xdlrc && $artix7;
	generate_xdlrc("Zynq7000", @devices::zynq7000) if $xdlrc && $zynq7000;
	generate_xdlrc("Spartan3E", @devices::spartan3e) if $xdlrc && $spartan3e;
	generate_xdlrc("Spartan6", @devices::spartan6) if $xdlrc && $spartan6;
	generate_xdlrc("Spartan6L", @devices::spartan6l) if $xdlrc && $spartan6l;
	# preprocess device packages as requested
	preprocess_packages("Development", @devices::development) if $packages && $development;
	preprocess_packages("Virtex", @devices::virtex) if $packages && $virtex;
	preprocess_packages("VirtexE", @devices::virtexe) if $packages && $virtexe;
	preprocess_packages("Virtex2", @devices::virtex2) if $packages && $virtex2;
	preprocess_packages("Virtex2P", @devices::virtex2p) if $packages && $virtex2p;
	preprocess_packages("Virtex4", @devices::virtex4) if $packages && $virtex4;
	preprocess_packages("Virtex5", @devices::virtex5) if $packages && $virtex5;
	preprocess_packages("Virtex6", @devices::virtex6) if $packages && $virtex6;
	preprocess_packages("Virtex6L", @devices::virtex6l) if $packages && $virtex6l;
	preprocess_packages("Virtex7", @devices::virtex7) if $packages && $virtex7;
	preprocess_packages("Virtex7L", @devices::virtex7l) if $packages && $virtex7l;
	preprocess_packages("Kintex7", @devices::kintex7) if $packages && $kintex7;
	preprocess_packages("Kintex7L", @devices::kintex7l) if $packages && $kintex7l;
	preprocess_packages("Artix7", @devices::artix7) if $packages && $artix7;
	preprocess_packages("Zynq7000", @devices::zynq7000) if $packages && $zynq7000;
	preprocess_packages("Spartan3E", @devices::spartan3e) if $packages && $spartan3e;
	preprocess_packages("Spartan6", @devices::spartan6) if $packages && $spartan6;
	preprocess_packages("Spartan6L", @devices::spartan6l) if $packages && $spartan6l;
	# preprocess as requested
	preprocess_architecture("Development", @devices::development) if $preprocess && $development;
	preprocess_architecture("Virtex", @devices::virtex) if $preprocess && $virtex;
	preprocess_architecture("VirtexE", @devices::virtexe) if $preprocess && $virtexe;
	preprocess_architecture("Virtex2", @devices::virtex2) if $preprocess && $virtex2;
	preprocess_architecture("Virtex2P", @devices::virtex2p) if $preprocess && $virtex2p;
	preprocess_architecture("Virtex4", @devices::virtex4) if $preprocess && $virtex4;
	preprocess_architecture("Virtex5", @devices::virtex5) if $preprocess && $virtex5;
	preprocess_architecture("Virtex6", @devices::virtex6) if $preprocess && $virtex6;
	preprocess_architecture("Virtex6L", @devices::virtex6l) if $preprocess && $virtex6l;
	preprocess_architecture("Virtex7", @devices::virtex7) if $preprocess && $virtex7;
	preprocess_architecture("Virtex7L", @devices::virtex7l) if $preprocess && $virtex7l;
	preprocess_architecture("Kintex7", @devices::kintex7) if $preprocess && $kintex7;
	preprocess_architecture("Kintex7L", @devices::kintex7l) if $preprocess && $kintex7l;
	preprocess_architecture("Artix7", @devices::artix7) if $preprocess && $artix7;
	preprocess_architecture("Zynq7000", @devices::zynq7000) if $preprocess && $zynq7000;
	preprocess_architecture("Spartan3E", @devices::spartan3e) if $preprocess && $spartan3e;
	preprocess_architecture("Spartan6", @devices::spartan6) if $preprocess && $spartan6;
	preprocess_architecture("Spartan6L", @devices::spartan6l) if $preprocess && $spartan6l;
	# process as requested
	process_wiring("Development", @devices::development) if $process && $development;
	process_wiring("Virtex", @devices::virtex) if $process && $virtex;
	process_wiring("VirtexE", @devices::virtexe) if $process && $virtexe;
	process_wiring("Virtex2", @devices::virtex2) if $process && $virtex2;
	process_wiring("Virtex2P", @devices::virtex2p) if $process && $virtex2p;
	process_wiring("Virtex4", @devices::virtex4) if $process && $virtex4;
	process_wiring("Virtex5", @devices::virtex5) if $process && $virtex5;
	process_wiring("Virtex6", @devices::virtex6) if $process && $virtex6;
	process_wiring("Virtex6L", @devices::virtex6l) if $process && $virtex6l;
	process_wiring("Virtex7", @devices::virtex7) if $process && $virtex7;
	process_wiring("Virtex7L", @devices::virtex7l) if $process && $virtex7l;
	process_wiring("Kintex7", @devices::kintex7) if $process && $kintex7;
	process_wiring("Kintex7L", @devices::kintex7l) if $process && $kintex7l;
	process_wiring("Artix7", @devices::artix7) if $process && $artix7;
	process_wiring("Zynq7000", @devices::zynq7000) if $process && $zynq7000;
	process_wiring("Spartan3E", @devices::spartan3e) if $process && $spartan3e;
	process_wiring("Spartan6", @devices::spartan6) if $process && $spartan6;
	process_wiring("Spartan6L", @devices::spartan6l) if $process && $spartan6l;


sub __suppress_warnings__() {
	%::tile_type_unbuffered_arcs = ();
	print feedback::out;
	$feedback::out_ref->flush();
}

