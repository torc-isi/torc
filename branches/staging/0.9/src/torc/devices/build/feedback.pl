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
	package feedback;


	# function prototypes
	sub print;
	sub TIEHANDLE;
	sub PRINT;
	sub flush($);
	sub raw_output($$);


	# set up a default feedback file handle
	$feedback::out_ref = tie *feedback::out, "feedback";


	# lazy interface for callers

	sub print {
		print feedback::out @_;
	}


	# function to tie the handle to this class

	sub TIEHANDLE {

		# declare a variable which we can bless
		my $blessed_variable = undef;

		# perform the rites
		bless \$blessed_variable, shift;

	}


	# function to handle the print function

	sub PRINT {

		# blessed reference
		my $blessed_variable_ref = shift;

		# we only consider the first element in the list
		my $output = $_[0];

		# call a subordinate to do the actual display
		$blessed_variable_ref->raw_output($output);

		# remember the last output
		$$blessed_variable_ref = $output;

	}


	# function to flush all previous feedback

	sub flush($) {

		# handle non-member call
		unshift @_, $feedback::out_ref if scalar @_ == 0;

		# blessed reference
		my $blessed_variable_ref = shift;

		# do a visual flush
		raw_output($blessed_variable_ref,"");

		# and clear the last output variable
		$$blessed_variable_ref = undef;

	}


	# low-level display function

	sub raw_output($$) {

		# blessed reference
		my $blessed_variable_ref = shift;
		my $output = shift;
		my $last_output = $$blessed_variable_ref;

		# store the buffering setting
		my $saved_buffering = $|;

		# turn off buffering
		$| = 1;

		if(defined $last_output) {
			$last_output =~ s/./\010 \010/g;
			print STDOUT $last_output;
		}

		# display the new output
		print STDOUT $output;

		# restore the buffering setting
		$| = $saved_buffering;

	}


