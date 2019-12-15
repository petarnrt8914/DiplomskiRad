#pragma once

using namespace System;

class LogRecord;

ref class DBAccess abstract
{
private:
	static System::Data::OleDb::OleDbConnection^ conn;
public:
	enum class Response {	
		OK=0, 
		NoSuchUser=1, 
		WrongPassword=2, 
		MultipleUsers=4, 
		ConnectionFailed=8
	};

private:
	static bool OpenConnection();

public:
	static bool SetConn();

public:
	static Response LogIn(int& id, String^ name, String^ pass);
	static Response SignUp(int& id, String^ name, String^ pass);
	static Response UpdateLog(LogRecord*);
};

