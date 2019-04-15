#include "Node.h"
#include "SerialObject.h"
#include <ros/ros.h>
#include "std_msgs/ByteMultiArray.h"

// --------- ROS Topic Implementation ----------

class Topic
{
	std::string name;
	SerialObject* object;

	char* sendBuffer;
	char* recvBuffer;
	std_msgs::ByteMultiArray sendMsg;

	ros::Publisher pub;
	ros::Subscriber sub;

public:
	Topic(std::string topicName, SerialObject* topicObject);

	std::string TopicName() { return(name); }
	SerialObject& TopicObject() { return(*object); }

	void Publish();
    void Callback(const std_msgs::ByteMultiArray::ConstPtr& msg);

	ros::Publisher& Publisher() { return(pub); }
	ros::Subscriber& Subscriber() { return(sub); }
};

Topic::Topic(std::string topicName, SerialObject* topicObject)
{
	name = topicName;
	object = topicObject;
	sendBuffer = new char[topicObject->GetObjectSize()];	// allocate buffer space
	recvBuffer = new char[topicObject->GetObjectSize()];	// allocate buffer space
}

void Topic::Publish()
{
	object->Serialize(sendBuffer);  // obtain serialized data buffer	

	sendMsg.data.clear();								// clear existing data
	for (int i = 0; i < object->GetObjectSize(); i++)	// fill message buffer
		sendMsg.data.push_back(sendBuffer[i]);

	pub.publish(sendMsg); 	// send message

}

void Topic::Callback(const std_msgs::ByteMultiArray::ConstPtr& msg)
{
    for(int i = 0; i < object->GetObjectSize(); i++)	// Convert msg vector to char array
	  recvBuffer[i] = (char)msg->data[i];

    object->Deserialize(recvBuffer);	// Deserialize data
	
	Node::Get()->CallInputFunction(name); // Notify input function    

}

// ---------------------------------------------



// --------- ROS Node Implementation ----------

ros::NodeHandle* nodeHandle = NULL;
Node* Node::instance = NULL;

Node* Node::Get()
{
	if (instance == NULL)
	{
		instance = CreateApplicationNode();
		return(instance);
	}
	else
	{
		return(instance);
	}
}


Node::Node()
{
	terminate = false;
}


Node::~Node()
{
	delete nodeHandle;
}


void Node::Setup(int argc, char** argv)
{
}


void Node::SetNodeName(int argc, char** argv, std::string& nodeName)
{
	nodeName = ("Unnamed");		// Default Node Name
}

void Node::Init(int argc, char** argv)
{	
	SetNodeName(argc, argv, _nodeName);

	ros::init(argc, argv, _nodeName);		// init ros system
	
	nodeHandle = new ros::NodeHandle();		// create node handle

	Setup(argc,argv);		// call to setup application-specific initialization

	// sleep(5);	// wait up bit....

	for (auto func = initFunctions.begin(); func != initFunctions.end(); func++)	// call init functions from this class
	  (Node::Get()->*(*func)) ();
}

void Node::Loop()
{
	while (terminate == false)	// Loop endlessly until terminated
	{
		ros::spinOnce();		// query ROS to process callbacks

		for (auto func = coreFunctions.begin(); func != coreFunctions.end(); func++)	// call all core functions
	  	  (Node::Get()->*(*func)) ();

		for (auto topic = publishers.begin(); topic != publishers.end(); topic++)	// call all publishers with flagged data
		{
			if ((*topic)->TopicObject().GetFlagged())
			{
				(*topic)->TopicObject().SetFlagged(false);
				(*topic)->Publish();
			}
		}
	}

	for (auto func = exitFunctions.begin(); func != exitFunctions.end(); func++)	// call all exit functions before control loop exits
		(Node::Get()->*(*func)) ();
}


void Node::Terminate()
{
	terminate = true;
}

void Node::Subscribe(std::string topicName, SerialObject* object)
{
	Topic* t = new Topic(topicName, object);
	t->Subscriber() = nodeHandle->subscribe(topicName, 1000, &Topic::Callback, t);
	subscriptions.push_back(t);
}

void Node::Publish(std::string topicName, SerialObject* object)
{
	Topic* t = new Topic(topicName, object);
	t->Publisher() = nodeHandle->advertise<std_msgs::ByteMultiArray>(topicName, 1000);
	publishers.push_back(t);
}


void Node::RegisterInitFunction(NodeFuncPtr f)
{
	initFunctions.push_back(f);
}

void Node::RegisterInputFunction(std::string topicName, NodeFuncPtr f)
{
	inputFunctions.insert(std::make_pair(topicName, f));		// add input function if NOT registered
}

void Node::RegisterCoreFunction(NodeFuncPtr f)
{
	coreFunctions.push_back(f);
}

void Node::RegisterExitFunction(NodeFuncPtr f)
{
	exitFunctions.push_back(f);
}

void Node::CallInputFunction(std::string topicName)
{
	if(inputFunctions.find(topicName) != inputFunctions.end()) // call input function if registered
	  (Node::Get()->*(inputFunctions[topicName])) ();
}


std::string Node::FindTopicName(std::string parameterName)
{
	std::string topicName;

	if(ros::param::has("~" + parameterName))
		ros::param::get("~" + parameterName, topicName);
	else
		topicName = "";

	return(topicName);
}

// ---------------------------------------------

