#!/usr/bin/perl 
use strict;

open LOG, ">processFile.log" or die "can not open ";
main(@ARGV);
close(LOG);

sub processFile{
	my ($filename) = @_;
	my $findout = 0;
	printf "$filename\n" ;
	print LOG "$filename\n" ;

	open SRC, "<$filename" or die "can not open $filename" ; 
	open DST, ">/tmp/xdwabc65390" or die "can not open ";

	while(my $line = <SRC> ){
		if($line =~ /toChange\(/ ){
			$findout = 1;
			print LOG "$line";
		
			print LOG "==>$line";
		}else{
			print DST $line 
		}
	}
	close (SRC);
	close (DST);
	if($findout == 1){
		`mv -f /tmp/xdwabc65390 $filename`;
		print "$filename changed completly\n";
	}
}

sub seekFolders
{
	my ($dir)=@_;
	my $next;
	opendir dir, $dir or die "Cannot open $dir : $!";
	foreach $next (readdir dir) {
		next if "$next" eq "." or "$next" eq ".." or "$next" eq ".svn";
		if ( -f "$dir/$next" )
		{
			if(( $next =~ /$\.cpp/) ||( $next =~ /$\.h/) ){
				processFile("$dir/$next") ;
			}
		}
		if ( -d "$dir/$next" ) {
			&seekFolders("$dir/$next");
		}
	}
	closedir (dir);
	return;
}

sub main(){
	if ( -f $ARGV[0]){
		processFile($ARGV[0]);
	} else {
		seekFolders($ARGV[0]);
	}
}
