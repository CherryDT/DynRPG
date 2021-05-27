#define DYNRPG_STATIC
#include "DynRPG.h"

namespace RPG {
	DString::operator std::string () const {
		std::string s = "";
		if(this) s = std::string(reinterpret_cast<const char *>(this), reinterpret_cast<const int *>(this)[-1]);
		return s;
	}

	DString::operator char *() const {
		return (char *)reinterpret_cast<const char *>(this);
	}

	int DString::length() {
		if(this) return reinterpret_cast<const int *>(this)[-1];
		return 0;
	}

	DStringPtr::DStringPtr() {
		str = NULL;
	}

	DStringPtr::DStringPtr(DStringPtr &s) {
		std::string tmp = s;
		assign((char *)tmp.c_str(), tmp.length());
	}

	DStringPtr::DStringPtr(std::string s) {
		str = NULL;
		assign((char *)s.c_str(), s.length());
	}

	DStringPtr::~DStringPtr() {
		clear();
	}

	DStringPtr::operator std::string () {
		return s_str();
	}

	DStringPtr::operator char *() {
		return reinterpret_cast<char *>(str);
	}

	const DStringPtr &DStringPtr::operator=(std::string rhs) {
		assign((char *)rhs.c_str(), rhs.length());
		return *this;
	}

	const DStringPtr &DStringPtr::operator=(char *rhs) {
		assign(rhs, strlen(rhs));
		return *this;
	}

	const DStringPtr &DStringPtr::operator=(const char *rhs) {
		assign((char *)rhs, strlen(rhs));
		return *this;
	}

	const DStringPtr &DStringPtr::operator=(DString *rhs) {
		clear();
		if(rhs) asm volatile("call *%%esi" : "=a" (_eax), "=d" (_edx) : "S" (0x404540), "a" (&str), "d" (rhs) : "ecx", "cc", "memory"); // LStrAsg
		return *this;
	}

	bool DStringPtr::operator==(std::string rhs) {
		return s_str() == rhs;
	}

	bool DStringPtr::operator==(const char *rhs) {
		return s_str() == std::string(rhs);
	}

	bool DStringPtr::operator==(DStringPtr &rhs) {
		return s_str() == rhs.s_str();
	}

	void DStringPtr::clear() {
		if(str) asm volatile("call *%%esi" : "=a" (_eax) : "S" (0x4044EC), "a" (&str) : "edx", "ecx", "cc", "memory"); // LStrClr
	}

	void DStringPtr::assign(char *s, int len) {
		clear();
		if(s && len) asm volatile("call *%%esi" : "=a" (_eax), "=d" (_edx), "=c" (_ecx) : "S" (0x4045DC), "a" (&str), "d" (s), "c" (len) : "cc", "memory"); // LStrFromPCharLen
	}

	std::string DStringPtr::s_str() {
		std::string s = *str;
		return s;
	}

	int DStringPtr::length() {
		return str->length();
	}
}
