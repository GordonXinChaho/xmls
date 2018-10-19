#pragma once

#include <vector>
#include <map>

#include "String.h"

namespace xmls
{
	class MemberBase;
	class CollectionBase;

	class XMLS_DLL_API Serializable
	{
		friend class CollectionBase;
	private:
		Serializable(Serializable const &s) { }
		Serializable operator=(Serializable const &s) { return *this; };
		static String strReplaceAll(String source, const String&from, const String& to);
	protected:
		String m_sXML;
		String m_sClassName;
		String m_sVersion;
		void* m_FieldMappings;
		void* m_SubclassMappings;
		void* m_SubclassCollections;
		Serializable();
		virtual ~Serializable();
		void setClassName(const String& ClassName) { m_sClassName = ClassName; };
		void Register(String MemberName, MemberBase *Member, String DefaultValue = String::null);
		void Register(String MemberName, Serializable *Member);
		void Register(String CollectionName, CollectionBase *SubclassCollection);
	private:
		void Serialize(void *classDoc, void *rootNode);
		static void Deserialize(Serializable *classItem, void *classDoc, void *rootNode);
	public:
		static bool fromXML(const String & XMLSource, Serializable *classObject);
		static bool fromXMLFile(const String &file, Serializable *classObject);
		bool fromXML(const String& xml);
		bool fromXMLFile(const String &file);
		static String IdentifyClass(const String& XMLSource);
		static String IdentifyClassVersion(const String& XMLSource);
		String getClassName() { return m_sClassName; };
		void setVersion(String value) { m_sVersion = value; };
		String getVersion() { return m_sVersion; };
		String toXML();
		void toXMLFile(const String & file);
		bool Compare(Serializable *messageToCompare);
		static bool Clone(Serializable *source, Serializable *destination);
		bool Copy(Serializable *source);
		void Replace(String searchFor, String replaceWith, bool recursive = true);
		void Clear();
		virtual int type() const;
	};

	class XMLS_DLL_API MemberBase :public Serializable
	{
	protected:
		String m_sValue;
	public:
		MemberBase();
		virtual ~MemberBase();
		String toString();
		const char* c_str();
		String *getStringPtr();
		int type() const override;
	};

	class XMLS_DLL_API xString : public MemberBase
	{
	private:
		void AssignValue(const String& value);
	public:
		xString();
		xString(String value);
		String value();
		xString& operator=(const String& value);
		xString& operator=(const char* value);
	};

	class XMLS_DLL_API xInt : public MemberBase
	{
	private:
		void AssignValue(const int value);
	public:
		xInt();
		xInt(int value);
		int value();
		xInt& operator=(xInt& value);
		xInt& operator=(const int value);
	};

	class XMLS_DLL_API xBool : public MemberBase
	{
	private:
		void AssignValue(const bool value);
	public:
		xBool();
		xBool(bool value);
		bool value();
		xBool& operator=(xBool& value);
		xBool& operator=(const bool value);
	};

	class XMLS_DLL_API xTime_t : public MemberBase
	{
	private:
		void AssignValue(const time_t value);
	public:
		xTime_t();
		xTime_t(time_t value);
		time_t value();
		xTime_t& operator=(xTime_t& value);
		xTime_t& operator=(const time_t value);
	};

	class XMLS_DLL_API xFloat :public MemberBase {
	private:
		void AssignValue(const float value);
	public:
		xFloat();
		xFloat(float value);
		float value();
		xFloat& operator=(xFloat& value);
		xFloat& operator=(const float value);
	};

	class XMLS_DLL_API xDouble :public MemberBase {
	private:
		void AssignValue(const double value);
	public:
		xDouble();
		xDouble(double value);
		double value();
		xDouble& operator=(xDouble& value);
		xDouble& operator=(const double value);
	};

	class CollectionBase
	{
		friend class Serializable;
	private:
		String m_sCollectionName;
		String m_sCollectionClassType;
	protected:
		CollectionBase() {};
		std::vector<Serializable*> m_vCollection;
		std::map<Serializable*, bool> m_mOwner;
		void setCollectionName(String value) { m_sCollectionName = value; };
		void setCollectionClassType(String value) { m_sCollectionClassType = value; };
		String getCollectionName() { return m_sCollectionName; };
		virtual Serializable *newElement() = 0;
	public:
		virtual ~CollectionBase() {};
		size_t size() { return m_vCollection.size(); };
		virtual Serializable *getItem(int itemID) { return m_vCollection.at(itemID); };
		void Clear() {
			if (m_vCollection.size() > 0)
			{
				for (std::vector<Serializable*>::iterator it = m_vCollection.begin(); it != m_vCollection.end(); ++it)
					if (m_mOwner.find(*it)->second)
						delete *it;
				m_vCollection.clear();
				m_mOwner.clear();
			}
		}
	};

	template <typename T>
	class Collection : public CollectionBase
	{
		friend class Serializable;
	public:
		~Collection() { Clear(); };

		T *newElement() override {
			T* newItem = new T();
			m_vCollection.push_back(newItem);
			m_mOwner[newItem] = true;
			return newItem;
		}
		T* operator[](size_t index) {
			return getItem(index);
		}
		void addItem(T *item) { m_vCollection.push_back(item); m_mOwner[item] = false; };
		T *getItem(int itemID) override { return (T*)m_vCollection.at(itemID); };

		void reset(T* vs, size_t size) {
			for (size_t i = 0; i < size; i++) {
				auto t = this->newElement();
				*t = vs[i];
			}
		}
	};
}