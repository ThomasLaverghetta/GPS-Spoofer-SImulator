#pragma once

#include <string>
#include <vector>
#include <map>

class Node;
class SerialObject;
class Topic;
typedef void (Node::*NodeFuncPtr)(void);


// IMPORTANT!! - Make sure there is a definition for CreateApplicationNode() in derivative applications
// --- This function should return a new instance of the class deriving from Node.
// --- This function is used when creating the Node singleton.
extern Node* CreateApplicationNode(); 


class Node
{
 protected:
	Node();
	~Node();
	static Node* instance;

private:
	friend class Topic;

	bool terminate;
	std::string _nodeName;

	std::vector<NodeFuncPtr> initFunctions;
	std::map<std::string, NodeFuncPtr> inputFunctions;
	std::vector<NodeFuncPtr> coreFunctions;
	std::vector<NodeFuncPtr> exitFunctions;
	std::vector<Topic*> subscriptions;
	std::vector<Topic*> publishers;

	void CallInputFunction(std::string topicName);

public:
	static Node* Get();
	void Init(int argc, char** argv);
	void Loop();
	void Terminate();

	
protected:
	virtual void Setup(int argc, char** argv);	
	virtual void SetNodeName(int argc, char** argv, std::string& nodeName);

	void Subscribe(std::string topicName, SerialObject* object);
	void Publish(std::string topicName, SerialObject* object);

	void RegisterInitFunction(NodeFuncPtr f);
	void RegisterInputFunction(std::string topicName, NodeFuncPtr f);
	void RegisterCoreFunction(NodeFuncPtr f);
	void RegisterExitFunction(NodeFuncPtr f);

	std::string FindTopicName(std::string parameterName);
};

