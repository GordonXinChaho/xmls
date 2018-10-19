#pragma once

#ifdef XMLS_DLL_EXPORT
#define XMLS_DLL_API __declspec(dllexport)
#else
#define XMLS_DLL_API __declspec(dllimport)
#pragma comment(lib,"xmls.lib")
#endif // XMLS_DLL_EXPORT

class XMLS_DLL_API String
{
public:
	static const size_t npos = 4294967295;
	static String null;
private:
	char*	data;
	size_t length;
public:
	String();
	String(char c);
	String(const char* c);
	String(const String& s);
	~String();

	size_t len() const;
	void redistribute(const char *s, size_t length);

	size_t find(const String& s, size_t off) const;

	String& replace(size_t off, size_t count, const String& chs);

	char* c_str() const;
	int index(char c) const;
	void upcase();
	void downcase();
	bool empty() const;
	void upcase(unsigned first, unsigned last);
	void downcase(unsigned first, unsigned last);
	void togglecase(unsigned first, unsigned last);
	char  operator[] (unsigned j) const;
	char& operator[] (unsigned j);

	String& operator= (const String& s);
	String& operator=(const char* s);
	String& operator+= (const String& s);

	friend String operator+ (const String& lhs, const String& rhs);
	friend String operator+ (const String& lhs, char          rhs);
	friend String operator+ (const String& lhs, const char*   rhs);
	friend String operator+ (char          lhs, const String& rhs);
	friend String operator+ (const char*   lhs, const String& rhs);

	friend bool operator== (const String& lhs, const String& rhs);
	friend bool operator== (const String& lhs, char          rhs);
	friend bool operator== (const String& lhs, const char*   rhs);
	friend bool operator== (char          lhs, const String& rhs);
	friend bool operator== (const char*   lhs, const String& rhs);

	friend bool operator> (const String& lhs, const String& rhs);
	friend bool operator> (const String& lhs, char          rhs);
	friend bool operator> (const String& lhs, const char*   rhs);
	friend bool operator> (char          lhs, const String& rhs);
	friend bool operator> (const char*   lhs, const String& rhs);

	friend bool operator!= (const String& lhs, const String& rhs);
	friend bool operator!= (const String& lhs, char          rhs);
	friend bool operator!= (const String& lhs, const char*   rhs);
	friend bool operator!= (char          lhs, const String& rhs);
	friend bool operator!= (const char*   lhs, const String& rhs);

	friend bool operator< (const String& lhs, const String& rhs);
	friend bool operator< (const String& lhs, char          rhs);
	friend bool operator< (const String& lhs, const char*   rhs);
	friend bool operator< (char          lhs, const String& rhs);
	friend bool operator< (const char*   lhs, const String& rhs);

	friend bool operator<= (const String& lhs, const String& rhs);
	friend bool operator<= (const String& lhs, char          rhs);
	friend bool operator<= (const String& lhs, const char*   rhs);
	friend bool operator<= (char          lhs, const String& rhs);
	friend bool operator<= (const char*   lhs, const String& rhs);

	friend bool operator>= (const String& lhs, const String& rhs);
	friend bool operator>= (const String& lhs, char          rhs);
	friend bool operator>= (const String& lhs, const char*   rhs);
	friend bool operator>= (char          lhs, const String& rhs);
	friend bool operator>= (const char*   lhs, const String& rhs);
};