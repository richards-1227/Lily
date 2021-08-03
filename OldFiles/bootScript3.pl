 #!/usr/bin/perl

print "\n\n\n Simulating data ..... \n\n\n";


# The theory in Chifman and Kubatko (2015) is derived for the case where each site has its own gene tree. 
# To simulate this, pick a large number of genes ($ngenes) and give each length 1 ($genelength).    

$ngenes = 10;
$genelength = 500;
$nsites = $ngenes*$genelength;

# The entire script can be run multiple times by changing the value of NLOOP
use constant NLOOP1 => 1;
$count = 0;
while ( $count < NLOOP1)
{


# This outer loop generates site pattern probabilities for an increasingly difficult set of branch lengths for one given topology. (the plan is to go simulate for 5, 8, 12, and 20 leaves going from a symmetric to asymmetric topology)


 
    # Set the branch lengths
    $tau1= 0.5;
    $tau2= 1.0;
    $tau3= 0.8;
    $tau4= 1.5;



    for ($j=0; $j<100; $j++){

    # The command below runs the ms program
system("./ms 5 $ngenes -t 2.0 -T -I 5 1 1 1 1 1 -ej $tau1 4 5 -ej $tau2 3 5 -ej $tau3 1 2 -ej $tau4 2 5 > treefile");
  

    # Get the file with the gene trees ready for seq-gen:
    system("awk -f awk.ms treefile > simtrees.dat");
    system("sed '1d' simtrees.dat > simtrees2.dat");
   system('sed "s/1/500/" simtrees2.dat > simtrees3.dat');

    # The commands below call seq-gen to simulate sites along the gene trees:
    # theta is set using the -s flag, where the numer following is 2theta
   #  JC69 model                                                                                                                
  #   system("./seq-gen < simtrees2.dat > infile -q -mHKY -s 0.001 -l $nsites -p $ngenes");

    #GTR model
     system("./seq-gen < simtrees3.dat > infile -q -mGTR -r 1.0 2.0 0.75 0.5 1.5 0.25 -f 0.15 0.35 0.35 0.15 -a 3.0 -g 3 -i 0.2 -s 0.0016 -l $nsites -p $ngenes");
 
    # Now get the sequences ready to be counted by the CountSites program
   #  system(qq(awk 'NR==1{print "4 " \$2}' infile > infile2));

   

    system("awk -f awk.script3 infile > temp1");
 system("awk -f awk.script4 infile > temp2");
system("awk -f awk.script5 infile > temp3"); 
system("awk -f awk.script6 infile > temp4");
system("awk -f awk.script7 infile > temp5");

     system("cat temp1 temp2 temp3 temp4 temp5 > infile_data");

system("awk -f awk.script1a infile > temp1a");
system("awk -f awk.script2a infile > temp2a");
system("awk -f awk.script3a infile > temp3a");
system("awk -f awk.script4a infile > temp4a");
system("awk -f awk.script5a infile > temp5a");

system("cat paupHeader temp1a temp2a temp3a temp4a temp5a paupFooter > output4");
system("cat paupHeader2 infile_data paupFooter paupFooter2 > infile_data2");

# This inner loop generates 100 runs drawing from the site pattern probabilities.
 

system("rm output2");
system('./TripBootML "infile_data2" 0.003 "((0,1),(2,(3,4)))" 100 10 500');

# system("./bootS");
# system("rm output5");


    }
#    system('./BootSumNew "output2a" 100 1 5');
   
    $count = $count+1;
      
}

 print "Done.\n";

