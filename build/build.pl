use strict;
use FindBin;
use lib "$FindBin::Bin";
use File::Path;
use File::Copy;
use POSIX qw/getcwd/;
use Cwd qw/abs_path/;
use File::Misc;
use File::Basename;
use CommonLog;
sub np($);

SetCommonLog("$FindBin::Bin/build.log");

our $full_ver = `git describe --tags --always`;
$full_ver =~ s/\s+//gs;
our ($ver) = $full_ver =~ /\D*([^\-]+(?:\-\d+)?)/;

our $echoon = 1;

our $pbRoot = "../pbsrc";
our $pbDir = "$pbRoot/pbinvoke";
our $docDir = "../doc";
our $vcDir = "../projects/libPBInvoke_vc9";
our $outDir = "builds/$ver";
our $tmpDir = 'tmp';
our $pbrtRoot = 'c:/usr/app/pbvm';
#our 

Print "Build version $full_ver ($ver)...\n";

our @SAMPLES = qw(BrowseForFolder SetWindowHook);
our @PB = (
	#{ VER=>10, DIR => 'PB10+', BUILD => '101_5502'},
	#{ VER=>10, DIR => 'PB10+', BUILD => '101_5502'},   #magic for PB6 to build
	#{ VER=>9 , DIR => 'PB9',   BUILD => '902_7509'},
	#{ VER=>6 , DIR => 'PB6-9', BUILD => '651_1163',},
	{ VER=>5 , DIR => 'PB6-9', BUILD => '504_76',},
	#{ VER=>6 , DIR => 'PB6-9', BUILD => '650_444'},
	{ VER=>10, DIR => 'PB10+', BUILD => '101_5502'},
);
our $errorlevel = 0;


TruncateCommonLog();

rmtree($tmpDir);

Print "Build DLL...\n";
call("$vcDir/build_all.cmd");
Print "Build DLL OK\n";



for my $pb (@PB)  {
	buildPBL($pb->{VER}, $pb->{BUILD}, $pb->{DIR});
}

for my $sample (@SAMPLES) {
	makeSample($sample);
}
makeSampleZip();
makeReadme();

makeDemo(map {$_->{DIR} } @PB);

makeFull(map {$_->{DIR} } @PB);


sub makeSample {
	my ($sample) = @_;
	Print "makeSample $sample ...\n";

	for my $pb (@PB) {
		makeSamplePB($sample, $pb);
	}
	Print "makeSample OK\n";
}

sub makeSamplePB {
	my ($sample, $pb) = @_;
	Print "PWD " . getcwd() . "\n";
	Print "  makeSample $sample $pb->{DIR} ...\n";
	my $sampleInDir = "$pbRoot/Samples/$sample";
	my $sampleOutDir = "$tmpDir/Samples/$pb->{DIR}/$sample";
	rmtree($sampleOutDir);
	mkpath($sampleOutDir);
	copyFiles($sampleInDir, "*.pbl", $sampleOutDir);
	copyFiles($sampleInDir, "*.pbt", $sampleOutDir);
	copyFiles($sampleInDir, "*.pbc", $sampleOutDir);
	copyFiles($sampleInDir, "*.dll", $sampleOutDir);
	copyFiles($sampleInDir, "*.txt", $sampleOutDir);
	migrate($sampleOutDir, $pb);
}

sub copyFiles {
	my ($srcDir, $mask, $destDir) = @_;
	#print "$srcMask\n";
	$mask =~ s/\./\\./gs;
	$mask =~ s/\*/.*/gs;
	$mask =~ s/\?/./gs;

	$mask = qr!$mask$!i;
	opendir(D, $srcDir) or die "$! $srcDir";
	my @names = grep {/$mask/} readdir D; 
	closedir D;
	#my @files = <../pbsrc/Samples/BrowseForFolder/*.pbl>; 
	
	for my $name (@names) {
		print "$destDir/$name\n";
		copy("$srcDir/$name", "$destDir/$name");
	}
}

sub makeReadme() {
	Print "makeReadme\n";
	my $readme = ReadFile("readme.txt");
	$readme =~ s/\$PBINVOKE_VER/$ver/gsi;
	WriteFile("$tmpDir/readme.txt", $readme);
	Print "makeReadme OK\n";
}
sub makeDemo {
	my (@pbver) = @_;
	Print "makeDemo ...\n";
	mkpath($outDir);
	my $arc = np abs_path("$outDir")."/pbinvoke-$ver-demo.zip";
	my $desc = "$outDir/demo";
	unlink $arc;

	copy ("$vcDir/ReleaseDemo/libPBInvoke.dll", "$tmpDir/libPBInvoke.dll");
	copy ("$docDir/pbinvoke_en.chm", "$tmpDir/pbinvoke_en.chm");
	copy ("license.txt", "$tmpDir/license.txt");
	pushd("$tmpDir");
	call ("pkzipc -add $arc *.txt");
	call ("pkzipc -add $arc libPBInvoke.dll");
	call ("pkzipc -add $arc *.chm");

	for my $pbver (@pbver) {
		call ("pkzipc -add -dir $arc $pbver/pbinvoke.pbl");
	}
	popd();
	WriteFile($desc, <<EOF);
(
	'file'=>'pbinvoke-$ver-demo.zip',
	'name'=>'PBInvoke $ver Demo'
)
EOF
	Print "makeDemo OK\n";
} 
sub makeSampleZip {
	Print "makeSampleZip ...\n";
	mkpath($outDir);
	my $arc = np abs_path("$outDir")."/pbinvoke-$ver-samples.zip";
	my $desc = "$outDir/samples";
	unlink $arc;

	pushd("$tmpDir");

	for my $pb (@PB) {
      for my $sample (@SAMPLES) {
			call ("pkzipc -add -dir $arc Samples/$pb->{DIR}/*.pbl");
			call ("pkzipc -add -dir $arc Samples/$pb->{DIR}/*.pbt");
			call ("pkzipc -add -dir $arc Samples/$pb->{DIR}/*.dll");
			call ("pkzipc -add -dir $arc Samples/$pb->{DIR}/*.txt");
      }
	}
	popd();
	WriteFile($desc, <<EOF);
(
	'file'=>'pbinvoke-$ver-samples.zip',
	'name'=>'PBInvoke Samples (All PB versions)'
)
EOF
	Print "makeSampleZip OK\n";
} 

sub makeFull {
	my (@pbver) = @_;
	Print "makeFull ...\n";
	mkpath($outDir);
	my $arc = np abs_path("$outDir")."/pbinvoke-$ver.zip";
	my $desc = "$outDir/product.xml.x";
	unlink $arc;
	copy ("$docDir/pbinvoke_en.chm", "$tmpDir/pbinvoke_en.chm");
	copy ("license.txt", "$tmpDir/license.txt");
	pushd("$tmpDir");
	call ("pkzipc -add $arc *.txt");
	#call ("pkzipc -add -store $arc libPBInvoke.dll");
	call ("pkzipc -add $arc *.chm");

	for my $pbver (@pbver) {
		call ("pkzipc -add -dir $arc $pbver/pbinvoke.pbl");
	}
	popd();
		
	unlink "$arc.x";
	rename $arc, "$arc.x";
	copy ("$vcDir/Release/libPBInvoke.dll", "$outDir/libPBInvoke.dll-$ver.x");
	 
	WriteFile($desc, <<EOF);
<product
	fulltemplate="pbinvoke-$ver.zip.x"
	fulldlltemplate="libPBInvoke.dll-$ver.x"
	fullname="pbinvoke-$ver.zip"
	dllname="libPBInvoke.dll"
/>
EOF

	Print "makeFull OK\n";
} 

our @dirs;
sub pushd {
	my ($dir) = @_;
	my $oldDir = getcwd();
	push @dirs, $oldDir;
	chdir $dir;
	Print "pushd $dir (old $oldDir)\n";
}
sub popd {
	my $dir = pop @dirs;
	chdir $dir;
	Print "popd $dir\n";
}


sub np($) {
	my ($path) = @_;
	$path =~ s!/!\\!gs;
	return $path;
}
sub buildPBL {
	my ($pbver, $pbbuild, $name) = @_;
	Print "buildPBL $pbver, $pbbuild, $name ...\n";
	my $buildDir = np "$tmpDir/$name";
	rmtree($buildDir);
	mkpath($buildDir);
	-d $buildDir or die "$buildDir does not exist"; 
	for (qw(pbinvoke.pbl build_pbinvoke.pbl default.pbc)) {
		copy ("$pbDir/$_", "$buildDir/$_") or die "$! can't copy $pbDir/$_";
	}

	my $pbrt = np "$pbrtRoot/$pbver/$pbbuild";
	my $path = $ENV{PATH};
	local $ENV{PATH} = "$pbrt;$path";

	call("pbc -v $pbver -p $buildDir/default.pbc -pbdir \"$pbrt\" -m -f migrate.pm -log $buildDir/migrate.log")
		or Die("buildPBL failed");

	Print "buildPBL OK\n";
}


sub migrate {
	my ($dir, $pb) = @_;
	pushd($dir);
	my $pbrt = $pb->{PBRT};
	$pbrt ||= np "$pbrtRoot/$pb->{VER}/$pb->{BUILD}";
	my $path = $ENV{PATH};
	local $ENV{PATH} = "$pbrt;$path";
	my $passes = 3;
	Print "Migration ...\n";
	while(1) {
		my $ret = call("pbc -v $pb->{VER} -p default.pbc -pbdir \"$pbrt\" -m -f migrate.pm -log migrate.log");
		Print("ret=$ret\n");
		last if $ret;
		#next if --$passes > 0;
		Die("Migration failed");
	}
	popd();
	Print "Migration OK\n";
}

sub call {
	my ($cmd) = @_;
	$cmd =~ s!(\S)/(\S)!$1\\$2!gs;
	my $print = $echoon;
	Print (sprintf "%s\n", $cmd)  if $print;
	$errorlevel = system $cmd;
	return $errorlevel == 0;
}