#include <Dragon.h>

class Sandbox : public Dragon::Application
{
public:
	Sandbox()
	{

	}
	
	~Sandbox()
	{

	}
};

Dragon::Application* Dragon::CreateApplication()
{
	return new Sandbox();
}