#!/usr/bin/perl -w

my $emp_line_cnt = 0;
my %map;

while (<STDIN>) {
	my $line = $_;
	chomp($line);
	if ($line =~ /^$/) {
		$emp_line_cnt++;
		if ($emp_line_cnt == 2) {
			last;
		}
	} elsif ($emp_line_cnt == 1 && $line =~ /^ *([0-9.]+) +[^ ]+ +[^ ]+ +[^ ]+ +[^ ]+ +[^ ]+ +([^ ].+)$/) {
		my $percentage = 0.0 + $1;
		if ($percentage == 0) {
			next;
		}
		my $name = $2;
		my $namespace = 'none';
		$name =~ s/const\s*$//;
		$name =~ s/\(([^()]++|(?R))\)//g;
		$name =~ s/<([^<>]++|(?R))>//g;
		$name =~ s/ *$//g;
		$name =~ s/^[^ ]* //g;
		if ($name =~ s/(.*):://) {
			$namespace = $1;
			$namespace =~ s/<.*$//;
		}
		$map{$namespace} = ($map{$namespace} or 0) + $percentage;
	}
}

my @list;

foreach my $key (keys(%map)) {
	my %data = (
		name => $key,
		value => $map{$key}
	);
	push(@list, \%data);
}

@list = sort { $b->{'value'} <=> $a->{'value'} } @list;

foreach my $ref (@list) {
	my %data = %$ref;
	printf "%2.2f %s\n", $data{'value'}, $data{'name'};
}
