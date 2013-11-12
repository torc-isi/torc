#!/usr/bin/perl -w
# This perl script takes in path to a folder as argument.
# It goes over all XDL files in the folder, generates NCD and then bitstreams.
# Author: Ritesh Kumar Soni (rsoni@vt.edu). Last modified: 21st August 2012

use strict;
use File::stat;
use Cwd;

# Check if the argument count is OK.
die "Usage: $0 <xdl_folder>\n" if scalar @ARGV != 1;

# Check if the directory exists
die "Input folder $ARGV[0] does not exist\n" unless -e $ARGV[0];
my $libDir = $ARGV[0];

print "Converting XDLs to BIT files...\n";

my $execDir = cwd(); #Get the current directory

my $partName = "xc5vfx130t";
print "Going to dir $libDir\n";
chdir $libDir;

# Go over all the XDL files in this directory
my @files = <*.xdl>;
my $badXdlFiles = "";
foreach my $xdlFile (@files) {
	print "\nWorking on XDL file $xdlFile ...\n";	
	# ncd file name
	my $ncdFileName = $xdlFile;
	$ncdFileName =~ s/.xdl/.ncd/g;
	my $execCode = 0;
	# generate NCD file from XDL file
	if(!(-e $ncdFileName)) {
		$execCode = generateNCDandBitstream($xdlFile);
	} else {
		# check if NCD is older than XDL
		my $xdlModTime = stat($xdlFile)->mtime;
		my $ncdModTime = stat($ncdFileName)->mtime;
		if($xdlModTime > $ncdModTime) {
			$execCode = generateNCDandBitstream($xdlFile);
		} else {
			print "NCD file $ncdFileName already present\n";
		}
	}
	
	# If there was problem generating NCD or bitstream
	if($execCode != 0) {
		$badXdlFiles = $badXdlFiles . $xdlFile . "\n";
	}
}

# Print out the names of XDL files for which NCD was not generated
if($badXdlFiles) {
	print "Bitstream generation failed for these XDLs \n $badXdlFiles \n";
}
chdir $execDir;


# Input: XDL file name
# Generates NCD and then bitstream. 
sub generateNCDandBitstream {
	my $xdlFile = $_[0];
	print "-----------------------------------------------\n";
	print "Running command xdl -xdl2ncd -force $xdlFile\n";
	my $execCode = system ("xdl -xdl2ncd -force $xdlFile 2>&1");
	print "-----------------------------------------------\n";
	# if NCD generated successfully
	if($execCode == 0) {

		print "Generated NCD...\n\n";

		# NCD file name
		my $ncdFile = $xdlFile;
		$ncdFile =~ s/\.xdl/\.ncd/g;

		# Generate bitstream
		print "-----------------------------------------------\n";
		print "bitgen -w -d $ncdFile";
		$execCode = system("bitgen -w -d $ncdFile 2>&1");
		print "-----------------------------------------------\n";
		if($execCode == 0) {
			print("Generated BITSTREAM...\n\n");
		} else {
			print "Failed to generate bitstream for $ncdFile\n";
		}
	} else {
		print "Failed to generate NCD for $xdlFile\n";
	}
	return $execCode;
} 

