#include "String.h"

#include <string>

static size_t strlen_s(const char* c, size_t maxlen = INT32_MAX) {
	size_t n = 0;
	while (c[n] != '\0')
	{
		n++;
		if (n >= maxlen - 1) {
			break;
		}
	}
	return n;
}

String String::null = String();

String::String()
{
	length = 0;
	data = new char[0];
}

String::String(char c)
{
	length = 1;
	data = new char(c);
}

String::String(const char* c)
{
	if (c)
	{
		unsigned n = strlen_s(c);
		length = n;
		data = new char[n + 1]{};
		for (unsigned j = 0; j < n; j++)
			data[j] = c[j];
	}
	else
	{
		length = 0;
		data = new char[0];
	}
}

String::String(const String& s)
{
	length = s.len();
	data = new char[length + 1]{};
	for (unsigned j = 0; j < length; j++)
		data[j] = s[j];
}

String::~String()
{
	delete[] data;
}

size_t String::len() const
{
	return length;
}

void String::redistribute(const char * s, size_t length)
{
	delete[] data;
	length = strlen_s(s);

	data = new char[length + 1]{};
	memcpy_s(data, sizeof(char)*length, s, sizeof(char)*length);
}

size_t String::find(const String & s, size_t off) const
{
	std::string s1(this->data);
	std::string s2(s.c_str());

	auto i = s1.find(s2, off);
	if (i == std::string::npos) {
		return npos;
	}
	return i;
}

String & String::replace(size_t off, size_t count, const String & chs)
{
	if (off > length || off + count > length /*|| count > chs.len()*/)
		throw std::exception("OutOfMemoryError");

	std::string s1(this->data);
	std::string s2(chs.c_str());
	auto str = s1.replace(off, count, s2);

	this->redistribute(str.c_str(), str.length());

	return *this;
}

char * String::c_str() const
{
	return data;
}

int String::index(char c) const
{
	for (unsigned j = 0; j < length; j++)
		if (data[j] == c) return (int)j;
	return -1;
}

void String::upcase()
{
	this->upcase(0, length - 1);
}

void String::downcase()
{
	this->downcase(0, length - 1);
}

bool String::empty() const
{
	return length == 0;
}

void String::upcase(unsigned first, unsigned last)
{
	if ((first > last) || (last > length)) throw 1;
	if (first == last)return;
	for (unsigned j = first; j < last; j++)
		if ('a' <= data[j] && data[j] <= 'z')
			data[j] -= ('a' - 'A');
}

void String::downcase(unsigned first, unsigned last)
{
	if ((first >= last) || (last > length)) throw 1;

	for (unsigned j = first; j < last; j++)
		if ('A' <= data[j] && data[j] <= 'Z')
			data[j] += ('a' - 'A');
}

void String::togglecase(unsigned first, unsigned last)
{
	if ((first >= last) || (last > length)) throw 1;

	for (unsigned j = first; j < last; j++)
		if ('A' <= data[j] && data[j] <= 'Z')
			data[j] += ('a' - 'A');
		else if ('a' <= data[j] && data[j] <= 'z')
			data[j] -= ('a' - 'A');
}

//std::ostream& operator<< (std::ostream& os, const String& s)
//{
//	if (s.len() > 0)
//	{
//		for (unsigned j = 0; j < s.len(); j++)
//			os << s[j];
//	}
//	else os << "";
//
//	return os;
//}
//
//std::istream& operator>> (std::istream& is, String& s)
//{
//	char* c = new char[1000];
//	is >> c;
//	s = String(c);
//	delete[] c;
//
//	return is;
//}

char String::operator[] (unsigned j) const
{
	if (j >= length) throw 1;
	return data[j];
}

char& String::operator[] (unsigned j)
{
	if (j >= length) throw 1;
	return data[j];
}

String& String::operator= (const String& s)
{
	if (this == &s) return *this;

	delete[] data;
	length = s.len();
	data = new char[length + 1]{};

	memcpy_s(data, length, s.c_str(), length);

	return *this;
}

String& String::operator=(const char* s) {
	delete[] data;
	auto len = strlen_s(s);
	length = len;
	data = new char[length + 1]{};
	memcpy_s(data, length, s, length);

	return *this;
}

String& String::operator+= (const String& s)
{
	unsigned len = length + s.len();
	char* str = new char[len + 1]{};

	memcpy_s(str, sizeof(char)*len, data, sizeof(char)*(length));

	memcpy_s(str + length, s.len(), s.c_str(), s.len());

	delete[] data;
	length = len;
	data = str;
	return *this;
}

String operator+ (const String& lhs, const String& rhs)
{
	return String(lhs) += rhs;
}

String operator+ (const String& lhs, char rhs)
{
	return String(lhs) += String(rhs);
}

String operator+ (const String& lhs, const char* rhs)
{
	return String(lhs) += String(rhs);
}

String operator+ (char lhs, const String& rhs)
{
	return String(lhs) += rhs;
}
String operator+ (const char* lhs, const String& rhs)
{
	return String(lhs) += rhs;
}

bool operator== (const String& lhs, const String& rhs)
{
	if (lhs.len() != rhs.len()) return false;

	unsigned cap = lhs.len();
	unsigned   n = 0;
	while ((n < cap) && (lhs[n] == rhs[n])) n++;
	return (n == cap);
}

bool operator== (const String& lhs, char rhs)
{
	return (lhs == String(rhs));
}

bool operator== (const String& lhs, const char* rhs)
{
	return (lhs == String(rhs));
}

bool operator== (char lhs, const String& rhs)
{
	return (String(lhs) == rhs);
}

bool operator== (const char* lhs, const String& rhs)
{
	return (String(lhs) == rhs);
}

bool operator> (const String& lhs, const String& rhs)
{
	unsigned cap = (lhs.len() < rhs.len()) ? lhs.len() : rhs.len();
	unsigned n = 0;
	while ((n < cap) && (lhs[n] == rhs[n])) n++;
	if (n == cap) return (lhs.len() > rhs.len());

	if ((('A' <= lhs[n] && lhs[n] <= 'Z') || ('a' <= lhs[n] && lhs[n] <= 'z')) &&
		(('A' <= rhs[n] && rhs[n] <= 'Z') || ('a' <= rhs[n] && rhs[n] <= 'z')))
	{
		char A = (lhs[n] & ~32);
		char B = (rhs[n] & ~32);
		if (A != B) return (A > B);
	}
	return lhs[n] > rhs[n];
}

bool operator> (const String& lhs, char rhs)
{
	return (lhs > String(rhs));
}

bool operator> (const String& lhs, const char* rhs)
{
	return (lhs > String(rhs));
}

bool operator> (char lhs, const String& rhs)
{
	return (String(lhs) > rhs);
}

bool operator> (const char* lhs, const String& rhs)
{
	return (String(lhs) > rhs);
}

bool operator!= (const String& lhs, const String& rhs)
{
	return !(lhs == rhs);
}

bool operator!= (const String& lhs, char rhs)
{
	return !(lhs == rhs);
}

bool operator!= (const String& lhs, const char* rhs)
{
	return !(lhs == rhs);
}

bool operator!= (char lhs, const String& rhs)
{
	return !(lhs == rhs);
}

bool operator!= (const char* lhs, const String& rhs)
{
	return !(lhs == rhs);
}

bool operator< (const String& lhs, const String& rhs)
{
	return !(lhs == rhs) && !(lhs > rhs);
}

bool operator< (const String& lhs, char rhs)
{
	return !(lhs == rhs) && !(lhs > rhs);
}

bool operator< (const String& lhs, const char* rhs)
{
	return !(lhs == rhs) && !(lhs > rhs);
}

bool operator< (char lhs, const String& rhs)
{
	return !(lhs == rhs) && !(lhs > rhs);
}

bool operator< (const char* lhs, const String& rhs)
{
	return !(lhs == rhs) && !(lhs > rhs);
}

bool operator<= (const String& lhs, const String& rhs)
{
	return !(lhs > rhs);
}

bool operator<= (const String& lhs, char rhs)
{
	return !(lhs > rhs);
}

bool operator<= (const String& lhs, const char* rhs)
{
	return !(lhs > rhs);
}

bool operator<= (char lhs, const String& rhs)
{
	return !(lhs > rhs);
}

bool operator<= (const char* lhs, const String& rhs)
{
	return !(lhs > rhs);
}

bool operator>= (const String& lhs, const String& rhs)
{
	return (lhs == rhs) || (lhs > rhs);
}

bool operator>= (const String& lhs, char rhs)
{
	return (lhs == rhs) || (lhs > rhs);
}

bool operator>= (const String& lhs, const char* rhs)
{
	return (lhs == rhs) || (lhs > rhs);
}

bool operator>= (char lhs, const String& rhs)
{
	return (lhs == rhs) || (lhs > rhs);
}

bool operator>= (const char* lhs, const String& rhs)
{
	return (lhs == rhs) || (lhs > rhs);
}