#include "Node.h"
#include <cstdio>

int main(int argc, char** argv)
{    
    Node::Get()->Init(argc, argv);
	Node::Get()->Loop();

	return 0;
}
