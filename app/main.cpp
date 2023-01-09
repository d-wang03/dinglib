#include "simmsgbox.h"
#include "ddispatcher.h"
#include <thread>

using namespace ding;
using namespace std::chrono_literals;
int main()
{
	//create msg boxes
	uint8_t id1 = 1, id2 = 2;
	auto msgbox1 = makeObject<SimMsgBox>(id1);
	auto msgbox2 = makeObject<SimMsgBox>(id2);
	//create dispatcher
	auto dispatcher = makeObject<DDispatcher>();
	dispatcher->addRoutePath(new SimMsg(id1), &DDispatcher::output<0>);
	dispatcher->addRoutePath(new SimMsg(id2), &DDispatcher::output<1>);
	//connect
	connect(msgbox1, &SimMsgBox::sendData, dispatcher, &DDispatcher::input);
	connect(msgbox2, &SimMsgBox::sendData, dispatcher, &DDispatcher::input);
	connect(dispatcher, &DDispatcher::output<0>, msgbox1, &SimMsgBox::received);
	connect(dispatcher, &DDispatcher::output<1>, msgbox2, &SimMsgBox::received);

	//start
	dispatcher->start();

	//put messages
	Msg msg;
	for (int i = 0; i < 10; ++i)
	{
		msg.src = (i%2)? id1 : id2;
		msg.dst = (i%2)? id2 : id1;
		msg.length = 3;
		msg.data[0] = i;
		msg.data[1] = i + 1;
		msg.data[2] = i + 2;
		msgbox1->send(msg);
		msgbox2->send(msg);
	}
    std::this_thread::sleep_for(500ms);
    dispatcher->stop();
	return 0;
}
