#include "includes.h"



int main(void) {
	
	initALL();
	initPIT();
	
	SET_motor_test();
	
	for(;;){
		KeyJudge();
	}
  
}



