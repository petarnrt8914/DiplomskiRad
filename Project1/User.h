#pragma once

using System::String;

ref class User
{
private:
	int ID;
	String ^username;
	String ^password;

public: //getters
	int getID() { return ID; }
	String^ getUsername() { return username; }

public: //constructors
	User() : ID(0) {}

	User(String ^newUsername, String ^newPassword)
		: User(0, newUsername, newPassword) {}

	User(int newID, String ^newUsername, String ^newPassword)
		: username(newUsername), password(newPassword), ID(newID) {}

public: //methods
	bool CheckPassword(String ^input) { return input == this->password; }
};

