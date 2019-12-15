#pragma once
#include "DBAccess.h"


namespace DiplomskiRad
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Data::OleDb;
	using namespace System::Text::RegularExpressions;

	/// <summary>
	/// Summary for FormLogin
	/// </summary>
	public ref class FormLogin : public System::Windows::Forms::Form
	{
	protected:
		OleDbConnection ^conn;
		static Regex ^usernameRegex = gcnew Regex("[a-zA-Z]([-_.]?[a-zA-Z0-9])+");

	public:
		FormLogin(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			DBAccess::SetConn();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormLogin()
		{
			if (conn) {
				if(conn->State != ConnectionState::Closed) conn->Close();
				delete conn;
			}
			if (components) delete components;
		}


	private: System::Windows::Forms::Label^  lblUsername;
	private: System::Windows::Forms::Label^  lblPassword;
	private: System::Windows::Forms::TextBox^  txtUsername;
	private: System::Windows::Forms::TextBox^  txtPassword;
	private: System::Windows::Forms::Button^  btnLogin;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->btnLogin = (gcnew System::Windows::Forms::Button());
			this->txtUsername = (gcnew System::Windows::Forms::TextBox());
			this->txtPassword = (gcnew System::Windows::Forms::TextBox());
			this->lblUsername = (gcnew System::Windows::Forms::Label());
			this->lblPassword = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// btnLogin
			// 
			this->btnLogin->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
																																									| System::Windows::Forms::AnchorStyles::Right));
			this->btnLogin->Location = System::Drawing::Point(155, 122);
			this->btnLogin->Name = L"btnLogin";
			this->btnLogin->Size = System::Drawing::Size(161, 28);
			this->btnLogin->TabIndex = 0;
			this->btnLogin->Text = L"Prijavi se";
			this->btnLogin->UseVisualStyleBackColor = true;
			this->btnLogin->Click += gcnew System::EventHandler(this, &FormLogin::btnLogin_Click);
			// 
			// txtUsername
			// 
			this->txtUsername->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
																																										 | System::Windows::Forms::AnchorStyles::Right));
			this->txtUsername->Location = System::Drawing::Point(155, 34);
			this->txtUsername->Margin = System::Windows::Forms::Padding(5, 4, 5, 4);
			this->txtUsername->Name = L"txtUsername";
			this->txtUsername->Size = System::Drawing::Size(161, 24);
			this->txtUsername->TabIndex = 1;
			this->txtUsername->Text = L"petarnrt8914";
			// 
			// txtPassword
			// 
			this->txtPassword->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
																																										 | System::Windows::Forms::AnchorStyles::Right));
			this->txtPassword->Location = System::Drawing::Point(155, 78);
			this->txtPassword->Name = L"txtPassword";
			this->txtPassword->Size = System::Drawing::Size(161, 24);
			this->txtPassword->TabIndex = 2;
			this->txtPassword->Text = L"gmxt1016";
			this->txtPassword->UseSystemPasswordChar = true;
			// 
			// lblUsername
			// 
			this->lblUsername->AutoSize = true;
			this->lblUsername->Location = System::Drawing::Point(40, 37);
			this->lblUsername->Name = L"lblUsername";
			this->lblUsername->Size = System::Drawing::Size(110, 17);
			this->lblUsername->TabIndex = 3;
			this->lblUsername->Text = L"Korisnično ime";
			// 
			// lblPassword
			// 
			this->lblPassword->AutoSize = true;
			this->lblPassword->Location = System::Drawing::Point(40, 81);
			this->lblPassword->Name = L"lblPassword";
			this->lblPassword->Size = System::Drawing::Size(61, 17);
			this->lblPassword->TabIndex = 4;
			this->lblPassword->Text = L"Lozinka";
			// 
			// FormLogin
			// 
			this->AcceptButton = this->btnLogin;
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(357, 188);
			this->Controls->Add(this->lblPassword);
			this->Controls->Add(this->lblUsername);
			this->Controls->Add(this->txtPassword);
			this->Controls->Add(this->txtUsername);
			this->Controls->Add(this->btnLogin);
			this->Font = (gcnew System::Drawing::Font(L"Verdana", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
																								static_cast<System::Byte>(254)));
			this->Margin = System::Windows::Forms::Padding(5, 4, 5, 4);
			this->MaximumSize = System::Drawing::Size(500, 258);
			this->MinimumSize = System::Drawing::Size(300, 227);
			this->Name = L"FormLogin";
			this->Text = L"FormLogin";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void btnLogin_Click(System::Object^  sender, System::EventArgs^  e);
					 void HandleResponse(DBAccess::Response);
					 bool IsUsernameValid(String^);
};
}
