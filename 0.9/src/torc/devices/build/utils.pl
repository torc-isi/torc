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

	# keep ourselves honest
	use strict;
	# this is the utils package
	package utils;


# function prototypes
sub deep_copy(@);
sub determine_membership($$);
sub array_to_hash(@);
sub remove_array_entry($$);
sub remove_array_duplicates(@);
sub substitute($@);
sub read_file_as_string($);
sub read_binary_file_as_string($);
sub write_string_as_file($$);


# do a deep copy of any structure

sub deep_copy(@) {
	my @copy = ();
	foreach my $item (@_) {
		my $type = ref $item;
		if($type eq "HASH") {
			my %item = deep_copy(%$item);
			push @copy, \%item;
		} elsif($type eq "ARRAY") {
			my @item = deep_copy(@$item);
			push @copy, \@item;
		} else {
			push @copy, $item;
		}
	}
	return @copy;
}


# split hash entries into A-only membership, B-only membership, and A and B membership

sub determine_membership($$) {
	my ($A_ref,$B_ref) = (shift,shift);
	my %A = %$A_ref;
	my %B = %$B_ref;
	my %AB = ();
	
	# yeah, there's probably a better way to do this
	
	# find intersection
	foreach my $A (keys %A) {
		if(exists $B{$A}) {
			$AB{$A} = undef;
		}
	}
	foreach my $B (keys %B) {
		if(exists $A{$B}) {
			$AB{$B} = undef;
		}
	}
	
	# find A-only members
	foreach my $A (keys %A) {
		if(exists $B{$A}) {
			delete $A{$A};
		}
	}
	
	# find B-only members
	foreach my $B (keys %A) {
		if(exists $A{$B}) {
			delete $B{$B};
		}
	}
	
	# and return the results
	return (\%A,\%AB,\%B);
}


# take the elements of the given array and return them as keys to a new hash

sub array_to_hash(@) {
	
	# declare a new empty hash
	my %hash = ();
	
	# loop through the array elements
	foreach my $key (@_) {
		$hash{$key} = undef;
	}
	
	# and return the hash
	return %hash;
	
}


# remove the specified entry from the given array, if that entry exists

sub remove_array_entry($$) {

	# extract the entry and the array
	my $entry = shift;
	my $array_ref = shift;

	# iterate through the array and remove the element in question if found
	for(my $i = $#$array_ref; $i >= 0; $i--) {
		splice @$array_ref, $i, 1 if $$array_ref[$i] eq $entry;
	}

#	# return the resulting array
#	return @array;

}


# remove duplicate elements from an array (WARNING: The order will be changed)

sub remove_array_duplicates(@) {
	
	# convert the array to a hash
	my %hash = array_to_hash(@_);
	
	# convert to hash back to an array
	return keys %hash;
	
}


# substitute values into placeholders

sub substitute($@) {
	
	# grab the parameters
	my $string = shift;
	my %substitutions = @_;
	
	# iterate through each substitution key and replace it in the string
	foreach my $placeholder (keys %substitutions) {
		
		$string =~ s/$placeholder/$substitutions{$placeholder}/g;
		
	}
	
	# return the modified string
	return $string;
	
}


# read the entire contents of a file and return them as a string

sub read_file_as_string($) {
	
	# grab the parameters
	my $file_name = shift;
	
	# open the file for input
	open(FILE,$file_name) or die "Unable to read $file_name: $!\n";
	
	# read the contents into a string
	my $contents = join "", <FILE>;
	
	# close the file
	close FILE;
	
	# return the contents
	return $contents;
	
}


# read the entire contents of a file and return them as a string

sub read_binary_file_as_string($) {
	
	# grab the parameters
	my $file_name = shift;
	
	# open the file for input
	open(FILE,$file_name) or die "Unable to read $file_name: $!\n";
	
	# be sure to specify binary access
	binmode FILE;
	
	# read the contents into a string
	my $contents = join "", <FILE>;
	
	# close the file
	close FILE;
	
	# return the contents
	return $contents;
	
}


# write the specified file with the specified contents

sub write_string_as_file($$) {
	
	# grab the parameters
	my $file_name = shift;
	my $contents = shift;
	
	# open the file for output
	open(FILE,">$file_name") or die "Unable to write $file_name: $!\n";
	
	# write the contents from the string
	print FILE $contents;
	
	# close the file
	close FILE;
	
}


