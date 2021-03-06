// xmlsdemo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <Windows.h>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include "../../xmls/XMLSerialization.h"
#include "airx25.h"
using namespace xmls;

class LastUsedDocument : public Serializable
{
public:
	LastUsedDocument();
	xString Name;
	xString Path;
	xInt Size;
};

class DatabaseLogin : public Serializable
{
public:
	DatabaseLogin();
	xString HostName;
	xInt Port;
	xString User;
	xString Password;
};

class ApplicationSettings : public Serializable
{
public:
	ApplicationSettings();
	xString Setting1;
	xString Setting2;
	xString Setting3;
	xInt Setting4;
	xInt Setting5;
	xBool Setting6;
	xBool Setting7;
	xFloat Setting8;
	DatabaseLogin Login;
	Collection<LastUsedDocument> LastUsedDocuments;
	Collection<xInt> IntArray;
	Collection<xDouble> DoubleArray;
};

LastUsedDocument::LastUsedDocument()
{
	setClassName("LastUsedDocument");
	Register("Name", &Name);
	Register("Path", &Path);
	Register("Size", &Size);
};

DatabaseLogin::DatabaseLogin()
{
	setClassName("DatabaseLogin");
	Register("HostName", &HostName);
	Register("Port", &Port);
	Register("User", &User);
	Register("Password", &Password);
};

ApplicationSettings::ApplicationSettings()
{
	setClassName("ApplicationSettings");
	Register("Setting1", &Setting1);
	Register("Setting2", &Setting2);
	Register("Setting3", &Setting3);
	Register("Setting4", &Setting4);
	Register("Setting5", &Setting5);
	Register("Setting6", &Setting6);
	Register("Setting7", &Setting7);
	Register("Setting8", &Setting8);
	Register("Login", &Login);
	Register("LastUsedDocuments", &LastUsedDocuments);
	Register("IntArray", &IntArray);
	Register("DoubleArray", &DoubleArray);
	setVersion("2.1");
};
using namespace std;

void ReadXml() {
	auto s2 = new ApplicationSettings();

	if (s2->fromXMLFile("settings.xml")) {
		for (size_t i = 0; i < s2->IntArray.size(); i++) {
			auto in = s2->IntArray.getItem(i);
			cout << "S2 Int " << i << ": " << in->value() << endl;
		}

		for (size_t i = 0; i < s2->DoubleArray.size(); i++) {
			auto in = s2->DoubleArray[i];
			cout << "S2 Int " << i << ": " << in->value() << endl;
		}
	}
	else {
		cout << "读取失败" << endl;
	}
	delete s2;
}

int main()
{
	ReadAirXml();
	AirInitXml();

	xDouble d = 3.141592632;
	auto tt = d.toXML();
	cout << tt.c_str() << endl;
	auto d2 = new xDouble();
	if (Serializable::fromXML(tt, d2)) {
		auto vv = d2->value();
		printf("%.12f\n", vv);
		cout << vv << endl;
	}

	ReadXml();

	std::cout << "Hello World!\n";
	String str("123");
	cout << str.c_str() << endl;

	cout << "Creating object..." << endl;
	ApplicationSettings *settings = new ApplicationSettings;
	settings->Setting1 = "Settings string 1";
	settings->Setting2 = "Settings string 2";
	settings->Setting3 = "Settings string 3";
	settings->Setting4 = 1234;
	settings->Setting5 = 5678;
	settings->Setting6 = false;
	settings->Setting7 = true;
	settings->Setting8 = 15.2545123f;
	settings->Login.HostName = "my.db.Settings.server.local";
	settings->Login.Port = 2000;
	settings->Login.User = "john.smith";
	settings->Login.Password = "JohnSmithPassword";

	for (int docNum = 1; docNum <= 10; docNum++)
	{
		LastUsedDocument *doc = settings->LastUsedDocuments.newElement();
		std::stringstream docName;
		docName << "Document #" << docNum;
		doc->Name = docName.str().c_str();
		doc->Path = "{FILEPATH}"; // Set Placeholder for search/replace
		doc->setVersion("1.1");

		auto i = settings->IntArray.newElement();
		*i = docNum + 20;

		auto d = settings->DoubleArray.newElement();
		*d = 30.0 + docNum + docNum * 0.1;
	}

	cout << "OK" << endl;

	// Serialize the Settings object
	cout << "Serializing object... " << endl;
	String xmlData = settings->toXML();
	cout << "OK" << endl << endl;
	cout << "Result:" << endl;
	cout << xmlData.c_str() << endl << endl;

	cout << "Login, URL:" << endl;
	cout << "Hostname: " << settings->Login.HostName.value().c_str();
	cout << ":" << settings->Login.Port.toString().c_str() << endl << endl;
	cout << "Show all collection items" << endl;
	for (size_t i = 0; i < settings->LastUsedDocuments.size(); i++)
	{
		LastUsedDocument* doc = settings->LastUsedDocuments.getItem(i);
		cout << "Item " << i << ": " << doc->Name.value().c_str() << endl;
	}

	for (size_t i = 0; i < settings->IntArray.size(); i++) {
		auto in = settings->IntArray.getItem(i);
		cout << "Int " << i << ": " << in->value() << endl;
	}

	cout << endl;

	cout << "Deserialization:" << endl;
	cout << "Class version: " << Serializable::IdentifyClassVersion(xmlData).c_str() << endl;
	cout << "Performing deserialization..." << endl;

	// Deserialize the XML text
	ApplicationSettings* dser_Settings = new ApplicationSettings;
	if (Serializable::fromXML(xmlData, dser_Settings))
	{
		cout << "OK" << endl << endl;

		// compare both objects
		cout << "Compareing objects: ";
		if (dser_Settings->Compare(settings))
			cout << "equal" << endl << endl; else
			cout << "net equal" << endl << endl;

		// now set value
		cout << "Set new value for field >password<..." << endl;
		dser_Settings->Login.Password = "newPassword";
		cout << "OK" << endl << endl;

		cout << "compare objects again: ";
		if (dser_Settings->Compare(settings))
			cout << "equal" << endl << endl; else
			cout << "net equal" << endl << endl;

		cout << "search and replace placeholders: ";
		dser_Settings->Replace("{FILEPATH}", "c:\\temp\\");
		cout << "OK" << endl << endl;

		//output xml-data
		cout << "Serialize and output xml data: " << endl;
		cout << dser_Settings->toXML().c_str() << endl << endl;

		cout << "Clone object:" << endl;
		ApplicationSettings *clone1(new ApplicationSettings);
		Serializable::Clone(dser_Settings, clone1);
		cout << "Serialize and output clone: " << endl;
		cout << clone1->toXML().c_str() << endl << endl;
		delete (clone1);
	}

	settings->toXMLFile("settings.xml");

	auto s2 = new ApplicationSettings();

	if (Serializable::fromXMLFile("settings.xml", s2)) {
		cout << (s2->Setting7.value() == settings->Setting7.value()) << endl;

		for (size_t i = 0; i < s2->IntArray.size(); i++) {
			auto in = s2->IntArray.getItem(i);
			cout << "S2 Int " << i << ": " << in->value() << endl;
		}

		for (size_t i = 0; i < s2->DoubleArray.size(); i++) {
			auto in = s2->DoubleArray[i];
			cout << "S2 Int " << i << ": " << in->value() << endl;
		}
	}
	else {
		cout << "读取失败" << endl;
	}
	delete s2;

	delete(settings);
	delete(dser_Settings);

	getchar();
	return 0;
}