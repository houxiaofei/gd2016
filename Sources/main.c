#include "includes.h"



int main(void) {
	
	initALL();
	initPIT();
	//BlueTx();
	
	SET_motor_test();
	
	for(;;){
		KeyJudge();
	}
  
}



