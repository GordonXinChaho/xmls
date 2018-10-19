#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

#include "tinyxml2.h"
#include "XMLSerialization.h"

#define RootClassName "SerializableClass"
#define XMLClassEndTag String("</")+RootClassName+">"
#define Empty_String String::null

#define SerializableType 0x9B47
#define MemberType 0x9B48

using namespace std;

namespace xmls
{
#pragma region Mapping

	class FieldMapping
	{
	public:
		String m_sFieldName;
		String *m_sField;
		FieldMapping(String FieldName, String *Field) {
			m_sFieldName = FieldName;
			m_sField = Field;
		}
		String getFieldName() { return m_sFieldName; }
		String *getField() { return m_sField; }
	};

	class ClassMapping
	{
	public:
		String m_sMemberName;
		Serializable *m_cSubclass;
		ClassMapping(String MemberName, Serializable *subclass) {
			m_sMemberName = MemberName;
			m_cSubclass = subclass;
		}
		String getName() { return m_sMemberName; }
		Serializable *getSubclass() { return m_cSubclass; }
	};
#pragma endregion

	typedef vector<FieldMapping*>::iterator FieldMappingIterator;
	typedef vector<ClassMapping*>::iterator ClassMappingIterator;
	typedef vector<CollectionBase*>::iterator CollectionIterator;

#pragma region Member

	MemberBase::MemberBase()
	{
		Register(String::null, this);
	}

	MemberBase::~MemberBase()
	{
	}
	String MemberBase::toString()
	{
		return m_sValue;
	}
	const char * MemberBase::c_str()
	{
		return m_sValue.c_str();
	}
	String * MemberBase::getStringPtr()
	{
		return &m_sValue;
	}
	int MemberBase::type() const
	{
		return MemberType;
	}

	void xString::AssignValue(const String & value)
	{
		m_sValue = value;
	}
	xString::xString()
	{
	}
	xString::xString(String value)
	{
		AssignValue(value);
	}
	String xString::value()
	{
		return m_sValue;
	}
	xString& xString::operator=(const String & value)
	{
		AssignValue(value);
		return *this;
	}
	xString& xString::operator=(const char * value)
	{
		AssignValue(value);
		return *this;
	}

	void xInt::AssignValue(int value)
	{
		String s;

		stringstream ss;
		ss << value;
		m_sValue = ss.str().c_str();
	}

	xInt::xInt()
		:xInt(0)
	{
	}

	xInt::xInt(int value)
	{
		AssignValue(value);
	}

	int xInt::value()
	{
		stringstream ss(m_sValue.c_str());
		int value;
		if ((ss >> value).fail())
			return 0;
		return value;
	}
	xInt& xInt::operator=(xInt& value) {
		AssignValue(value.value());

		return *this;
	}

	xInt & xInt::operator=(const int value)
	{
		AssignValue(value); return *this;
	}

	void xBool::AssignValue(const bool value)
	{
		m_sValue = value ? "TRUE" : "FALSE";
	}

	xBool::xBool()
		:xBool(false)
	{
	}

	xBool::xBool(bool value)
	{
		AssignValue(value);
	}

	bool xBool::value()
	{
		m_sValue.upcase();
		if (m_sValue == "TRUE") return true;
		return false;
	}

	xBool & xBool::operator=(xBool& value) {
		AssignValue(value.value());

		return *this;
	}

	xBool & xBool::operator=(const bool value)
	{
		AssignValue(value); return *this;
	}

	void xTime_t::AssignValue(const time_t value)
	{
		stringstream ss;
		ss << value;
		m_sValue = ss.str().c_str();
	}

	xTime_t::xTime_t()
		:xTime_t(0)
	{
	}

	xTime_t::xTime_t(time_t value)
	{
		AssignValue(value);
	}

	time_t xTime_t::value()
	{
		time_t value;
		stringstream ss(m_sValue.c_str());
		if ((ss >> value).fail())
			return time_t();

		return value;
	}

	xTime_t & xTime_t::operator=(xTime_t& value) {
		AssignValue(value.value());
		return *this;
	}

	xTime_t & xTime_t::operator=(const time_t value)
	{
		AssignValue(value);
		return *this;
	}

	void xFloat::AssignValue(float value)
	{
		stringstream ss;
		ss.precision(7);
		ss << value;
		m_sValue = ss.str().c_str();
	}

	xFloat::xFloat()
		:xFloat(0.0f)
	{
	}

	xFloat::xFloat(float value)
	{
		AssignValue(value);
	}

	float xFloat::value()
	{
		stringstream ss(m_sValue.c_str());
		ss.precision(7);
		float value;
		if ((ss >> value).fail())
			return 0.0f;
		return value;
	}

	xFloat & xFloat::operator=(xFloat& value) {
		AssignValue(value.value());

		return *this;
	}

	xFloat & xFloat::operator=(const float value)
	{
		AssignValue(value); return *this;
	}

	void xDouble::AssignValue(double value)
	{
		stringstream ss;
		ss.precision(16);
		ss << value;
		m_sValue = ss.str().c_str();
	}

	xDouble::xDouble()
		:xDouble(0.0)
	{
	}

	xDouble::xDouble(double value)
	{
		AssignValue(value);
	}

	double xDouble::value()
	{
		//return stod(m_sValue.c_str());
		stringstream ss(m_sValue.c_str());
		ss.precision(16);
		double value;
		if ((ss >> value).fail())
			return 0.0;
		return value;
	}

	xDouble & xDouble::operator=(xDouble& value) {
		AssignValue(value.value());

		return *this;
	}

	xDouble & xDouble::operator=(const double value)
	{
		AssignValue(value); return *this;
	}
#pragma endregion

	Serializable::Serializable() :
		m_sClassName("Serializable"),
		m_sVersion("1.0")
	{
		m_FieldMappings = new vector<FieldMapping*>();
		m_SubclassMappings = new vector<ClassMapping*>();
		m_SubclassCollections = new vector<CollectionBase*>();
	}

	Serializable::~Serializable()
	{
		auto mSubclassCollections = (vector<CollectionBase*>*)m_SubclassCollections;
		auto mSubclassMappings = (vector<ClassMapping*>*)m_SubclassMappings;
		auto mFieldMappings = (vector<FieldMapping*>*)m_FieldMappings;
		if (mSubclassCollections->size() > 0)
		{
			for (CollectionIterator it_collection = mSubclassCollections->begin(); it_collection != mSubclassCollections->end(); ++it_collection)
				(*it_collection)->Clear();
			mSubclassCollections->clear();
		}
		if (mSubclassMappings->size() > 0)
			for (ClassMappingIterator it_subClassMapping = mSubclassMappings->begin(); it_subClassMapping != mSubclassMappings->end(); ++it_subClassMapping)
				delete(*it_subClassMapping);

		if (mFieldMappings->size() > 0)
			for (FieldMappingIterator it_fieldMapping = mFieldMappings->begin(); it_fieldMapping != mFieldMappings->end(); ++it_fieldMapping)
				delete(*it_fieldMapping);

		mSubclassCollections->clear();
		mSubclassMappings->clear();
		mFieldMappings->clear();

		delete mSubclassCollections;
		delete mSubclassMappings;
		delete mFieldMappings;
	}

	void Serializable::Register(String MemberName, MemberBase *Member, String DefaultValue)
	{
		auto mFieldMappings = (vector<FieldMapping*>*)m_FieldMappings;
		FieldMapping *mapping = new FieldMapping(MemberName, Member->getStringPtr());
		if (!DefaultValue.empty())
			*(Member->getStringPtr()) = DefaultValue;
		mFieldMappings->push_back(mapping);
	}

	void Serializable::Register(String MemberName, Serializable *Member)
	{
		auto mSubclassMappings = (vector<ClassMapping*>*)m_SubclassMappings;
		ClassMapping *mapping = new ClassMapping(MemberName, Member);
		mSubclassMappings->push_back(mapping);
	}

	void Serializable::Register(String CollectionName, CollectionBase *SubclassCollection)
	{
		auto mSubclassCollections = (vector<CollectionBase*>*)m_SubclassCollections;
		SubclassCollection->setCollectionName(CollectionName);
		mSubclassCollections->push_back(SubclassCollection);
	}

	void Serializable::Serialize(void *classDoc, void *rootNode)
	{
		auto mSubclassCollections = (vector<CollectionBase*>*)m_SubclassCollections;
		auto mSubclassMappings = (vector<ClassMapping*>*)m_SubclassMappings;
		auto mFieldMappings = (vector<FieldMapping*>*)m_FieldMappings;
		auto xmlDoc = (tinyxml2::XMLDocument*) classDoc;
		auto root = (tinyxml2::XMLElement*)rootNode;
		for (FieldMappingIterator it_member = mFieldMappings->begin(); it_member != mFieldMappings->end(); ++it_member)
		{
			String fieldName = (*it_member)->getFieldName();
			tinyxml2::XMLElement *memberNode = xmlDoc->NewElement("Member");
			if (!fieldName.empty()) {//name is not null
				memberNode->SetAttribute("Name", fieldName.c_str());
			}
			tinyxml2::XMLText* memberValue;
			memberValue = xmlDoc->NewText((*(*it_member)->getField()).c_str());
			memberNode->InsertEndChild(memberValue);
			root->InsertEndChild(memberNode);
		}

		for (ClassMappingIterator it_subclass = mSubclassMappings->begin(); it_subclass != mSubclassMappings->end(); ++it_subclass)
		{
			Serializable *subClass = (*it_subclass)->getSubclass();
			tinyxml2::XMLElement *subClassNode = xmlDoc->NewElement("Class");
			subClassNode->SetAttribute("Name", ((*it_subclass)->getName()).c_str());
			subClassNode->SetAttribute("Type", (*it_subclass)->getSubclass()->getClassName().c_str());
			subClassNode->SetAttribute("Version", (*it_subclass)->getSubclass()->getVersion().c_str());
			root->InsertEndChild(subClassNode);
			subClass->Serialize(xmlDoc, subClassNode);
		}

		for (CollectionIterator it_collection = mSubclassCollections->begin(); it_collection != mSubclassCollections->end(); ++it_collection)
		{
			tinyxml2::XMLElement *listNode = xmlDoc->NewElement("Collection");
			listNode->SetAttribute("Name", (*it_collection)->getCollectionName().c_str());

			for (size_t c = 0; c < (*it_collection)->size(); c++)
			{
				Serializable *item = (*it_collection)->getItem(c);
				if (item->type() == MemberType) {
					auto m = (MemberBase*)item;
					tinyxml2::XMLElement *elementNode = xmlDoc->NewElement("Member");
					tinyxml2::XMLText* memberValue = xmlDoc->NewText(m->getStringPtr()->c_str());
					elementNode->InsertEndChild(memberValue);
					listNode->InsertEndChild(elementNode);
				}
				else {
					tinyxml2::XMLElement *elementNode = xmlDoc->NewElement("Class");
					elementNode->SetAttribute("Type", item->getClassName().c_str());
					elementNode->SetAttribute("Version", item->getVersion().c_str());
					item->Serialize(xmlDoc, elementNode);
					listNode->InsertEndChild(elementNode);
				}
			}
			root->InsertEndChild(listNode);
		}
	}

	void Serializable::Deserialize(Serializable *classItem, void *classDoc, void* rootNode)
	{
		auto xmlDoc = (tinyxml2::XMLDocument*)classDoc;
		auto root = (tinyxml2::XMLElement *)rootNode;
		classItem->setClassName(root->Attribute("Type"));
		classItem->setVersion(root->Attribute("Version"));
		auto mSubclassCollections = (vector<CollectionBase*>*)classItem->m_SubclassCollections;
		auto mSubclassMappings = (vector<ClassMapping*>*)classItem->m_SubclassMappings;
		auto mFieldMappings = (vector<FieldMapping*>*)classItem->m_FieldMappings;
		tinyxml2::XMLElement *memberNode = root->FirstChildElement("Member");

		while (memberNode && memberNode->GetText())
		{
			String fieldName = memberNode->Attribute("Name");
			for (FieldMappingIterator it = mFieldMappings->begin(); it != mFieldMappings->end(); ++it)
				if (fieldName.empty() || (*it)->getFieldName() == fieldName)
					*(*it)->getField() = memberNode->GetText();
			memberNode = memberNode->NextSiblingElement("Member");
		}

		tinyxml2::XMLElement *classNode = root->FirstChildElement("Class");
		while (classNode != NULL)
		{
			String className = classNode->Attribute("Name");

			for (ClassMappingIterator it_subclass = mSubclassMappings->begin(); it_subclass != mSubclassMappings->end(); ++it_subclass)
				if ((*it_subclass)->getName() == className)
					(*it_subclass)->getSubclass()->Deserialize((*it_subclass)->getSubclass(), xmlDoc, classNode);
			classNode = classNode->NextSiblingElement("Class");
		}

		tinyxml2::XMLElement *collectionNode = root->FirstChildElement("Collection");
		while (collectionNode != NULL)
		{
			String collectionName = collectionNode->Attribute("Name");
			for (CollectionIterator it_collection = mSubclassCollections->begin(); it_collection != mSubclassCollections->end(); ++it_collection)
			{
				if ((*it_collection)->getCollectionName() == collectionName)
				{
					tinyxml2::XMLElement *classNode = collectionNode->FirstChildElement("Class");
					while (classNode != NULL)
					{
						Serializable *newItem = (*it_collection)->newElement();
						Serializable::Deserialize(newItem, xmlDoc, classNode);
						classNode = classNode->NextSiblingElement("Class");
					}
					if (classNode == NULL) {
						auto m = collectionNode->FirstChildElement("Member");
						while (m != NULL)
						{
							MemberBase *newItem = (MemberBase*)((*it_collection)->newElement());
							*(newItem->getStringPtr()) = m->GetText();
							m = m->NextSiblingElement("Member");
						}
					}
					break;
				}
			}
			collectionNode = collectionNode->NextSiblingElement("Collection");
		}
	}

	String Serializable::toXML()
	{
		tinyxml2::XMLDocument *classDoc = new tinyxml2::XMLDocument();
		tinyxml2::XMLElement *rootNode;
		rootNode = classDoc->NewElement(RootClassName);
		rootNode->SetAttribute("Type", m_sClassName.c_str());
		rootNode->SetAttribute("Version", m_sVersion.c_str());
		classDoc->InsertFirstChild(rootNode);
		Serialize(classDoc, rootNode);
		tinyxml2::XMLPrinter printer;
		classDoc->Accept(&printer);
		String xmlResult = printer.CStr();
		classDoc->Clear();
		delete(classDoc);
		return xmlResult;
	}

	void Serializable::toXMLFile(const String & file) {
		std::ofstream fw(file.c_str());
		std::string str(toXML().c_str());
		fw << str;
		fw.close();
	}

	bool Serializable::fromXML(const String & XMLSource, Serializable *classObject)
	{
		bool result = false;
		tinyxml2::XMLDocument *classDoc = new tinyxml2::XMLDocument();
		tinyxml2::XMLElement *rootNode;
		if (tinyxml2::XML_SUCCESS == classDoc->Parse(XMLSource.c_str(), XMLSource.len()))
		{
			rootNode = classDoc->FirstChildElement(RootClassName);
			if (rootNode)
			{
				Serializable::Deserialize(classObject, classDoc, rootNode);
				result = true;
			}
		}
		classDoc->Clear();
		delete(classDoc);
		return result;
	}

	bool Serializable::fromXMLFile(const String & file, Serializable *classObject) {
		std::ifstream fr(file.c_str(), std::ios::in);
		std::istreambuf_iterator<char> beg(fr), end;
		std::string str(beg, end);
		fr.close();
		String s(str.c_str());
		return fromXML(s, classObject);
	}

	bool Serializable::fromXML(const String & xml)
	{
		return Serializable::fromXML(xml, this);
	}

	bool Serializable::fromXMLFile(const String & file)
	{
		return Serializable::fromXMLFile(file, this);
	}

	String Serializable::IdentifyClass(const String& XMLSource)
	{
		tinyxml2::XMLDocument doc;
		doc.Parse(XMLSource.c_str(), XMLSource.len());
		tinyxml2::XMLElement* rootNode;
		rootNode = doc.FirstChildElement(RootClassName);
		if (rootNode)
		{
			const char* value = rootNode->Attribute("Type");
			if (value) return String(value);
		}
		return Empty_String;
	}

	String Serializable::IdentifyClassVersion(const String& XMLSource)
	{
		tinyxml2::XMLDocument doc;
		doc.Parse(XMLSource.c_str(), XMLSource.len());
		tinyxml2::XMLElement* rootNode;
		rootNode = doc.FirstChildElement(RootClassName);
		if (rootNode)
		{
			const char* value = rootNode->Attribute("Version");
			if (value) return String(value);
		}
		return Empty_String;
	}

	bool Serializable::Compare(Serializable *msg)
	{
		bool identical = true;
		if (getClassName() != msg->getClassName())
			identical = false;
		auto mSubclassCollections = (vector<CollectionBase*>*)m_SubclassCollections;
		auto mSubclassMappings = (vector<ClassMapping*>*)m_SubclassMappings;
		auto mFieldMappings = (vector<FieldMapping*>*)m_FieldMappings;

		auto mSubclassCollections2 = (vector<CollectionBase*>*)msg->m_SubclassCollections;
		auto mSubclassMappings2 = (vector<ClassMapping*>*)msg->m_SubclassMappings;
		auto mFieldMappings2 = (vector<FieldMapping*>*)msg->m_FieldMappings;
		if (identical)
			for (FieldMappingIterator it = mFieldMappings->begin(); it != mFieldMappings->end(); ++it)
				for (FieldMappingIterator it_c = mFieldMappings2->begin(); it_c != mFieldMappings2->end(); ++it_c)
					if (((*it)->getFieldName()) == ((*it_c)->getFieldName()))
						if (*(*it)->getField() != *(*it_c)->getField())
							identical = false;

		if (identical)
			for (ClassMappingIterator it = mSubclassMappings->begin(); it != mSubclassMappings->end(); ++it)
				for (ClassMappingIterator it_c = mSubclassMappings2->begin(); it_c != mSubclassMappings2->end(); ++it_c)
					if ((*it)->getName() == (*it_c)->getName())
						if (!(*it)->getSubclass()->Compare((*it_c)->getSubclass()))
							identical = false;

		if (identical)
		{
			if (mSubclassCollections->size() != mSubclassCollections2->size())
			{
				identical = false;
			}
			else
				for (CollectionIterator it = mSubclassCollections->begin(); it != mSubclassCollections->end(); ++it)
					for (CollectionIterator it_c = mSubclassCollections2->begin(); it_c != mSubclassCollections2->end(); ++it_c)
						if ((*it)->getCollectionName() == (*it_c)->getCollectionName())
							if ((*it)->size() != (*it_c)->size())
								identical = false; else
								for (size_t c = 0; c < (*it)->size(); c++)
									if (!(*it)->getItem(c)->Compare((*it_c)->getItem(c)))
									{
										identical = false; break;
									}
		}
		return identical;
	}

	bool Serializable::Clone(Serializable *source, Serializable *destination)
	{
		bool result = true;
		if (source->getClassName() != destination->getClassName())
			result = false;

		auto mSubclassCollections = (vector<CollectionBase*>*)source->m_SubclassCollections;
		auto mSubclassMappings = (vector<ClassMapping*>*)source->m_SubclassMappings;
		auto mFieldMappings = (vector<FieldMapping*>*)source->m_FieldMappings;

		auto mSubclassCollections2 = (vector<CollectionBase*>*)destination->m_SubclassCollections;
		auto mSubclassMappings2 = (vector<ClassMapping*>*)destination->m_SubclassMappings;
		auto mFieldMappings2 = (vector<FieldMapping*>*)destination->m_FieldMappings;

		if (result)
			for (FieldMappingIterator it_source = mFieldMappings->begin(); it_source != mFieldMappings->end(); ++it_source)
				for (FieldMappingIterator it_dest = mFieldMappings2->begin(); it_dest != mFieldMappings2->end(); ++it_dest)
					if ((*it_dest)->getFieldName() == (*it_source)->getFieldName())
						*(*it_dest)->getField() = *(*it_source)->getField();

		if (result)
			for (ClassMappingIterator it_source = mSubclassMappings->begin(); it_source != mSubclassMappings->end(); ++it_source)
				for (ClassMappingIterator it_dest = mSubclassMappings2->begin(); it_dest != mSubclassMappings2->end(); ++it_dest)
					if ((*it_dest)->getName() == (*it_source)->getName())
						if (!(*it_dest)->getSubclass()->Copy((*it_source)->getSubclass()))
							result = false;

		for (CollectionIterator it_source = mSubclassCollections->begin(); it_source != mSubclassCollections->end(); ++it_source)
			for (CollectionIterator it_dest = mSubclassCollections2->begin(); it_dest != mSubclassCollections2->end(); ++it_dest)
				if ((*it_source)->getCollectionName() == (*it_dest)->getCollectionName())
					for (size_t c = 0; c < (*it_source)->size(); c++)
					{
						Serializable *newItem = (*it_dest)->newElement();
						if (!newItem->Copy((*it_source)->getItem(c)))
						{
							result = false;
						}
					}
		return result;
	}

	bool Serializable::Copy(Serializable *source)
	{
		return Serializable::Clone(source, this);
	}

	String Serializable::strReplaceAll(String source, const String& searchFor, const String& replaceWith)
	{
		if (searchFor.empty())
			return source;
		size_t start_pos = 0;
		while ((start_pos = source.find(searchFor, start_pos)) != String::npos) {
			source.replace(start_pos, searchFor.len(), replaceWith);
			start_pos += replaceWith.len();
		}
		return source;
	}

	void Serializable::Replace(String searchFor, String replaceWith, bool recursive)
	{
		auto mSubclassCollections = (vector<CollectionBase*>*)m_SubclassCollections;
		auto mSubclassMappings = (vector<ClassMapping*>*)m_SubclassMappings;
		auto mFieldMappings = (vector<FieldMapping*>*)m_FieldMappings;

		for (FieldMappingIterator it = mFieldMappings->begin(); it != mFieldMappings->end(); ++it)
			*((*it)->getField()) = strReplaceAll((*(*it)->getField()), searchFor, replaceWith);
		if (recursive)
		{
			for (ClassMappingIterator it = mSubclassMappings->begin(); it != mSubclassMappings->end(); ++it)
				(*it)->getSubclass()->Replace(searchFor, replaceWith);
			for (CollectionIterator it = mSubclassCollections->begin(); it != mSubclassCollections->end(); ++it)
				for (size_t c = 0; c < (*it)->size(); c++) { (*it)->getItem(c)->Replace(searchFor, replaceWith); }
		}
	}
	void Serializable::Clear() {
	}

	int Serializable::type() const
	{
		return SerializableType;
	}
}