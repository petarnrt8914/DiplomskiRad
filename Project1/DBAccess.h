#pragma once

using namespace System;


class LogRecord;

ref class DBAccess abstract
{
private:
	static System::Data::OleDb::OleDbConnection^ conn;
public:
	enum class Response {OK, NoSuchUser, WrongPassword, MultipleUsers, ConnectionFailed };

public:
	static void SetConn();

public:
	static Response LogIn(int& id, String^ name, String^ pass);
	static Response SignUp(int& id, String^ name, String^ pass);
	static Response UpdateLog(LogRecord*);
};

