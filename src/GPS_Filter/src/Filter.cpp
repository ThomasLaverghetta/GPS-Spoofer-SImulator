#include "Filter.h"

// ------------------------------------------
// IMPORTANT!! - Make sure there is a definition for CreateApplicationNode()
Node* CreateApplicationNode()
{
	return new Filter();        // Make sure to change this to correct Node class type
}
// ------------------------------------------


void termination_handler (int signum)
{
  Node::Get()->Terminate(); // Example call to terminate the application with OS control signal
}


void Filter::Setup(int argc, char** argv)
{   
	std::string input1, input2, input3, input4, input5;
	input1 = FindTopicName("input1");
	input2 = FindTopicName("input2");
	input3 = FindTopicName("input3");
	input4 = FindTopicName("input4");
	input5 = FindTopicName("input5");

	Subscribe(input1, &G[0]);
	Subscribe(input2, &G[1]);
	Subscribe(input3, &G[2]);
	Subscribe(input4, &G[3]);
	Subscribe(input5, &G[4]);

	std::string output_topic1 = FindTopicName("output1");
	std::string output_topic2 = FindTopicName("output2");
	std::string output_topic3 = FindTopicName("output3");
	std::string output_topic4 = FindTopicName("output4");

	Publish(output_topic1, &G[0]);
	Publish(output_topic2, &G[1]);
	Publish(output_topic3, &G[2]);
	Publish(output_topic4, &G[3]);

	RegisterInitFunction(static_cast<NodeFuncPtr>(&Filter::AppInit));
    
	RegisterCoreFunction(static_cast<NodeFuncPtr>(&Filter::Process));
    
    RegisterExitFunction(static_cast<NodeFuncPtr>(&Filter::OnExit));
}


void Filter::SetNodeName(int argc, char** argv, std::string& nodeName)
{
	nodeName = "Filter";
}

void Filter::AppInit()
{	
	srand (time(NULL));
	Keyboard_Init();
}



void Filter::Process()
{
 	// ---- Termination Signal ------ //
  	if (signal (SIGINT, termination_handler) == SIG_IGN)
    	signal (SIGINT, SIG_IGN);
    // bool match = false, spoof = false;
    // int sat[] = {-1, -1};
    // for (int i = 0; i < (sizeof(G)/sizeof(*G)); i++){
    // 	if (G[i].GetSpoofer == 1){
    // 		int temp = G[i].GetGPSNum();
    // 		spoof = true; sat[0] = i;
    // 		for (int j = 0; j < (sizeof(G)/sizeof(*G)); j++){
    // 			if (j != i && G[j].GetGPSNum() == temp){
    // 				match = true; sat[1] = j;  
    // 				break;
    // 			}
    // 		}
    // 		break;
    // 	}
    // }

    // if (sat[0] != -1)
    // 	if (match)
    // 		G[]

    // G[4] is spoofed signal (might need to be improved to take in more signals)
    int match = -1; bool spoof = false;
    if (G[4].GetSpoofer()){
    	int temp = G[4].GetGPSNum();
    	for (int j = 0; j < (sizeof(G)/sizeof(*G)-1); j++){
    		if (G[j].GetGPSNum() == temp && j != 4){
    			match = j;
   				break;
   			}
   		}
   	}

   	if (match != -1){
   		G[match] = G[4]; // Copying spoofed data into true signal, such that, it will be published
   		printf("Spoofed GPS is %i\n", match);
   	}
   	else if (spoof){	// randomly choosing which satlite to replace
   		int r = rand()%4;
   		printf("Spoofed GPS is %i\n", r);
   		G[r] = G[4];
   	}


  /// sorting the quality of the signal (doing the first four signals; fifth is gone)
    // Quaility == strength
  int i, j; 
  for(i = 0; i < 3; i++)
    for (j = 0; j < 3-i;j++)
      if (G[j].GetQualityInd() > G[j+1].GetQualityInd()){
        GPSObject temp = G[j];
        G[j] = G[j+1];
        G[j+1] = temp;
      }

  for (int j = 0; j < (sizeof(G)/sizeof(*G)-1); j++)
     G[j].SetFlagged(true);
}

void Filter::OnExit()
{
	Keyboard_Cleanup();
}
