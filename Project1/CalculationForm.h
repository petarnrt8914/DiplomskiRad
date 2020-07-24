#pragma once
#include "pch.h"
#include "DBAccess.h"
#include "TrangularMatrix.h"

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
	public:
		enum class InterpolationMethod { None, Lagrange, Newton, Both };

	protected:
		int userID;
		Dictionary<String^,int>^ mathOperations;
		//MAYBE get users
		PointF	^L_Interpolated, ^N_Interpolated,	// for keeping current interpolated points
						^L_Normalized, ^N_Normalized;			// for drawing without recalculating, TODO check if I'm using it at all
		//MAYBE
		array<PointF>^ const interpolatedPoints; // 0: Lagrange, 1: Newton
		array<PointF>^ normalizedPoints;

		TriangularMatrix^ DividedDifferenceTable;
		array<double>^ reciprocalBaricentricWeights;
	public:
		CalculationForm(int userID)
			: userID(userID), interpolatedPoints(gcnew array<PointF>(2))
		{
			InitializeComponent();

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
	private: System::Windows::Forms::TabPage^  tabInterpolation;

	private: System::Windows::Forms::Button^  btnAddPointOrInterpolate;
	private: System::Windows::Forms::Button^  btnDeletePoints;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  txtNewPointY;
	private: System::Windows::Forms::TextBox^  txtNewPointX;
	private: System::Windows::Forms::ListBox^  listPoints;
	private: System::Windows::Forms::Panel^  pnlGraphArea;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::RadioButton^  rbNewtonMethod;
	private: System::Windows::Forms::RadioButton^  rbLagrangeMethod;
	private: System::Windows::Forms::RadioButton^  rbBothInterpolations;
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
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabInterpolation = (gcnew System::Windows::Forms::TabPage());
			this->rbBothInterpolations = (gcnew System::Windows::Forms::RadioButton());
			this->rbNewtonMethod = (gcnew System::Windows::Forms::RadioButton());
			this->rbLagrangeMethod = (gcnew System::Windows::Forms::RadioButton());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->pnlGraphArea = (gcnew System::Windows::Forms::Panel());
			this->btnAddPointOrInterpolate = (gcnew System::Windows::Forms::Button());
			this->btnDeletePoints = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txtNewPointY = (gcnew System::Windows::Forms::TextBox());
			this->txtNewPointX = (gcnew System::Windows::Forms::TextBox());
			this->listPoints = (gcnew System::Windows::Forms::ListBox());
			this->tabControl1->SuspendLayout();
			this->tabInterpolation->SuspendLayout();
			//(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numPointRadius))->BeginInit();
			this->SuspendLayout();
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabInterpolation);
			this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl1->Location = System::Drawing::Point(0, 0);
			this->tabControl1->Margin = System::Windows::Forms::Padding(4);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(726, 389);
			this->tabControl1->TabIndex = 0;
			// 
			// tabInterpolation
			// 
			this->tabInterpolation->BackColor = System::Drawing::SystemColors::Control;
			//this->tabInterpolation->Controls->Add(this->numPointRadius);
			this->tabInterpolation->Controls->Add(this->rbBothInterpolations);
			this->tabInterpolation->Controls->Add(this->rbNewtonMethod);
			this->tabInterpolation->Controls->Add(this->rbLagrangeMethod);
			this->tabInterpolation->Controls->Add(this->label3);
			this->tabInterpolation->Controls->Add(this->pnlGraphArea);
			this->tabInterpolation->Controls->Add(this->btnAddPointOrInterpolate);
			this->tabInterpolation->Controls->Add(this->btnDeletePoints);
			this->tabInterpolation->Controls->Add(this->label2);
			this->tabInterpolation->Controls->Add(this->label1);
			this->tabInterpolation->Controls->Add(this->txtNewPointY);
			this->tabInterpolation->Controls->Add(this->txtNewPointX);
			this->tabInterpolation->Controls->Add(this->listPoints);
			this->tabInterpolation->Location = System::Drawing::Point(4, 25);
			this->tabInterpolation->Margin = System::Windows::Forms::Padding(4);
			this->tabInterpolation->Name = L"tabInterpolation";
			this->tabInterpolation->Padding = System::Windows::Forms::Padding(15);
			this->tabInterpolation->Size = System::Drawing::Size(718, 360);
			this->tabInterpolation->TabIndex = 0;
			this->tabInterpolation->Text = L"Interpolacija";
			// 
			// rbBothInterpolations
			// 
			this->rbBothInterpolations->AutoSize = true;
			this->rbBothInterpolations->Location = System::Drawing::Point(501, 13);
			this->rbBothInterpolations->Name = L"rbBothInterpolations";
			this->rbBothInterpolations->Size = System::Drawing::Size(53, 21);
			this->rbBothInterpolations->TabIndex = 11;
			this->rbBothInterpolations->Text = L"Oba";
			this->rbBothInterpolations->UseVisualStyleBackColor = true;
			// 
			// rbNewtonMethod
			// 
			this->rbNewtonMethod->AutoSize = true;
			this->rbNewtonMethod->Checked = true;
			this->rbNewtonMethod->Location = System::Drawing::Point(426, 14);
			this->rbNewtonMethod->Name = L"rbNewtonMethod";
			this->rbNewtonMethod->Size = System::Drawing::Size(59, 21);
			this->rbNewtonMethod->TabIndex = 10;
			this->rbNewtonMethod->TabStop = true;
			this->rbNewtonMethod->Text = L"Njutn";
			this->rbNewtonMethod->UseVisualStyleBackColor = true;
			// 
			// rbLagrangeMethod
			// 
			this->rbLagrangeMethod->AutoSize = true;
			this->rbLagrangeMethod->Location = System::Drawing::Point(332, 14);
			this->rbLagrangeMethod->Name = L"rbLagrangeMethod";
			this->rbLagrangeMethod->Size = System::Drawing::Size(78, 21);
			this->rbLagrangeMethod->TabIndex = 9;
			this->rbLagrangeMethod->Text = L"Lagranž";
			this->rbLagrangeMethod->UseVisualStyleBackColor = true;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(191, 15);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(135, 17);
			this->label3->TabIndex = 8;
			this->label3->Text = L"Metod interpolacije: ";
			// 
			// pnlGraphArea
			// 
			this->pnlGraphArea->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
																																											 | System::Windows::Forms::AnchorStyles::Left)
																																											| System::Windows::Forms::AnchorStyles::Right));
			this->pnlGraphArea->BackColor = System::Drawing::SystemColors::ControlLightLight;
			this->pnlGraphArea->Location = System::Drawing::Point(194, 36);
			this->pnlGraphArea->Name = L"pnlGraphArea";
			this->pnlGraphArea->Size = System::Drawing::Size(506, 306);
			this->pnlGraphArea->TabIndex = 5;
			this->pnlGraphArea->Resize += gcnew System::EventHandler(this, &CalculationForm::pnlGraphArea_Resize);
			// 
			// btnAddPointOrInterpolate
			// 
			this->btnAddPointOrInterpolate->Enabled = false;
			this->btnAddPointOrInterpolate->Location = System::Drawing::Point(22, 66);
			this->btnAddPointOrInterpolate->Name = L"btnAddPointOrInterpolate";
			this->btnAddPointOrInterpolate->Size = System::Drawing::Size(154, 27);
			this->btnAddPointOrInterpolate->TabIndex = 2;
			this->btnAddPointOrInterpolate->Text = L"Dodaj tačku";
			this->btnAddPointOrInterpolate->UseVisualStyleBackColor = true;
			this->btnAddPointOrInterpolate->Click += gcnew System::EventHandler(this, &CalculationForm::btnAddPointOrInterpolate_Click);
			this->btnAddPointOrInterpolate->PreviewKeyDown += gcnew System::Windows::Forms::PreviewKeyDownEventHandler(this, &CalculationForm::txtNewPoint_PreviewKeyDown);
			// 
			// btnDeletePoints
			// 
			this->btnDeletePoints->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->btnDeletePoints->Enabled = false;
			this->btnDeletePoints->Location = System::Drawing::Point(22, 313);
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
			this->txtNewPointY->TextChanged += gcnew System::EventHandler(this, &CalculationForm::txtNewPoint_TextChanged);
			this->txtNewPointY->PreviewKeyDown += gcnew System::Windows::Forms::PreviewKeyDownEventHandler(this, &CalculationForm::txtNewPoint_PreviewKeyDown);
			// 
			// txtNewPointX
			// 
			this->txtNewPointX->Location = System::Drawing::Point(22, 36);
			this->txtNewPointX->Margin = System::Windows::Forms::Padding(4);
			this->txtNewPointX->Name = L"txtNewPointX";
			this->txtNewPointX->Size = System::Drawing::Size(66, 23);
			this->txtNewPointX->TabIndex = 0;
			this->txtNewPointX->TextChanged += gcnew System::EventHandler(this, &CalculationForm::txtNewPoint_TextChanged);
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
			this->listPoints->Size = System::Drawing::Size(154, 196);
			this->listPoints->TabIndex = 3;
			this->listPoints->SelectedIndexChanged += gcnew System::EventHandler(this, &CalculationForm::listPoints_SelectedIndexChanged);
			this->listPoints->PreviewKeyDown += gcnew System::Windows::Forms::PreviewKeyDownEventHandler(this, &CalculationForm::listPoints_PreviewKeyDown);
			// 
			// numPointRadius
			// 
			//this->numPointRadius->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 65536 });
			//this->numPointRadius->Location = System::Drawing::Point(577, 13);
			//this->numPointRadius->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			//this->numPointRadius->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			//this->numPointRadius->Name = L"numPointRadius";
			//this->numPointRadius->Size = System::Drawing::Size(120, 23);
			//this->numPointRadius->TabIndex = 12;
			//this->numPointRadius->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 0 });
			//this->numPointRadius->ValueChanged += gcnew System::EventHandler(this, &CalculationForm::numPointRadius_ValueChanged);
			// 
			// CalculationForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(726, 389);
			this->Controls->Add(this->tabControl1);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
																								static_cast<System::Byte>(254)));
			this->Margin = System::Windows::Forms::Padding(4);
			this->MinimumSize = System::Drawing::Size(590, 345);
			this->Name = L"CalculationForm";
			this->Text = L"Matematička izračunavanja";
			this->Load += gcnew System::EventHandler(this, &CalculationForm::CalculationForm_Load);
			this->tabControl1->ResumeLayout(false);
			this->tabInterpolation->ResumeLayout(false);
			this->tabInterpolation->PerformLayout();
			//(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numPointRadius))->EndInit();
			this->ResumeLayout(false);

		}
		#pragma endregion
	private:
		System::Void CalculationForm_Load(System::Object^  sender, System::EventArgs^  e);
		System::Void txtNewPoint_TextChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void btnAddPointOrInterpolate_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void txtNewPoint_PreviewKeyDown(System::Object^  sender, System::Windows::Forms::PreviewKeyDownEventArgs^  e);
		System::Void listPoints_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void listPoints_PreviewKeyDown(System::Object^  sender, System::Windows::Forms::PreviewKeyDownEventArgs^  e);
		System::Void btnDeletePoints_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void pnlGraphArea_Resize(System::Object^  sender, System::EventArgs^  e);

		bool canAddPointOrInterpolate();
		double Interpolate(InterpolationMethod, array<PointF>^, double newX, Object ^ precalculatedPart);
		double Interpolate(InterpolationMethod, array<PointF>^, double newX);
		property array<PointF>^ InputPoints { array<PointF>^ get(); }
		property InterpolationMethod CurrentInterpolationMethod {InterpolationMethod get();};
		property InterpolationMethod ChosenInterpolationMethod {InterpolationMethod get();};
		bool AddPointToList();
		bool IsNewPointValid(PointF ^ newPoint);

		bool PerformInterpolation();

		void DrawEverything(Control ^ graphArea, array<PointF>^ points, InterpolationMethod method);
		//array<array<PointF>^>^ getNormalizedLines(array<PointF>^ points, Drawing::Size graphAreaSize, InterpolationMethod);
		//PointF approximateLineEnd(PointF currentPoint, PointF^ lowerBound, PointF^ upperBound);
		void DrawLine(Graphics^ g, array<PointF>^ normalizedPoints, InterpolationMethod method);

		//void DrawPoints(Control ^ graphArea, array<PointF>^ points, PointF firstPointOfGraph, InterpolationMethod method);
		void DrawPoints(Control ^ graphArea, array<PointF>^ points, InterpolationMethod method);
		void DrawNormalizedPoints(Control ^ graphArea, array<PointF>^ points, array<int>^ indicesOfSelectedPoints, InterpolationMethod method);

		array<PointF>^ calculatePoints(Drawing::Size panelSize, array<PointF>^ points, InterpolationMethod method);
		array<PointF>^ calculatePoints(Drawing::Size panelSize, array<PointF>^ points, PointF ^ firstPointOfGraph, InterpolationMethod method);
		array<PointF>^ getInterpoints(array<PointF>^ points, InterpolationMethod);
		array<PointF>^ FitInterpoints(array<PointF>^ interpoints, PointF first, PointF last);
		void getMinAndMax(array<PointF>^ points, PointF ^% min, PointF ^% max);
		void getMinAndMax(array<PointF>^ points, Drawing::Size, PointF ^% min, PointF ^% max);
};
}

