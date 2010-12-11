#!/usr/bin/perl
#
# usage: petype <exefile>
#
$exe = $ARGV[0];

open(EXE, $exe) or die "can't open $exe: $!";
binmode(EXE);
if (read(EXE, $doshdr, 68)) {

   ($magic,$skip,$offset)=unpack('a2a58l', $doshdr);
   die("Not an executable") if ($magic ne 'MZ');

   seek(EXE,$offset,SEEK_SET);
   if (read(EXE, $pehdr, 6)){
       ($sig,$skip,$machine)=unpack('a2a2v', $pehdr);
       die("No a PE Executable") if ($sig ne 'PE');

       if ($machine == 0x014c){
            print "i386\n";
       }
       elsif ($machine == 0x0200){
            print "IA64\n";
       }
       elsif ($machine == 0x8664){
            print "AMD64\n";
       }
       else{
            printf("Unknown machine type 0x%lx\n", $machine);
       }
   }
}

close(EXE);
