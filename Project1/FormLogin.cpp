#include "FormLogin.h"
#include "DBAccess.h"
#include "CalculationForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace DiplomskiRad;

inline Void FormLogin::btnLogin_Click(Object ^sender, EventArgs ^e) {
	String ^username = txtUsername->Text,
		     ^password = txtPassword->Text;
	int id = 0;

	if( ! IsUsernameValid(username)) {
		MessageBox::Show("Korisničko ime nije validno.\n"+
										 "-Mora počinjati slovom\n"+
										 "-imati 3-25 znakova\n"+
										 "-završavati se alfanumeričkim znakom\n"+
										 "-i može imati jedan od znakova (.-_) između alfanumeričkih");
		return;
	}

	DBAccess::Response response = DBAccess::LogIn(id, username, password);

	switch (response) {
	case DBAccess::Response::NoSuchUser: {
		auto answer = MessageBox::Show("Nema korisnika sa unetim korisničkim imenom.\n" +
																	 "Želite li da se registrujete sa unetim podacima?",
																	 "Nepostojeće korisničko ime", MessageBoxButtons::YesNo );
		if (answer == System::Windows::Forms::DialogResult::Yes) { // neverovatno
			response = DBAccess::SignUp(id, username, password);
			if (response == DBAccess::Response::OK)
				MessageBox::Show("Registracija uspešna. Možete da se prijavite.");
			else
				HandleResponse(response);
			break;
		}
		else {
			txtPassword->ResetText();
			txtUsername->Focus();
			txtUsername->SelectAll();
			break;
		}
	}
	case DBAccess::Response::OK: {
		// open calculation form with current user ID
		//MessageBox::Show("Opening with userID "+id);
		this->Hide();
		Form ^f = gcnew CalculationForm(id);
		f->ShowDialog();
		delete f;
		this->Show();
		break;
	}
	default:
		HandleResponse(response);
	}
}

inline void FormLogin::HandleResponse(DBAccess::Response response) {
	String ^msgText, ^msgTitle = "Neuspešno logovanje";
	switch (response) {
	case DBAccess::Response::WrongPassword:
		msgText = "Lozinka koju ste uneli nije tačna.";
		txtPassword->Focus();
		txtPassword->SelectAll();
		break;
	case DBAccess::Response::MultipleUsers:
		msgText = "Greška! Postoji više korisnika sa datim korisničkim imenom. Javite administratoru.";
		break;
	case DBAccess::Response::ConnectionFailed:
		msgText = "Konekcija neuspešna...";
		break;
	default:
		msgText = "Nemoguć scenario";
		break;
	}
	
	MessageBox::Show(msgText, msgTitle, MessageBoxButtons::OK, MessageBoxIcon::Error);
}

inline bool FormLogin::IsUsernameValid(String ^ username) {
	return usernameRegex->IsMatch(username) && username->Length <= 25 && username->Length >= 3;
	//try {
	//	usernameValidator->Validate(username);
	//	return username->Length <= 25 && username->Length >= 3;
	//}
	//catch (Exception^) {
	//	return false;
	//}
}

inline Void DiplomskiRad::FormLogin::FormLogin_Shown(Object ^ sender, EventArgs ^ e) {
	//making sure the window appears on top
	this->TopMost = true;
	this->TopMost = false;
}

