#include "Receiver.h"

int c = 299792; // speed of light km/s

// ------------------------------------------
// IMPORTANT!! - Make sure there is a definition for CreateApplicationNode()
Node* CreateApplicationNode()
{
	return new Receiver();        // Make sure to change this to correct Node class type
}
// ------------------------------------------


void termination_handler (int signum)
{
  Node::Get()->Terminate(); // Example call to terminate the application with OS control signal
}


void Receiver::Setup(int argc, char** argv)
{    
	std::string input1, input2, input3, input4;
	// std::string output1;

	input1 = FindTopicName("input1");
	input2 = FindTopicName("input2");
	input3 = FindTopicName("input3");
	input4 = FindTopicName("input4");
	//output1 = FindTopicName("output1");


	// sorted data from Filter
	Subscribe(input1, &G[0].GPS);
	Subscribe(input2, &G[1].GPS);
	Subscribe(input3, &G[2].GPS);
	Subscribe(input4, &G[3].GPS);

	//Publish(output1, &vehicle_location);

	RegisterInitFunction(static_cast<NodeFuncPtr>(&Receiver::AppInit));
    
	RegisterCoreFunction(static_cast<NodeFuncPtr>(&Receiver::Process));
    
    RegisterExitFunction(static_cast<NodeFuncPtr>(&Receiver::OnExit));
}


void Receiver::SetNodeName(int argc, char** argv, std::string& nodeName)
{
	nodeName = "Receiver";
}

void Receiver::AppInit()
{	

	// if(Load(parameterFile) == false)
 //    {
 //    	printf("Failed to Load Parameters File: %s\n", parameterFile);
 //    	Node::Get()->Terminate();
 //    }

	ResetClock();
	Keyboard_Init();
}

void Receiver::Process()
{
 	// ---- Termination Signal ------ //
  	if (signal (SIGINT, termination_handler) == SIG_IGN)
    	signal (SIGINT, SIG_IGN);

    /* Calculating x,y,z for the first three GPSs 
    	--The calculation were found: http://www.telesens.co/2017/07/17/calculating-position-from-raw-gps-data/
    */
    
    // constants for finding GPS positioning in x,y,z; Converting spherical to cartisian
    float f = 1/298.257, a = 6378137, e = sqrt(2*f-pow(f,2.0)), N, A, B, C;
    for (int j = 0; j < (sizeof(G)/sizeof(*G)-1); j++){
    	N = a / sqrt(1-pow(e*sin(G[j].GPS.GetLat()/180.0f*M_PI),2.0));
    	G[j].x = (N + G[j].GPS.GetAlt())*cos(G[j].GPS.GetLat()/180.0f*M_PI)*cos(G[j].GPS.GetLong()/180.0f*M_PI);
    	G[j].y = (N + G[j].GPS.GetAlt())*cos(G[j].GPS.GetLat()/180.0f*M_PI)*sin(G[j].GPS.GetLong()/180.0f*M_PI);
    	G[j].z = (N*(1-pow(e,2))+G[j].GPS.GetAlt())*sin(G[j].GPS.GetLat()/180.0f*M_PI);
    	D[j] = c*G[j].GPS.GetDeltaT(); // in km
    }

    // Calculate vectors and mag between each sat
    for (int j = 0; j < (sizeof(G)/sizeof(*G)-2); j++){
    	V[j].x = G[j].x-G[j+1].x;
    	V[j].y = G[j].y-G[j+1].y; 
    	V[j].z = G[j].z-G[j+1].z;
    	V[j].mag = sqrt(V[j].x*V[j].x+V[j].y*V[j].y+V[j].z*V[j].z);
    }

    /*
    	0 == AB
    	1 == BC
    	2 == AC
    */
	V[2].x = G[0].x-G[2].x;
    V[2].y = G[0].y-G[2].y; 
    V[2].z = G[0].z-G[2].z;
    V[2].mag = sqrt(V[2].x*V[2].x+V[2].y*V[2].y+V[2].z*V[2].z);

    // Calculate 
    M[0].x = G[0].x+((pow(D[0],2.0)+pow(V[0].mag,2.0)-pow(D[1],2.0))/(2*pow(V[0].mag,2.0)))*V[0].x;
    M[0].y = G[0].y+((pow(D[0],2.0)+pow(V[0].mag,2.0)-pow(D[1],2.0))/(2*pow(V[0].mag,2.0)))*V[0].y; 
    M[0].z = G[0].z+((pow(D[0],2.0)+pow(V[0].mag,2.0)-pow(D[1],2.0))/(2*pow(V[0].mag,2.0)))*V[0].z;
    // M[0].mag = sqrt(V[j].x*V[j].x+V[j].y*V[j].y+V[j].z*V[j].z);
    M[2].x = G[0].x+((pow(D[0],2.0)+pow(V[2].mag,2.0)-pow(D[2],2.0))/(2*pow(V[2].mag,2.0)))*V[2].x;
    M[2].y = G[0].y+((pow(D[0],2.0)+pow(V[2].mag,2.0)-pow(D[2],2.0))/(2*pow(V[2].mag,2.0)))*V[2].y; 
    M[2].z = G[0].z+((pow(D[0],2.0)+pow(V[2].mag,2.0)-pow(D[2],2.0))/(2*pow(V[2].mag,2.0)))*V[2].z;
    // M[1].mag = sqrt(V[j].x*V[j].x+V[j].y*V[j].y+V[j].z*V[j].z);
    M[1].x = G[1].x+((pow(D[1],2.0)+pow(V[1].mag,2.0)-pow(D[2],2.0))/(2*pow(V[1].mag,2.0)))*V[1].x;
	M[1].y = G[1].y+((pow(D[1],2.0)+pow(V[1].mag,2.0)-pow(D[2],2.0))/(2*pow(V[1].mag,2.0)))*V[1].y;
	M[1].z = G[1].z+((pow(D[1],2.0)+pow(V[1].mag,2.0)-pow(D[2],2.0))/(2*pow(V[1].mag,2.0)))*V[1].z;
    // M[2].mag = sqrt(V[2].x*V[2].x+V[2].y*V[2].y+V[2].z*V[2].z);

	// Finding Ns (NEED TO FINISH)
	float AC_AB = (V[0].x*V[2].x+V[0].y*V[2].y+V[0].z*V[2].z)/pow(V[0].mag,2.0);
	float AB_AC = (V[0].x*V[2].x+V[0].y*V[2].y+V[0].z*V[2].z)/pow(V[2].mag,2.0);
	float AB_BC = (V[0].x*V[1].x+V[0].y*V[1].y+V[0].z*V[1].z)/pow(V[1].mag,2.0);

	K[0].x = (V[2].x-(AC_AB)*V[1].x);
    K[0].y = (V[2].y-(AC_AB)*V[1].y); 
    K[0].z = (V[2].z-(AC_AB)*V[1].z);

    float mag = sqrt(pow(K[0].x,2.0)+pow(K[0].y,2.0)+pow(K[0].z,2.0));

	K[0].x = (V[2].x-(AC_AB)*V[1].x)/mag;
    K[0].y = (V[2].y-(AC_AB)*V[1].y)/mag; 
    K[0].z = (V[2].z-(AC_AB)*V[1].z)/mag;

    // M[0].mag = sqrt(V[j].x*V[j].x+V[j].y*V[j].y+V[j].z*V[j].z);
	K[2].x = (V[0].x-(AB_AC)*V[0].x);
    K[2].y = (V[0].y-(AB_AC)*V[0].y); 
    K[2].z = (V[0].z-(AB_AC)*V[0].z);

    mag = sqrt(pow(K[2].x,2.0)+pow(K[2].y,2.0)+pow(K[2].z,2.0));

	K[2].x = (V[0].x-(AB_AC)*V[0].x)/mag;
    K[2].y = (V[0].y-(AB_AC)*V[0].y)/mag; 
    K[2].z = (V[0].z-(AB_AC)*V[0].z)/mag;

    // M[1].mag = sqrt(V[j].x*V[j].x+V[j].y*V[j].y+V[j].z*V[j].z);
	K[1].x = (-V[1].x+(AB_BC)*V[1].x);
    K[1].y = (-V[1].y+(AB_BC)*V[1].y); 
    K[1].z = (-V[1].z+(AB_BC)*V[1].z);

    mag = sqrt(pow(K[2].x,2.0)+pow(K[2].y,2.0)+pow(K[2].z,2.0));

	K[1].x = (-V[1].x+(AB_BC)*V[1].x)/mag;
    K[1].y = (-V[1].y+(AB_BC)*V[1].y)/mag; 
    K[1].z = (-V[1].z+(AB_BC)*V[1].z)/mag;

    //// Sheet 4

    // AB
    At[0][0] = At[0][6] = Z[6][0] = Z[0][0] = K[0].x; 
    At[0][1] = At[0][7] = Z[7][0] = Z[1][0] = K[0].y; 
    At[0][2] = At[0][8] = Z[8][0] = Z[2][0] = K[0].z;

    // AC
    At[1][0] = Z[0][1] = (-1)*K[2].x; At[1][3] = Z[3][1] = (-1)*Z[0][1];
    At[1][1] = Z[1][1] = (-1)*K[2].y; At[1][4] = Z[4][1] = (-1)*Z[1][1];
    At[1][2] = Z[2][1] = (-1)*K[2].x; At[1][5] = Z[5][1] = (-1)*Z[1][1];

    // BC
    At[2][6] = At[2][3] = Z[6][2] = Z[3][2] = (-1)*K[1].x;
    At[2][7] = At[2][4] = Z[7][2] = Z[4][2] = (-1)*K[1].y;
    At[2][8] = At[2][5] = Z[8][2] = Z[5][2] = (-1)*K[1].z; 

    b[0] = M[2].x - M[0].x;
    b[1] = M[2].y - M[0].y;
    b[2] = M[2].z - M[0].z;

    b[3] = M[1].x - M[2].x;
    b[4] = M[1].y - M[2].y;
    b[5] = M[1].z - M[2].z;

    b[6] = M[1].x - M[0].x;
    b[7] = M[1].y - M[0].y;
    b[8] = M[1].z - M[0].z;

    for (int i = 3; i < 6; i++){ 
        At[0][i] = Z[i][0] = 0;
        At[2][i-3] = Z[i-3][2] = 0;
        At[3][i+3] = Z[i+3][3] = 0;
    }



    //// Taking Dot product
    /*
    	At = transpose matrix of A
    */
    // Initializing Matrix to zero
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            mat[i][j] = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 9; j++)
            mat2[i][j] = 0;
    for (int i = 0; i<3;i++)
        m_[i] = 0;
    // Dot product
    for (int r = 0; r < 3; r++)
    	for (int i = 0; i < 9; i++)
    		for (int k = 0; k < 3; k++)
    			mat[r][k] += At[r][i]*Z[i][k];

    //// Finding inverse matrix
    //finding determinant
	for(int i = 0; i < 3; i++){
		determinant = determinant + (mat[0][i] * (mat[1][(i+1)%3] * mat[2][(i+2)%3] - mat[1][(i+2)%3] * mat[2][(i+1)%3]));
    }
		
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			Inverse[i][j] = ((mat[(j+1)%3][(i+1)%3] * mat[(j+2)%3][(i+2)%3]) - (mat[(j+1)%3][(i+2)%3] * mat[(j+2)%3][(i+1)%3]))/ determinant;
        }
    }

    // dot product Inverse and At
    for (int k = 0; k < 3; k++)
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 9; j++)
                mat2[k][j]+=Inverse[k][i]*At[i][j];

    /*
        m_[0] = AB
        m_[1] = AC
        m_[2] = BC
    */
    // Dot product mat2 and b
    for (int k = 0; k < 3; k++)
        for (int i = 0; i < 9; i++)
            m_[k]+=mat2[k][i]*b[i]; 

    float M_AB_D = D[0]*sqrt(1-pow((pow(D[0],2.0)+pow(V[0].mag,2.0)-pow(D[1],2.0))/(2*V[0].mag*D[0]),2.0));    
    float M_AC_D = D[0]*sqrt(1-pow((pow(D[0],2.0)+pow(V[2].mag,2.0)-pow(D[2],2.0))/(2*V[2].mag*D[0]),2.0));
    float M_BC_D = D[1]*sqrt(1-pow((pow(D[1],2.0)+pow(V[1].mag,2.0)-pow(D[2],2.0))/(2*V[1].mag*D[1]),2.0));

    float ACxAB[] = {(V[2].y*V[0].z-V[0].y*V[2].z),(-1)*(V[2].x*V[0].z-V[2].z*V[0].x), V[2].x*V[0].y-V[2].y*V[0].x}; 
    float Mag_ACxAB = sqrt(pow(ACxAB[0],2.0)+pow(ACxAB[1],2.0)+pow(ACxAB[2],2.0));
    float N_D[] = {(ACxAB[0]/Mag_ACxAB),(ACxAB[1]/Mag_ACxAB),(ACxAB[2]/Mag_ACxAB)};

    Pos_P[0] = M[0].x+m_[0]*K[0].x+N_D[0]*sqrt(pow(M_AB_D,2.0)-pow(m_[0],2.0)); // x
    Pos_P[1] = M[0].y+m_[0]*K[0].y+N_D[0]*sqrt(pow(M_AB_D,2.0)-pow(m_[0],2.0)); // y
    Pos_P[2] = M[0].z+m_[0]*K[0].z+N_D[0]*sqrt(pow(M_AB_D,2.0)-pow(m_[0],2.0)); // z

    Neg_P[0] = M[0].x+m_[0]*K[0].x-N_D[0]*sqrt(pow(M_AB_D,2.0)-pow(m_[0],2.0));
    Neg_P[1] = M[0].y+m_[0]*K[0].y-N_D[0]*sqrt(pow(M_AB_D,2.0)-pow(m_[0],2.0)); 
    Neg_P[2] = M[0].z+m_[0]*K[0].z-N_D[0]*sqrt(pow(M_AB_D,2.0)-pow(m_[0],2.0));

    printf("Position of the Sat (%f, %f, %f) OR (%f, %f, %f)\n", Pos_P[0],Pos_P[1],Pos_P[2],Neg_P[0], Neg_P[1],Neg_P[2]);

    //// Converting to Lat, Long, and Alt
    Long = atan(Pos_P[0]/Pos_P[1])*180.0f/M_PI;
    // https://gssc.esa.int/navipedia/index.php/Ellipsoidal_and_Cartesian_Coordinates_Conversion
    lat = atan(Pos_P[3]/((1-pow(e,2.0))*sqrt(pow(Pos_P[0],2.0)+pow(Pos_P[1],2.0))))*180.0f/M_PI; 
    alt = sqrt(pow(Pos_P[0],2.0)+pow(Pos_P[1],2.0)+pow(Pos_P[2],2.0))-6371;

    printf("Position of the Sat Lat (%f, %f, %f) ", Long, lat, alt);

    Long = atan(Neg_P[0]/Neg_P[1])*180.0f/M_PI;
    // https://gssc.esa.int/navipedia/index.php/Ellipsoidal_and_Cartesian_Coordinates_Conversion
    lat = atan(Neg_P[3]/((1-pow(e,2.0))*sqrt(pow(Neg_P[0],2.0)+pow(Neg_P[1],2.0))))*180.0f/M_PI; 
    alt = sqrt(pow(Neg_P[0],2.0)+pow(Neg_P[1],2.0)+pow(Neg_P[2],2.0))-6371;

    printf("OR (%f, %f, %f)\n\n", Long, lat, alt);


}  



void Receiver::GPSPosition(){
	
}

void Receiver::OnExit()
{
	Keyboard_Cleanup();
}
