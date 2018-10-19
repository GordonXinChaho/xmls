#pragma once

#include "../../xmls/XMLSerialization.h"

using namespace xmls;
using namespace std;

class BoltCharacterConfig :public Serializable
{
public:
	BoltCharacterConfig()
	{
		BoltNum = 1;
		BoltLevel = 0;
		Toplimit = 5;
		BottomLimit = 0.1;
		InitHeight = 5;
		Lock_feed_Height = 0.06;
		R1_Torque_Start = -0.21;
		R1_Torque_Limit = 10;
		Mans_D_Bias = 2;
		Euclidean_D_Accuracy = 2;
		Num_Zero_Adjust = 8;

		Lock_Kp = 0.25;
		Sensitive_Rate = 0;

		Register("BoltNum", &BoltNum);
		Register("BoltLevel", &BoltLevel);
		Register("Toplimit", &Toplimit);
		Register("BottomLimit", &BottomLimit);
		Register("InitHeight", &InitHeight);
		Register("Lock_feed_Height", &Lock_feed_Height);
		Register("R1_Torque_Start", &R1_Torque_Start);
		Register("R1_Torque_Limit", &R1_Torque_Limit);
		Register("Scan_Speed", &Scan_Speed);
		Register("Scan_Tendency_Count", &Scan_Tendency_Count);
		Register("Scan_Tendency_Count_Back", &Scan_Tendency_Count_Back);
		Register("Kp", &Kp);
		Register("Ki", &Ki);
		Register("Mans_D_Bias", &Mans_D_Bias);
		Register("Euclidean_D_Accuracy", &Euclidean_D_Accuracy);
		Register("Num_Zero_Adjust", &Num_Zero_Adjust);
		Register("Zero_Adjust", &Zero_Adjust);
		Register("Lock_Kp", &Lock_Kp);
		Register("Sensitive_Rate", &Sensitive_Rate);
	}

	//Machine Structure
	xInt BoltNum = 1;
	xInt BoltLevel = 0;
	xDouble Toplimit = 5;
	xDouble BottomLimit = 0.1;
	xDouble InitHeight = 5;
	xDouble Lock_feed_Height = 0.06;
	xDouble R1_Torque_Start = -0.21;
	xDouble R1_Torque_Limit = 10;
	//Controller
	Collection<xDouble> Scan_Speed;

	Collection<xInt> Scan_Tendency_Count;
	Collection<xInt> Scan_Tendency_Count_Back;
	Collection<xDouble> Kp;
	Collection<xDouble> Ki;
	//Decision Function
	xDouble Mans_D_Bias = 2;
	xDouble Euclidean_D_Accuracy = 2;
	xInt Num_Zero_Adjust = 8;
	Collection<xInt> Zero_Adjust;
	xDouble Lock_Kp = 0.25;
	xDouble Sensitive_Rate = 0;
};

class CavityChannelConfig :public Serializable
{
public:
	CavityChannelConfig()
	{
		Register("Channel_ID", &Channel_ID);
		Register("StartMHZ", &StartMHZ);
		Register("EndMHZ", &EndMHZ);
		Register("StepMHZ", &StepMHZ);
		Register("NumofPoint", &NumofPoint);
		Register("ReturnLossStartMHZ", &ReturnLossStartMHZ);
		Register("ReturnLossEndMHZ", &ReturnLossEndMHZ);
		Register("ReturnLossTarget", &ReturnLossTarget);
		Register("InsertLossStartMHZ", &InsertLossStartMHZ);
		Register("InsertLossEndMHZ", &InsertLossEndMHZ);
		Register("InsertLoss_Target", &InsertLoss_Target);
		Register("NumofRejection_LowF", &NumofRejection_LowF);
		Register("RejectionPoint_LowF", &RejectionPoint_LowF);
		Register("Rejection_LowF_Target", &Rejection_LowF_Target);
		Register("NumofRejection_HighF", &NumofRejection_HighF);
		Register("RejectionPoint_HighF", &RejectionPoint_HighF);
		Register("Rejection_HighF_Target", &Rejection_HighF_Target);
		Register("CrossPoint", &CrossPoint);
		Register("Leis_Aim_Buff", &Leis_Aim_Buff);
		Register("Leis_Aim_Size", &Leis_Aim_Size);
		Register("Leis_Sum", &Leis_Sum);
		Register("Lei_StepID", &Lei_StepID);
		Register("NumofBolt", &NumofBolt);
		Register("NumofZeroPointBolt", &NumofZeroPointBolt);
		Register("Bolt", &Bolt);
		Register("R1_Torque", &R1_Torque);
		Register("R1_MaxSpeed", &R1_MaxSpeed);
		Register("R2_Torque", &R2_Torque);
		Register("ErrorRange", &ErrorRange);
		Register("TopLimit", &TopLimit);
		Register("Horizon", &Horizon);
		Register("Width", &Width);
		Register("LeftPeakout", &LeftPeakout);
		Register("Boltbuff", &Boltbuff);
		Register("MoveDegree", &MoveDegree);
		Register("S21WindowStartp", &S21WindowStartp);
		Register("S21WindowEndp", &S21WindowEndp);
		Register("MoveRate", &MoveRate);
		Register("Maxdegree", &Maxdegree);
		Register("Lock_feed_Height", &Lock_feed_Height);
		Register("Start_Torque", &Start_Torque);
		Register("R1_Final_Lock", &R1_Final_Lock);
		Register("R1_Final_Lock_H", &R1_Final_Lock_H);

		Channel_ID = 1;
		StartMHZ = 820;//(MHZ)
		EndMHZ = 915; //(MHZ)
		StepMHZ = 0.11875;//(MHZ)
		NumofPoint = 801;
		ReturnLossStartMHZ = 868.5;
		ReturnLossEndMHZ = 881.5;
		ReturnLossTarget = -21;

		InsertLossStartMHZ = 869;
		InsertLossEndMHZ = 880;
		InsertLoss_Target = -1.1;

		NumofRejection_LowF = 2;

		NumofRejection_HighF = 2;

		Leis_Aim_Size = 5;
		Leis_Sum = 10;//切换目标时的裕量;
		Lei_StepID = 0;
		//--------------------------------------------//
		NumofBolt = 25;
		NumofZeroPointBolt = 3;

		//------------------------------------------------------------//
		R1_Torque = -0.95; //夹头 转螺母
								  //xDouble R1_Torque = -9; //夹头 转螺母
		R1_MaxSpeed = 0.2; //夹头 转螺母
		R2_Torque = 6; //批头 转螺钉

		//-----------------------------------------//
		TopLimit = 1.8;
		Horizon = 0;
		Width = 0;
		LeftPeakout = 0;

		MoveDegree = 0;
		S21WindowStartp = 350;
		S21WindowEndp = 550;
		MoveRate = 6;
		Maxdegree = 360;

		Lock_feed_Height = 0.1;
		Start_Torque = -0.5;
		R1_Final_Lock = 6;//单位kg.cm
		R1_Final_Lock_H = 0.005;
		R1_Final_Speed = 0.4;
	}

	xInt Channel_ID = 1;
	xDouble StartMHZ = 820;//(MHZ)
	xDouble EndMHZ = 915; //(MHZ)
	xDouble StepMHZ = 0.11875;//(MHZ)
	xInt NumofPoint = 801;
	xDouble ReturnLossStartMHZ = 868.5;
	xDouble ReturnLossEndMHZ = 881.5;
	xDouble ReturnLossTarget = -21;

	xDouble InsertLossStartMHZ = 869;
	xDouble InsertLossEndMHZ = 880;
	xDouble InsertLoss_Target = -1.1;

	xInt NumofRejection_LowF = 2;
	Collection<xDouble> RejectionPoint_LowF;
	Collection<xDouble>  Rejection_LowF_Target;
	xInt NumofRejection_HighF = 2;
	Collection<xDouble> RejectionPoint_HighF;
	Collection<xDouble>  Rejection_HighF_Target;
	Collection<xInt> CrossPoint;

	Collection<xDouble> Leis_Aim_Buff;
	xInt Leis_Aim_Size = 5;
	xDouble Leis_Sum = 10;//切换目标时的裕量;
	xInt Lei_StepID = 0;
	//--------------------------------------------//
	xInt NumofBolt = 25;
	xInt NumofZeroPointBolt = 3;
	Collection<BoltCharacterConfig> Bolt;
	//------------------------------------------------------------//
	xDouble R1_Torque = -0.95; //夹头 转螺母
							  //xDouble R1_Torque = -9; //夹头 转螺母
	xDouble R1_MaxSpeed = 0.2; //夹头 转螺母
	xDouble R2_Torque = 6; //批头 转螺钉
						  //--------------------------------------------------------//
	Collection<xDouble> ErrorRange;
	//-----------------------------------------//
	xDouble TopLimit = 1.8;
	xInt Horizon;
	xInt Width;
	xInt LeftPeakout;
	Collection<xInt> Boltbuff;
	xInt MoveDegree;
	xInt S21WindowStartp = 350;
	xInt S21WindowEndp = 550;
	xInt MoveRate = 6;
	xInt Maxdegree = 360;

	xDouble Lock_feed_Height = 0.1;
	xDouble Start_Torque = -0.5;
	xDouble R1_Final_Lock = 6;//单位kg.cm
	xDouble R1_Final_Lock_H = 0.005;
	xDouble R1_Final_Speed = 0.4;
};

void ReadAirXml() {
	CavityChannelConfig config;

	if (config.fromXMLFile("config.xml")) {
		cout << config.Bolt.size() << endl;
	}
	else {
		cout << "读取config.xml 失败";
	}
}

void AirInitXml() {
	CavityChannelConfig config;

	for (size_t i = 0; i < 25; i++)
	{
		auto bc = config.Bolt.newElement();

		xDouble ds[10] = { 8,2,2,8,4,5,0,0,0,0 };
		xInt Scan_Tendency_Count[10] = { 7,8,9,8,8,10,0,0,0,0 };
		xInt Scan_Tendency_Count_Back[10] = { 7,8,7,7,10,8,0,0,0,0 };
		xDouble Kp[10] = { 30,2,4,50,60,25,0,0,0,0 };
		xDouble Ki[10] = { };
		xInt Zero_Adjust[8] = {};

		bc->Scan_Speed.reset(ds, 10);
		bc->Scan_Tendency_Count.reset(Scan_Tendency_Count, 10);
		bc->Scan_Tendency_Count_Back.reset(Scan_Tendency_Count_Back, 10);
		bc->Kp.reset(Kp, 10);
		bc->Ki.reset(Ki, 10);
		bc->Zero_Adjust.reset(Zero_Adjust, 8);
	}

	xDouble RejectionPoint_LowF[20] = {};
	xDouble  Rejection_LowF_Target[20] = {};
	xDouble RejectionPoint_HighF[20] = {};
	xDouble  Rejection_HighF_Target[20] = {};
	xInt CrossPoint[10] = {};

	xDouble Leis_Aim_Buff[5] = { -12. - 15,-18,-21,-25 };

	xDouble ErrorRange[10] = { 30,60,0,0,0,0,0,0,0,0 };
	xInt Boltbuff[50] = {};

	config.RejectionPoint_LowF.reset(RejectionPoint_LowF, 20);
	config.Rejection_LowF_Target.reset(Rejection_LowF_Target, 20);
	config.RejectionPoint_HighF.reset(RejectionPoint_HighF, 20);
	config.Rejection_HighF_Target.reset(Rejection_HighF_Target, 20);
	config.CrossPoint.reset(CrossPoint, 10);

	config.Leis_Aim_Buff.reset(Leis_Aim_Buff, 5);

	config.ErrorRange.reset(ErrorRange, 10);
	config.Boltbuff.reset(Boltbuff, 50);

	config.toXMLFile("config.xml");
}