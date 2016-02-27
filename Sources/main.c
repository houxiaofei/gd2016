#include "includes.h"



int main(void) {
	
	initALL();
	mdelay(1000);
	DataSet();
	initPIT();
	
	SET_motor(90,90);
	
	for(;;){
		KeyJudge();
	}
  
}



