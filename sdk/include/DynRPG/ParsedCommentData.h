namespace RPG {
	/*! \brief Type of a parsed comment parameter
		\sa RPG::ParsedCommentParameter
		\sa RPG::ParsedCommentData
	*/
	enum ParsedCommentParameterType {
		PARAM_NUMBER, //!< The parameter is a number
		PARAM_STRING, //!< The parameter is a string
		PARAM_TOKEN //!< The parameter is a token
	};
	
	/*! \brief Used for parameters of parsed event comments (see \ref event_comments guidelines)
		\sa RPG::ParsedCommentData
	*/
	class ParsedCommentParameter {
		public:
			ParsedCommentParameterType type; //!< Type of the parameter
			double number; //!< Numerical value (if \c type is RPG::PARAM_NUMBER)
			char text[200]; //!< Text value (if \c type is RPG::PARAM_STRING or RPG::PARAM_TOKEN)
	};
	
	/*! \brief Used for parsed event comments (see \ref event_comments guidelines)
		\sa RPG::ParsedCommentParameter
	*/
	class ParsedCommentData {
		public:
			char command[200]; //!< Command name (empty if it's no "special" comment at all)
			int parametersCount; //!< Number of parameters
			ParsedCommentParameter parameters[100]; //!< Parsed parameters
	};
}
