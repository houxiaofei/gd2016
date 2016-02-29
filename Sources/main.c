#include "includes.h"



int main(void) {
	
	initALL();
	mdelay(1000);
	DataSet();
	
	tsl=120;
	tsr=120;
	SET_motor(tsl,tsr);
	
	initPIT();
	
	BlueTx();
	
	for(;;){
		//KeyJudge();
	}
  
}



