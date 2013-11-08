use strict;
use File::Misc;
use File::Path;
use File::Copy;

my $templateFile = "_template_help.html";
my $outDir = "genhelp";

my $templateText = ReadFile($templateFile);

my @files = sort (<*.*>, );

for my $file (@files) {
	if ($file =~ /template/i) {
		next;
	}
	elsif ($file =~ /\.html$/i) {
		genFile($file) ;
	}
	elsif ($file =~ /\.(css|png|gif|hh\w)$/i) {
		copyFile($file);
	}
}

sub copyFile {
	my ($file) = @_;
	print "Copying $file\n";
	copy($file, "$outDir/$file");
}

sub genFile {
	my ($file) = @_;
	print "Generating $file\n";
	my $content = ReadFile($file) or die "$!: $file";
	my ($title) = $content =~ m!<h1>(.*?)</h1>!is;
	$title ||= $file;
	my $text = $templateText;
	$text =~ s/\$CONTENT\b/$content/gs;
	$text =~ s/\$TITLE\b/$title/gs;

	# colorize
	$text =~ s!(\<pb\>)(.*?)(\<\/pb\>)!'<pre class="pb">'. colorize($2). '</pre>'!gsie;
	
	# remove 1st empty line in <pre> 
	$text =~ s/(\<pre[^\>]*\>)\s+/$1/gsi;

	mkpath($outDir);
	WriteFile("$outDir/$file", $text);
}

sub colorize {
	my ($text) = @_;
	$text =~ s!(//.*)$!<span class="cmt">$1</span>!gm;
	$text =~ s!(/\*.*?\*/)!<span class="cmt">$1</span>!gms;
	return $text;
}