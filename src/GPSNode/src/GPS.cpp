#include "GPS.h"

// ------------------------------------------
// IMPORTANT!! - Make sure there is a definition for CreateApplicationNode()
Node* CreateApplicationNode()
{
	return new GPS();        // Make sure to change this to correct Node class type
}
// ------------------------------------------


void termination_handler (int signum)
{
  Node::Get()->Terminate(); // Example call to terminate the application with OS control signal
}


void GPS::Setup(int argc, char** argv)
{   
	// determining if spoofer node
	if (ros::param::has("~spoof"))
		ros::param::get("~spoof", spoofer);
	else 
		spoofer = 0;

	// if spoofer node, copy input data
	if (spoofer){
		std::string input1;
		input1 = FindTopicName("input1");
		Subscribe(input1, &GPSInputData);
	}

	std::string output1;
	output1 = FindTopicName("output1");

	Publish(output1, &GPSData);

	RegisterInitFunction(static_cast<NodeFuncPtr>(&GPS::AppInit));
    
	RegisterCoreFunction(static_cast<NodeFuncPtr>(&GPS::Process));
    
    RegisterExitFunction(static_cast<NodeFuncPtr>(&GPS::OnExit));
}


void GPS::SetNodeName(int argc, char** argv, std::string& nodeName)
{
	nodeName = "GPS";
}

void GPS::AppInit()
{	
	if (spoofer){
		// What is the spoofing attack
		int AttackMode;
		if(ros::param::has("~AttackMode")); // distance in Km)
		{
			ros::param::get("~AttackMode",AttackMode);
			if (AttackMode == 0) // Use Different Position
				SetParamGPS();

			/* 
				Slow Change Position
				changing the position over time, such that, 
				changes will not be noticed.
			*/
			else if(AttackMode == 1) 
				GPSData = GPSInputData;

		}
		else 
			printf("ERROR \n");
	}
	else
		SetParamGPS();
 

	ResetClock();

	Keyboard_Init();
}

void GPS::SetParamGPS(){
	float f = 1/298.257, a = 6378137, e = sqrt(2*f-pow(f,2.0)), N, A, B, C;
	if(ros::param::has("~lat")){
	  	ros::param::get("~lat", Lat); GPSData.SetLat(Lat); // degrees east
	}
	else
		GPSData.SetLat(0);

	if(ros::param::has("~long")){
	  	ros::param::get("~long", Long); GPSData.SetLong(Long); // degrees north
	}
	else 
		GPSData.SetLong(0);

	if(ros::param::has("~SatNum")){
	  	ros::param::get("~SatNum", SatNum); GPSData.SetGPSNum(SatNum); // int 
	}
	else{
		printf("NO instance of GPS Num\n"); 
		GPSData.SetGPSNum(0);
	}

	if (ros::param::has("~D")){
		ros::param::get("~D", D); // distance in Km
	}

	if(ros::param::has("~Quality")); // distance in Km)
	{
		ros::param::get("~Quality",Quality); GPSData.SetQualityInd(Quality);
	}


		/*// CALCULATING ALT - Calculation and Proof will be included in report
	  		Summary: 
	  		-- uses the distance, latitude, & longitude to calculate altitude
	  		-- of GPS	
		*/
	  	// Ellipsoid Constant
	  	N = a / sqrt(1-pow(e*sin(GPSData.GetLat()/180.0f*M_PI),2.0));
	  	float alpha = cos(GPSData.GetLat()/180.0f*M_PI)*cos(GPSData.GetLong()/180.0f*M_PI);
	  	float beta = sin(GPSData.GetLat()/180.0f*M_PI)*cos(GPSData.GetLong()/180.0f*M_PI);
	  	float lamda = sin(GPSData.GetLong()/180.0f*M_PI);
	  	float phi = N*(1-pow(e,2.0));
	  	// More Constants
	  	A = pow(alpha,2.0) + pow(beta,2.0) + pow(lamda,2.0);
	  	B = 2*N*(pow(alpha,2.0)+pow(beta,2.0))+2*phi*pow(lamda,2.0);
	  	C = pow(D,2.0)-pow(N,2.0)*(pow(alpha,2.0)+pow(beta,2.0))-pow(phi*lamda,2.0);
	  	// Determine if complex
	  	if (A*C < 0)
	  		Node::Get()->Terminate();
	  	float hPos = (-B+sqrt(pow(B,2.0)+4*A*C))/(2.0*A);
	  	float hNeg = (-B-sqrt(pow(B,2.0)+4*A*C))/(2.0*A);

	  	// Find largest value between two altitude valuess
	  	GPSData.SetAlt((hPos > hNeg) ? hPos : hNeg);
}

void GPS::Process()
{
 	// // ---- Termination Signal ------ //
  // 	if (signal (SIGINT, termination_handler) == SIG_IGN)
  //   	signal (SIGINT, SIG_IGN);

    sleep(10); // n-second delay

    if (spoofer) falseSignal();
    else trueSignal();

}

// True GPS Signal
void GPS::trueSignal(){

	// Getting the latest time
	GPSData.SetTime(UTC_Time());
	GPSData.SetFlagged(true);
}

// Spoofer GPS Signal
void GPS::falseSignal(){
	// If we want to change the mode of attack on the fly 
	Keyboard_Update(0, 1000);


	// Position Attack change position (give some random position)
	if(Keyboard_GetLastKey() == 0 || AttackMode == 0)
	{ 
		// Reset Parameters
		if (Keyboard_GetLastKey() == 0)
			SetParamGPS(); 

		// Get Latest Time 
		GPSData.SetTime(UTC_Time());
		GPSData.SetFlagged(true);
	}

	else if (Keyboard_GetLastKey() == 1 ||AttackMode == 1){
		// Resetting GPS data to change
		if (Keyboard_GetLastKey() == 1)
			GPSData = GPSInputData; 
		
		// Changing position
		GPSData.SetLong(GPSData.GetLong()-0.00005); 
		GPSData.SetLat(GPSData.GetLat()-0.00005);
		GPSData.SetTime(UTC_Time());
		GPSData.SetFlagged(true); 
	}

	/*
		Time Attack
		Change the time, such that, positions will be calculated wrong
	*/
	else if(Keyboard_GetLastKey() == 2 || AttackMode == 2)
	{	
		// Rereset Time 
		if (Keyboard_GetLastKey() == 2)
			GPSData.SetTime(UTC_Time());
		
		// Change Time
		GPSData.SetTime(UTC_Time()-0.05);
		GPSData.SetFlagged(true);
	}
}

void GPS::OnExit()
{
	Keyboard_Cleanup();
}
