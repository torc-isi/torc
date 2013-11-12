#!/bin/perl -W
# Perl script to extract memory address to bit address map information for ll files.
use strict;

use File::Spec;
use File::Basename;

# we expect a single input file name as an argument
die "Usage: $0 [file_name].ll\n" if scalar @ARGV != 1;
die "Input file $ARGV[0] does not exist on disk.\n" unless -s $ARGV[0];

my $input_filename = $ARGV[0];
# Get input file directory
my ($inputFileBase, $inputFileDir) = fileparse($input_filename);

# read the file contents
open(INPUT, $input_filename) or die "Unable to read $input_filename: $!\n";
my @lines = <INPUT>;
close INPUT;
chomp @lines;
print "Generating micro-bitstreams for LUT memory from $input_filename\n";
print "$inputFileBase: read " . (scalar @lines) . " lines\n";

# open the output file in format <architecture>-<SiteType>-<BEL>-<Config>.cbit
# For ROM bits of LUT, only first letter of BEL is used. Eg. for C6LUT, just C is used.
# Config name used is ROM. This is hard coded and has to match with Xdl2Bit.cpp
my $output_filename = $input_filename;
$output_filename =~ s/ll/cbit/;

# extract the data
# sample line: Bit 17156960 0x001000a0   1248 Block=SLICE_X0Y99 Rom=A:3
my $pattern = "\^Bit\\s+(\\d+)\\s+0x([0-9a-fA-F]+)\\s+(\\d+)\\s+Block=([A-Z0-9_]+)\\s+Ram=([A-D]):(\\d+)\$";

# map of map to store - LUT->Bitposition in ROM->Bitaddress
my %bit = ();

foreach my $line (@lines) {
	# match the line against the expected pattern
	if($line =~ /$pattern/) 
	{
		my ($offset, $hex_frame_address, $frame_bit_index, $blockname, $lut_id, $index) = ($1, $2, $3, $4, $5, $6);
	
		# extract the relevant fields if there is a match
		my $minor_address = hex substr $hex_frame_address, 6, 2; # frame address
		my $word_index = $frame_bit_index >> 5;
		my $bit_index = $frame_bit_index & 0x1F;
		$bit{$lut_id}{$index} = "$minor_address:$word_index:$bit_index";
	} else {
#		print "Line did not match\n";
	}
}

# print all the bit addresses in sequential order of memory address
my $count = pack "N", 1;
foreach my $lut ( keys %bit) {
	# one output file for each LUT
	my $lut_filename = $output_filename;
	$lut_filename =~ s/X/$lut/;
	open(OUTPUT, ">$lut_filename") or die "Unable to write $lut_filename: $!\n";

	# Number of bit addresses
	$count = pack "L", scalar keys %{$bit{$lut}};
	print OUTPUT "$count";

	# For each bit in ROM/memory
	foreach my $bit_position ( sort { $a <=> $b } keys %{$bit{$lut}} ) {
		# split the subfields apart
		my ($minor_address, $word_index, $bit_index) = split ":", $bit{$lut}{$bit_position};
		# pack the subfields into a single 32-bit word
		# 16 bits for frame index, 8 bits for word index, and 8 bits for bit index
		my $pack = pack "L", ((($minor_address & 0x3f) << 16) | (($word_index & 0xff) << 8) | $bit_index);
		print OUTPUT "$pack";
#		printf "%5d: %v02X\n", $bit_position, $pack;
	}
	close OUTPUT;
}

## ; Created by bitgen O.87xd at Tue Aug 21 10:29:06 2012
# ; Bit lines have the following form:
# ; <offset> <frame address> <frame offset> <information>
# ; <information> may be zero or more <kw>=<value> pairs
# ; Block=<blockname     specifies the block associated with this
# ;                      memory cell.
# ;
# ; Latch=<name>         specifies the latch associated with this memory cell.
#;
#; Net=<netname>        specifies the user net associated with this
#;                      memory cell.
#;
#; COMPARE=[YES | NO]   specifies whether or not it is appropriate
#;                      to compare this bit position between a
#;                      "program" and a "readback" bitstream.
#;                      If not present the default is NO.
#;
#; Ram=<ram id>:<bit>   This is used in cases where a CLB function
#; Rom=<ram id>:<bit>   generator is used as RAM (or ROM).  <Ram id>
#                      will be either 'F', 'G', or 'M', indicating
#                      that it is part of a single F or G function
#                      generator used as RAM, or as a single RAM
#;                      (or ROM) built from both F and G.  <Bit> is
#;                      a decimal number.
#;
#; Info lines have the following form:
#; Info <name>=<value>  specifies a bit associated with the LCA
#;                      configuration options, and the value of
#;                      that bit.  The names of these bits may have
#;                      special meaning to software reading the .ll file.


