#!/bin/perl -W

use strict;
use File::Basename;
use File::Spec;

# we expect a single input file name as an argument
die "Usage: $0 [file_name].ll\n" if scalar @ARGV != 1;
die "Input file $ARGV[0] does not exist on disk.\n" unless -s $ARGV[0];
my $input_filename = $ARGV[0];
my ($inputFileBase, $inputFileDir) = fileparse($input_filename);

# read the file contents
open(INPUT, $input_filename) or die "Unable to read $input_filename: $!\n";
my @lines = <INPUT>;
close INPUT;
chomp @lines;
print "$input_filename: read " . (scalar @lines) . " lines\n";

# open the output file
my $output_filename = $inputFileBase;
$output_filename =~ s/\.ll$/.bits/;
$output_filename = "$inputFileDir/$output_filename";

open(OUTPUT, ">$output_filename") or die "Unable to write $output_filename: $!\n";

my $par_filename = $input_filename;
$par_filename =~ s/\.ll$/-par.bits/;
open(PARFILE, ">$par_filename") or die "Unable to write $par_filename: $!\n";

# extract the data
# sample line: Bit 34086752 0x00200000    992 Block=RAMB36_X0Y23 Ram=B:BIT0
my $pattern = "\^Bit\\s+(\\d+)\\s+0x([0-9a-fA-F]+)\\s+(\\d+)\\s+Block=(\\w+)\\s+Ram=(B):([A-Za-z]+)(\\d+)\$";
my %bit;
my %par_bit;

foreach my $line (@lines) {
	# match the line against the expected pattern
	if($line =~ /$pattern/) 
	{
		my ($offset, $hex_frame_address, $frame_bit_index, $blockname, $ram_id, $bit_parbit, $index) = ($1, $2, $3, $4, $5, $6, $7);
	
		# extract the relevant fields if there is a match
		my $minor_address = hex substr $hex_frame_address, 6, 2;
		my $word_index = $frame_bit_index >> 5;
		my $bit_index = $frame_bit_index & 0x1F;
		if($bit_parbit eq "PARBIT") {
			$par_bit{$index} = "$minor_address:$word_index:$bit_index";
		} else {
			$bit{$index} = "$minor_address:$word_index:$bit_index";
		}
	}
}

# print all the bits in sequential order
my $count = pack "N", 1;
foreach my $key (sort { $a <=> $b } keys %bit) {
	# split the subfields apart
	my ($minor_address, $word_index, $bit_index) = split ":", $bit{$key};
	# pack the subfields into a single 32-bit word
	# 16 bits for frame index, 8 bits for word index, and 8 bits for bit index
	my $pack = pack "N", (($minor_address & 0xffff) << 16) | (($word_index & 0xff) << 8) | $bit_index;
	print OUTPUT "$count$pack";
#	printf "%5d: %v02X\n", $key, $pack;
}
close OUTPUT;

# print all the bits in sequential order
$count = pack "N", 1;
foreach my $key (sort { $a <=> $b } keys %par_bit) {
	# split the subfields apart
	my ($minor_address, $word_index, $bit_index) = split ":", $par_bit{$key};
	# pack the subfields into a single 32-bit word
	# 16 bits for frame index, 8 bits for word index, and 8 bits for bit index
	my $pack = pack "N", (($minor_address & 0xffff) << 16) | (($word_index & 0xff) << 8) | $bit_index;
	print PARFILE "$count$pack";
#	printf "%5d: %v02X\n", $key, $pack;
}
close PARFILE;

__END__
; <offset> <frame address> <frame offset> <information>
; <information> may be zero or more <kw>=<value> pairs
; Block=<blockname     specifies the block associated with this
;                      memory cell.
;
; Latch=<name>         specifies the latch associated with this memory cell.
;
; Net=<netname>        specifies the user net associated with this
;                      memory cell.
;
; COMPARE=[YES | NO]   specifies whether or not it is appropriate
;                      to compare this bit position between a
;                      "program" and a "readback" bitstream.
;                      If not present the default is NO.
;
; Ram=<ram id>:<bit>   This is used in cases where a CLB function
; Rom=<ram id>:<bit>   generator is used as RAM (or ROM).  <Ram id>
;                      will be either 'F', 'G', or 'M', indicating
;                      that it is part of a single F or G function
;                      generator used as RAM, or as a single RAM
;                      (or ROM) built from both F and G.  <Bit> is
;                      a decimal number.
;
; Info lines have the following form:
; Info <name>=<value>  specifies a bit associated with the LCA
;                      configuration options, and the value of
;                      that bit.  The names of these bits may have
;                      special meaning to software reading the .ll file.
