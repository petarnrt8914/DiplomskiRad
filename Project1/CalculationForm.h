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
	using System::Collections::Generic::Dictionary;

	/// <summary>
	/// Summary for CalculationForm
	/// </summary>
	public ref class CalculationForm : public System::Windows::Forms::Form
	{
	protected:
		int userID;
		Dictionary<String^,int>^ mathOperations;
		//get users

	public:
		CalculationForm(int userID) {
			InitializeComponent();

			this->userID = userID;
			if (DBAccess::ReadMathOperations(mathOperations)==DBAccess::Response::OK) {
				//mozda nesto
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CalculationForm() {
			if (components) delete components;
		}
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabLagrangeInterpolation;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::Button^  btnAddPoint;
	private: System::Windows::Forms::Button^  btnDeletePoints;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  txtNewPointY;
	private: System::Windows::Forms::TextBox^  txtNewPointX;
	private: System::Windows::Forms::ListBox^  listPoints;
	private: System::Windows::Forms::Panel^  pnlGraph;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuAddPoint;
	private: System::Windows::Forms::ToolStripMenuItem^  itemAddPointHere;
	private: System::Windows::Forms::ToolStripMenuItem^  itemCancel;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


		#pragma region Windows Form Designer generated code
				/// <summary>
				/// Required method for Designer support - do not modify
				/// the contents of this method with the code editor.
				/// </summary>
		void InitializeComponent(void) {
			this->components = (gcnew System::ComponentModel::Container());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabLagrangeInterpolation = (gcnew System::Windows::Forms::TabPage());
			this->pnlGraph = (gcnew System::Windows::Forms::Panel());
			this->contextMenuAddPoint = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->itemAddPointHere = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->itemCancel = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->btnAddPoint = (gcnew System::Windows::Forms::Button());
			this->btnDeletePoints = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txtNewPointY = (gcnew System::Windows::Forms::TextBox());
			this->txtNewPointX = (gcnew System::Windows::Forms::TextBox());
			this->listPoints = (gcnew System::Windows::Forms::ListBox());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->tabControl1->SuspendLayout();
			this->tabLagrangeInterpolation->SuspendLayout();
			this->contextMenuAddPoint->SuspendLayout();
			this->SuspendLayout();
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabLagrangeInterpolation);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl1->Location = System::Drawing::Point(0, 0);
			this->tabControl1->Margin = System::Windows::Forms::Padding(4);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(552, 363);
			this->tabControl1->TabIndex = 0;
			// 
			// tabLagrangeInterpolation
			// 
			this->tabLagrangeInterpolation->BackColor = System::Drawing::SystemColors::Control;
			this->tabLagrangeInterpolation->Controls->Add(this->pnlGraph);
			this->tabLagrangeInterpolation->Controls->Add(this->btnAddPoint);
			this->tabLagrangeInterpolation->Controls->Add(this->btnDeletePoints);
			this->tabLagrangeInterpolation->Controls->Add(this->label2);
			this->tabLagrangeInterpolation->Controls->Add(this->label1);
			this->tabLagrangeInterpolation->Controls->Add(this->txtNewPointY);
			this->tabLagrangeInterpolation->Controls->Add(this->txtNewPointX);
			this->tabLagrangeInterpolation->Controls->Add(this->listPoints);
			this->tabLagrangeInterpolation->Location = System::Drawing::Point(4, 25);
			this->tabLagrangeInterpolation->Margin = System::Windows::Forms::Padding(4);
			this->tabLagrangeInterpolation->Name = L"tabLagrangeInterpolation";
			this->tabLagrangeInterpolation->Padding = System::Windows::Forms::Padding(15);
			this->tabLagrangeInterpolation->Size = System::Drawing::Size(544, 334);
			this->tabLagrangeInterpolation->TabIndex = 0;
			this->tabLagrangeInterpolation->Text = L"Lagranžova interpolacija";
			// 
			// pnlGraph
			// 
			this->pnlGraph->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
																																									 | System::Windows::Forms::AnchorStyles::Left)
																																									| System::Windows::Forms::AnchorStyles::Right));
			this->pnlGraph->BackColor = System::Drawing::SystemColors::ControlLightLight;
			this->pnlGraph->ContextMenuStrip = this->contextMenuAddPoint;
			this->pnlGraph->Location = System::Drawing::Point(194, 18);
			this->pnlGraph->Name = L"pnlGraph";
			this->pnlGraph->Size = System::Drawing::Size(332, 298);
			this->pnlGraph->TabIndex = 7;
			// 
			// contextMenuAddPoint
			// 
			this->contextMenuAddPoint->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->itemAddPointHere,
					this->itemCancel
			});
			this->contextMenuAddPoint->Name = L"contextMenuAddPoint";
			this->contextMenuAddPoint->Size = System::Drawing::Size(167, 48);
			// 
			// itemAddPointHere
			// 
			this->itemAddPointHere->Name = L"itemAddPointHere";
			this->itemAddPointHere->Size = System::Drawing::Size(166, 22);
			this->itemAddPointHere->Text = L"Dodaj tačku ovde";
			// 
			// itemCancel
			// 
			this->itemCancel->Name = L"itemCancel";
			this->itemCancel->Size = System::Drawing::Size(166, 22);
			this->itemCancel->Text = L"Otkaži";
			// 
			// btnAddPoint
			// 
			this->btnAddPoint->Enabled = false;
			this->btnAddPoint->Location = System::Drawing::Point(22, 66);
			this->btnAddPoint->Name = L"btnAddPoint";
			this->btnAddPoint->Size = System::Drawing::Size(154, 27);
			this->btnAddPoint->TabIndex = 2;
			this->btnAddPoint->Text = L"Dodaj tačku";
			this->btnAddPoint->UseVisualStyleBackColor = true;
			this->btnAddPoint->Click += gcnew System::EventHandler(this, &CalculationForm::btnAddPoint_Click);
			// 
			// btnDeletePoints
			// 
			this->btnDeletePoints->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->btnDeletePoints->Enabled = false;
			this->btnDeletePoints->Location = System::Drawing::Point(22, 287);
			this->btnDeletePoints->Margin = System::Windows::Forms::Padding(4);
			this->btnDeletePoints->Name = L"btnDeletePoints";
			this->btnDeletePoints->Size = System::Drawing::Size(154, 28);
			this->btnDeletePoints->TabIndex = 4;
			this->btnDeletePoints->Text = L"Obriši izabrane tačke";
			this->btnDeletePoints->UseVisualStyleBackColor = true;
			this->btnDeletePoints->Click += gcnew System::EventHandler(this, &CalculationForm::btnDeletePoints_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(129, 15);
			this->label2->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(28, 17);
			this->label2->TabIndex = 4;
			this->label2->Text = L"f(x)";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(48, 15);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(14, 17);
			this->label1->TabIndex = 3;
			this->label1->Text = L"x";
			// 
			// txtNewPointY
			// 
			this->txtNewPointY->Location = System::Drawing::Point(110, 36);
			this->txtNewPointY->Margin = System::Windows::Forms::Padding(4);
			this->txtNewPointY->Name = L"txtNewPointY";
			this->txtNewPointY->Size = System::Drawing::Size(66, 23);
			this->txtNewPointY->TabIndex = 1;
			this->txtNewPointY->TextChanged += gcnew System::EventHandler(this, &CalculationForm::txtNewPointY_TextChanged);
			this->txtNewPointY->PreviewKeyDown += gcnew System::Windows::Forms::PreviewKeyDownEventHandler(this, &CalculationForm::txtNewPoint_PreviewKeyDown);
			// 
			// txtNewPointX
			// 
			this->txtNewPointX->Location = System::Drawing::Point(22, 36);
			this->txtNewPointX->Margin = System::Windows::Forms::Padding(4);
			this->txtNewPointX->Name = L"txtNewPointX";
			this->txtNewPointX->Size = System::Drawing::Size(66, 23);
			this->txtNewPointX->TabIndex = 0;
			this->txtNewPointX->TextChanged += gcnew System::EventHandler(this, &CalculationForm::txtNewPointX_TextChanged);
			this->txtNewPointX->PreviewKeyDown += gcnew System::Windows::Forms::PreviewKeyDownEventHandler(this, &CalculationForm::txtNewPoint_PreviewKeyDown);
			// 
			// listPoints
			// 
			this->listPoints->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
																																										| System::Windows::Forms::AnchorStyles::Left));
			this->listPoints->FormattingEnabled = true;
			this->listPoints->ItemHeight = 16;
			this->listPoints->Location = System::Drawing::Point(22, 100);
			this->listPoints->Margin = System::Windows::Forms::Padding(4);
			this->listPoints->Name = L"listPoints";
			this->listPoints->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->listPoints->Size = System::Drawing::Size(154, 180);
			this->listPoints->Sorted = true;
			this->listPoints->TabIndex = 3;
			this->listPoints->SelectedIndexChanged += gcnew System::EventHandler(this, &CalculationForm::listPoints_SelectedIndexChanged);
			// 
			// tabPage2
			// 
			this->tabPage2->Location = System::Drawing::Point(4, 25);
			this->tabPage2->Margin = System::Windows::Forms::Padding(4);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(4);
			this->tabPage2->Size = System::Drawing::Size(544, 334);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Njutnova interpolacija";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// CalculationForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(552, 363);
			this->Controls->Add(this->tabControl1);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
																								static_cast<System::Byte>(254)));
			this->Margin = System::Windows::Forms::Padding(4);
			this->MinimumSize = System::Drawing::Size(515, 345);
			this->Name = L"CalculationForm";
			this->Text = L"Matematička izračunavanja";
			this->tabControl1->ResumeLayout(false);
			this->tabLagrangeInterpolation->ResumeLayout(false);
			this->tabLagrangeInterpolation->PerformLayout();
			this->contextMenuAddPoint->ResumeLayout(false);
			this->ResumeLayout(false);

		}
		#pragma endregion
	private:
		System::Void txtNewPointX_TextChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void txtNewPointY_TextChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void btnAddPoint_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void listPoints_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void btnDeletePoints_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void txtNewPoint_PreviewKeyDown(System::Object^  sender, System::Windows::Forms::PreviewKeyDownEventArgs^  e);
};
}
