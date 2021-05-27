namespace RPG {
	/*! \brief Helper class representing a Delphi string
		
		This class has no members. It is only "virtual", which means an
		RPG::DString pointer can be used instead of a <tt>char *</tt> to be
		able to access the length information of the string and provide a
		nice automatic cast to std::string even when there are embedded nulls.
		\note In most cases, the RPG::DStringPtr class is used instead of an
		RPG::DString pointer.
		\warning Please mind the \ref rules about strings!
		\sa RPG::DStringPtr
	*/
	class DString {
		public:
			//! Cast operator returning a \c std::string (recommended)
			operator std::string () const;
			
			//! Cast operator returning a <tt>char *</tt>
			operator char *() const;
			
			//! Returns the length of the string
			int length();
	};
	
	/*! \brief Mighty wrapper class for RPG::DString pointers, <i>the</i>
		string class for %RPG Maker strings
		
		This class can also be used to assign a Delphi string. The string will
		then automatically be reallocated if necessary.
		\warning Please mind the \ref rules about strings!
		\sa DString
	*/
	class DStringPtr {
		public:
			DString *str; //!< Pointer to the actual string data
			
			DStringPtr(); //!< Constructor (intializes the pointer to zero)
			DStringPtr(RPG::DStringPtr &s); //!< Copy constructor
			DStringPtr(std::string s); //!< Constructor (uses a \c std::string as template)
			~DStringPtr(); //!< Destructor (deallocates the string when the class is destroyed)
			
			operator std::string (); //!< Cast operator for \c std::string (recommended)
			operator char *(); //!< Cast operator for <tt>char *</tt>
			const RPG::DStringPtr &operator =(std::string rhs); //!< Assignment operator for \c std::string (recommended)
			const RPG::DStringPtr &operator =(char *rhs); //!< Assignment operator for <tt>char *</tt>
			const RPG::DStringPtr &operator =(const char *rhs); //!< Assignment operator for <tt>const char *</tt>
			const RPG::DStringPtr &operator =(RPG::DString *rhs); //!< Can be used to copy a string by writing <tt>newString = oldString.str;</tt>
			bool operator ==(std::string rhs); //!< Comparison operator for \c std::string
			bool operator ==(const char *rhs); //!< Comparison operator for \c char *
			bool operator ==(RPG::DStringPtr &rhs); //!< Comparison oeprator for other RPG::DStringPtr classes
			
			//! Clears the string
			void clear();
			
			//! Assigns the string from a <tt>char *</tt> which may contain null bytes.
			void assign(char *s, int len);
			
			//! Explicit conversion function to \c std::string (recommended)
			std::string s_str();
			
			//! Returns the length of the string
			int length();
	};
}
