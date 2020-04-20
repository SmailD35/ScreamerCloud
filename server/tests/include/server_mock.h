#ifndef SERVER_SERVER_MOCK_H
#define SERVER_SERVER_MOCK_H

class Command
{
public:

	virtual void Do();
	virtual void Undo();

};


class Invoker
{
public:

	virtual void Do();
	virtual void Undo();
	virtual void SetCommand(Command command);
};

class InvokerMock : public Invoker
{
public:

	MOCK_METHOD(void, Do, (), (override));
	MOCK_METHOD(void, Undo, (), (override));
	MOCK_METHOD(void, SetCommand, (Command command), (override));
};


class RegisterUserCommand : Command
{
public:

	virtual void Do();
	virtual void Undo();
};

class RegisterUserCommandMock : public RegisterUserCommand
{
public:

	MOCK_METHOD(void, Do, (), (override));
	MOCK_METHOD(void, Undo, (), (override));
};


class SendFileCommand : Command
{
public:

	virtual void Do();
	virtual void Undo();
};

class SendFileCommandMock : public SendFileCommand
{
public:

	MOCK_METHOD(void, Do, (), (override));
	MOCK_METHOD(void, Undo, (), (override));
};


class RecvFileCommand : public Command
{
public:

	virtual void Do();
	virtual void Undo();
};

class RecvFileCommandMock : public RecvFileCommand
{
public:

	MOCK_METHOD(void, Do, (), (override));
	MOCK_METHOD(void, Undo, (), (override));
};


class SendFileListCommand : public Command
{
public:

	virtual void Do();
	virtual void Undo();
};

class SendFileListCommandMock : public SendFileListCommand
{
public:

	MOCK_METHOD(void, Do, (), (override));
	MOCK_METHOD(void, Undo, (), (override));
};


class Server
{
public:

	virtual void ConnectionsLoop();
	virtual void QueriesLoop();
};

class ServerMock : public Server
{
public:

	MOCK_METHOD(void, QueriesLoop, (), (override));
	MOCK_METHOD(void, ConnectionsLoop, (), (override));
};

#endif //SERVER_SERVER_MOCK_H
