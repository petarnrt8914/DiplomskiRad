#include "DBAccess.h"

using namespace System;
using namespace System::Data;
using namespace System::Data::OleDb;

using System::Security::Cryptography::SHA256;
using System::Windows::Forms::OpenFileDialog;
using System::Windows::Forms::MessageBox;

#define DB_PATH LR"(..\Project1\Log (encrypted passwords).mdb)"	//change when deployed


bool DBAccess::OpenConnection() {
	if (conn->State == ConnectionState::Open) return true;

	try {
		conn->Open();
		return true;
	}
	catch (Exception^ ex){
		return SetConn();
	}
}

bool DBAccess::SetConn() {
	//inicijalizuj konekciju
	OleDbConnectionStringBuilder ^connBuilder = gcnew OleDbConnectionStringBuilder();
	connBuilder->Provider = "Microsoft.ACE.OLEDB.12.0";
	connBuilder->DataSource = DB_PATH;
	if (conn == nullptr) {
		//conn = gcnew OleDbConnection("Provider=Microsoft.ACE.OLEDB.12.0; Data Source="+DB_PATH);
		conn = gcnew OleDbConnection(connBuilder->ToString());
	}

	if (conn->State == ConnectionState::Open) return true;

	//ako ne uspe, naci bazu koristici FileDialog
	for (int i = 0; i<3; i++) {
		try {
			conn->Open(); conn->Close();
			return true;
		}
		catch (Exception^) {
			MessageBox::Show("Baza podataka nije nađena, nađite je manualno.");
			OpenFileDialog ^openDB = gcnew OpenFileDialog();
			openDB->InitialDirectory = "..\\";
			if (openDB->ShowDialog() == Windows::Forms::DialogResult::Cancel) return false;
			connBuilder->DataSource = openDB->FileName;
			conn->ConnectionString = connBuilder->ToString();
		}
	}
	try {
		conn->Open(); conn->Close();
		return true;
	}
	catch (Exception^) {
		return false;
	}
}


	// mozda odmah da trazi i da se password poklapa (user enumeration prevention)
	// a mozda i ne, jer je SignUp jednostavan
DBAccess::Response DBAccess::LogIn(int& userID, String ^ username, String ^ password) {

	String ^selectText = "SELECT ID, password FROM [USER] WHERE username=?";
	OleDbCommand ^cmdSelectUser = gcnew OleDbCommand(selectText, conn);
	cmdSelectUser->Parameters->AddWithValue("username", username);
	OleDbDataReader^ reader;

	if (OpenConnection()) {
		reader = cmdSelectUser->ExecuteReader();
		Response resp = Response::NoSuchUser; // default
		while (reader->Read()) {
			if (userID) {
				resp = Response::MultipleUsers;	// ne bi trebalo da se desi
				break;
			}
			else if (EncryptPassword(password)==reader["password"]->ToString()) {
				userID = static_cast<int>(reader["ID"]);
				resp = Response::OK;
			}
			//else if (password==reader["password"]->ToString()) {
			//	userID = static_cast<int>(reader["ID"]);
			//	resp = Response::OK;
			//}
			else
				resp = Response::WrongPassword;
		}

		delete reader, cmdSelectUser;
		conn->Close();
		return resp;
	}
	else {
		return Response::ConnectionFailed;
	}
}

String^ DBAccess::EncryptPassword(String^ password) {
	//odnos w_char i char
	static unsigned char ratio = sizeof(wchar_t)/sizeof(unsigned char);
	array<unsigned char>^ passwordBytes = gcnew array<unsigned char>(password->Length * ratio);
	auto encoder = gcnew Text::UnicodeEncoding();
	int size = encoder->GetBytes(password, 0, password->Length, passwordBytes, 0);
	SHA256^ hasher = SHA256::Create();
	//hasher->ComputeHash(passwordBytes)
	Text::StringBuilder^ result = gcnew Text::StringBuilder(65);
	auto bytesOfData = hasher->ComputeHash(passwordBytes);
	for each (unsigned char c in bytesOfData) {
		result->AppendFormat("{0:X2}", c); //append every char as 2 hex digits
	}
	return result->ToString();
}

//mozda da napravim da se korisnik uloguje automatski cim se registruje,
// u suprotnom, userID je suvisan
DBAccess::Response DBAccess::SignUp(int& userID, String ^ username, String ^ password) {
	Response resp;
	String ^selectText = "INSERT INTO [USER] ([username], [password]) VALUES (?, ?)";
	OleDbCommand ^cmdSelectUser = gcnew OleDbCommand(selectText, conn);
	cmdSelectUser->Parameters->AddWithValue("username", username);
	cmdSelectUser->Parameters->AddWithValue("password", EncryptPassword(password));
	OleDbDataReader^ reader;
	if (OpenConnection()) {
		try {
			resp = cmdSelectUser->ExecuteNonQuery()==1 ? Response::OK : Response::ConnectionFailed;
			return resp;
		}
		finally {
			delete reader, cmdSelectUser;
			conn->Close();
		}
	}
	else return Response::ConnectionFailed;
}


DBAccess::Response DBAccess::UpdateLog(LogRecord *) {
	throw gcnew NotImplementedException();
}
