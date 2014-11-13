#include <stdlib.h>
#include <time.h>
#include <math.h>

//===== OPERATING PARAMETERS =======

#define RUN_TIME (500000000) //number of 0.1 nsec units to run simulation

#define TX_PER 4 //transmission period in units of 100 ps

#define DEAD_TIME 1000   //in 0.1 nsec units

#define LINK_LOSS (0.90)

#define QBER (0.0)

#define H_EFF (0.6)
#define V_EFF (0.6)
#define L_EFF (0.6)
#define R_EFF (0.6)

//=================================

//Model Constants

#define HV_BASIS 0
#define LR_BASIS 1

//H=L=0, V=R=1

#define H 0
#define V 1
#define L 2
#define R 3

//Global vars

int dead_flags[4];
int dead_counts[4];

int a_basis, a_value, b_basis, b_value;
long int bit_count; 
long int tossed_bits;
  
int bob_key[RUN_TIME];

double det_eff[4];

double transition_prob;

//======================================= 

double TransitionProb(int key[], int nbits)
{
	int i, count;
	
	count = 0;
	for (i=0; i < nbits-1; i++)
		if(key[i] != key[i+1])
			count++;
	
	return (double)count/(double)(nbits-1);
}

//=======================================

void doQKD(long int tx_per, int dead_time) //both params in units of 100 ps
{
  int i, d, d2;
  double r1, r2, r3, r4, r5, r6, r7;
  long int clk;
  
  int sequence_count;
  
  //initialize everything

  srand(time(NULL));

  bit_count = 0;
  tossed_bits = 0;
  
  sequence_count = 0;

  for(d=H; d<=R; d++){
    dead_flags[d] = 0;
    dead_counts[d] = 0;
  }

  det_eff[H] = H_EFF;
  det_eff[V] = V_EFF;
  det_eff[L] = L_EFF;
  det_eff[R] = R_EFF;

  for (clk = 0; clk <= RUN_TIME; clk++){

    if((clk % tx_per) == 0){
    	//roll seven dice...
    	r1 = (double)rand()/((double)RAND_MAX + 1); //Alice's basis
    	r2 = (double)rand()/((double)RAND_MAX + 1); //Alice's bit value
    	r3 = (double)rand()/((double)RAND_MAX + 1); //link loss
    	r4 = (double)rand()/((double)RAND_MAX + 1); //Bob's basis
    	r5 = (double)rand()/((double)RAND_MAX + 1); //Bob's bit value (if req'd)
    	r6 = (double)rand()/((double)RAND_MAX + 1); //Error introduction/Background Counts
    	r7 = (double)rand()/((double)RAND_MAX + 1); //Detector Efficiency

    	//generate Alice's random bits...

    	a_basis = (r1 < 0.5) ? HV_BASIS : LR_BASIS;
	    a_value = (r2 < 0.5) ? 0 : 1;

    	if (r3 > LINK_LOSS){
      		//select Bob's basis
      		b_basis = (r4 < 0.5) ? HV_BASIS : LR_BASIS;
      		//sift
      		if (a_basis == b_basis){  //Bit goes through and is recorded...
				b_value = a_value;
				b_value = (r6<QBER) ? (!b_value) : b_value;  //introduce errors...
	
				d = (b_basis*2) + b_value;  //selects H,V,L or R... I know, I'm pretty clever...
	
				//d2 is assigned other detector in basis...
				d2 = ((d%2) == 0) ? d+1 : d-1;
	
				if ((r7 < det_eff[d]) && (!dead_flags[d])){
		    		if (!dead_flags[d2]){
	  					bob_key[bit_count] = b_value;
	  					bit_count++;
		    		}
		    		else
						tossed_bits++;
	            	dead_flags[d] = 1;
				}
      		}
      		else{  //basis is wrong, and detector fires, but bit is not recorded...
				b_value = (r5 < 0.5) ? 1 : 0;
				d = (b_basis*2) + b_value;
        		if ((r7 < det_eff[d]) && (!dead_flags[d])){
			  		dead_flags[d] = 1;
				}
	    	}
    	}
    }

    //increment dead time counters with the clock...

    for (d=H; d<=R; d++){
      if (dead_flags[d]){
		if(dead_counts[d] < dead_time)
	  		dead_counts[d]++;
		else{
	  		dead_flags[d] = 0;
	  		dead_counts[d] = 0;
        }
      }
    }
  }
  
  if ((dead_flags[H]==1)&&(dead_flags[V]==1))
  	sequence_count++;
  if ((dead_flags[H]==0)&&(dead_flags[V]==0)&&(sequence_count != 0)){
  	printf("%d\n", sequence_count);
  	sequence_count=0;
  }
}

//====================================

int main(int argc, char* argv[])
{
  doQKD(TX_PER, DEAD_TIME);
  return 0;
}
