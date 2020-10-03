 #!/usr/bin/perl

print "\n\n\n Simulating data ..... \n\n\n";


# The theory in Chifman and Kubatko (2015) is derived for the case where each site has its own gene tree. 
# To simulate this, pick a large number of genes ($ngenes) and give each length 1 ($genelength).    

$ngenes = 50;
$genelength = 50;
$nsites = $ngenes*$genelength;

# The entire script can be run multiple times by changing the value of NLOOP
use constant NLOOP1 => 1;
$count = 0;
while ( $count < NLOOP1)
{

    @theta= (0.0016, 0.0048, 0.0144, 0.0432);

    for ($k=0; $k<4; $k++){

	open(my $fh1, '>>', 'output5a');
	print $fh1 "theta $k \n";
	close $fh1;
        open(my $fh2, '>>', 'astralFinal3');
        print $fh2 "theta $k \n";
        close $fh2;

# This outer loop generates site pattern probabilities for an increasingly difficult set of branch lengths for one given topology. (the plan is to go simulate for 5, 8, 12, and 20 leaves going from a symmetric to asymmetric topology)
for ($i=0; $i<4; $i++){

 
    # Set the branch lengths
   
    @tau1= (0.1, 0.25, 0.5, 1.0);
    @tau2= (0.2, 0.5, 1.0, 2.0);
    @tau3= (0.3, 0.75, 1.5, 3.0);
    @tau4= (0.4, 1.0, 2.0, 4.0);

    $j=0;
    while($j<100){

    # The command below runs the ms program

   system("./ms 5 $ngenes -t 2.0 -T -I 5 1 1 1 1 1 -ej $tau1[$i] 4 5 -ej $tau2[$i] 3 5 -ej $tau3[$i] 2 5 -ej $tau4[$i] 1 5 > treefile");
  

    # Get the file with the gene trees ready for seq-gen:
    system("awk -f awk.ms treefile > simtrees.dat");
    system("sed '1d' simtrees.dat > simtrees2.dat");
   system('sed "s/1/50/" simtrees2.dat > simtrees3.dat');

    # The commands below call seq-gen to simulate sites along the gene trees:
    # theta is set using the -s flag, where the numer following is 2theta
   #  JC69 model                                                                                                                
  #   system("./seq-gen < simtrees2.dat > infile -q -mHKY -s 0.001 -l $nsites -p $ngenes");

    #GTR model
     system("./seq-gen < simtrees3.dat > infile -q -mGTR -r 1.0 2.0 0.75 0.5 1.5 0.25 -f 0.15 0.35 0.35 0.15 -a 3.0 -g 3 -i 0.2 -s $theta[$k] -l $nsites -p $ngenes");
 
    # Now get the sequences ready to be counted by the CountSites program
   #  system(qq(awk 'NR==1{print "4 " \$2}' infile > infile2));

   

system("awk -f awk.script3 infile > temp1");
system("awk -f awk.script4 infile > temp2");
system("awk -f awk.script5 infile > temp3"); 
system("awk -f awk.script6 infile > temp4");
   system("awk -f awk.script7 infile > temp5");

system("cat temp1 temp2 temp3 temp4 temp5  > infile_data");

system("awk -f awk.script1a infile > temp1a");
system("awk -f awk.script2a infile > temp2a");
system("awk -f awk.script3a infile > temp3a");
system("awk -f awk.script4a infile > temp4a");
system("awk -f awk.script5a infile > temp5a");

system("cat paupHeader temp1a temp2a temp3a temp4a temp5a paupFooter > output4");
 system("cat paupHeader2 infile_data paupFooter paupFooter2 > infile_data2");
#  system("wait");

# system("paup -n paupTest");
 system("./GG 5 50 50");
 system("perl FT3.pl");
  system("wait");
 system("rm mamOut");
 system("java -jar astral.5.7.3.jar -i FTout -o mamOut -t 0");
 system("rm FTout");
 system('sed "s/Species1/0/" mamOut > mamOut2');
 system('sed "s/Species2/1/" mamOut2 > mamOut3');
 system('sed "s/Species3/2/" mamOut3 > mamOut4');
 system('sed "s/Species4/3/" mamOut4 > mamOut5');
 system('sed "s/Species5/4/" mamOut5 > mamOut13');
 system('sed "s/:0.0)/)/" mamOut13 > mamOut14');
 system("cat mamOut14 >> astralFinal2");

# This inner loop generates 100 runs drawing from the site pattern probabilities.

# system('./TripAssemBeta "infile_data2" 0.003'); # BWTA 

# system("paup -n paupfile"); # Unweighted SVDquartets
# system("rm output4");

     system("rm output8a");
 system('./BUWQAb "infile_data2" 0.003');   # BWQA 
 system("./max-cut-tree qrtt=output8 weights=on otre=output8a");
 my $filename = 'output8a';
 if(-e $filename){
     system("cat output8a >> output8b");
     system("cat mamOut14 >> astralFinal");
     ++$j;
 }
 system("rm output8");

# system("./svdqwRun 8 50000 100"); # Bootstrap weighted SVDquartets 
# system("./max-cut-tree qrtt=output7 weights=on otre=output7a");
# system("cat output7a >> output7b");
# system("rm output7");
    }
   

    open(my $fh1, '>>', 'output5a');
    print $fh1 "tau $i \n";
    close $fh1;
    open(my $fh2, '>>', 'astralFinal3');
    print $fh2 "tau $i \n";
    close $fh2;
   # system('./AssemSum 12 100 "(((0,(1,2)),(3,(4,5))),((6,(7,8)),(9,(10,11))))" 0 "output1" ');
  #  system("rm output1");
 #   system('./AssemSum 12 100 "(((0,(1,2)),(3,(4,5))),((6,(7,8)),(9,(10,11))))" 1 "svdq.tre" ');
#    system("rm svdq.tre");
    system('./AssemSum 5 100 "(0,(1,(2,(3,4))))" 1 "output8b" ');
    system("cat output8b >> output8c");
    system("rm output8b");
    system('./AssemSum 5 100 "(0,(1,(2,(3,4))))" 1 "astralFinal" ');
    system('cat astralFinal2 >> astralFinal3');
    system("rm astralFinal");
    system("rm astralFinal2");
   
}
    $count = $count+1;
      
    }

}

 print "Done.\n";

