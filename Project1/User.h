#pragma once

using namespace System;

ref class User
{
private:
	Int32 ID;
	String ^username;
	String ^password;

public: //getters
	Int32 getID() { return ID; }
	String ^ getUsername() { return username; }

public: //constructors
	User() : ID(0) {}

	User(String ^newUsername, String ^newPassword)
		//:username(newUsername), password(newPassword) {}
		: User(0, newUsername, newPassword) {}

	User(int newID, String ^newUsername, String ^newPassword)
		: username(newUsername), password(newPassword), ID(newID) {}

public: //methods
	int CheckPassword(String^);
};

