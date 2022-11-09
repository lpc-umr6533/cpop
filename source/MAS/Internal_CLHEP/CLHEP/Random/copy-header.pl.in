#!/usr/bin/env perl
#
#  copy a header into the temporary CLHEP header tree if
#    a) the header is not already there
#    b) the source is newer
#

if ($#ARGV != 1) {
    print "mungeit requires two arguments - exit\n";
    exit;
} else {
    $source_header=$ARGV[0];
    $dest_header=$ARGV[1];
    &check_header;
}

exit;

sub check_header {
    # make sure source exists
    if ( ! -e $source_header ) {
	print "$source_header does not exist - exit\n";
	exit;
    }
    # copy if not already there - otherwise, check the time stamp
    if ( ! -e $dest_header ) {
	$cmd = "cp -p ".$source_header." ".$dest_header;
	print "$cmd\n";
	system($cmd);
    } else {
	$source_header_time = -M $source_header;
	$dest_header_time = -M $dest_header;
	# copy only if source is newer
	if( $source_header_time < $dest_header_time ) {
	    $cmd = "cp -p ".$source_header." ".$dest_header;
	    print "$cmd\n";
	    system($cmd);
	}
    }
}

