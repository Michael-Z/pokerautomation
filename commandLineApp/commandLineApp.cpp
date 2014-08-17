/*
$Revision: 41 $
$Date: 2008-09-11 02:47:50 -0700 (Thu, 11 Sep 2008) $
$Author: vananth $
*/
#include <iostream>
#include "programInfo.h"
#include "strategyProb.h"
#include "pokerAcadTable.h"

void main()
{
	programInfo pInfo;
	gameState gs(&pInfo);
	win32Event ourTurnEvent;
	strategyProb aiStrat(&gs);
	pokerAcadTable table(&pInfo,&gs, &ourTurnEvent);
	

	int status = table.connect();	
	if (status)
	{
		std::cout<<"Error. Cannot connect to table\n";
		exit (1);
	}
	while (1)
	{
		ourTurnEvent.wait();
		ourTurnEvent.clear();
		aiStrat.doEval();
		table.performBettingAction(aiStrat.getAction());
	}
}