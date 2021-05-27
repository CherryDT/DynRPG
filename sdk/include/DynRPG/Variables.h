namespace RPG {
	/*! \brief Provides easy access to in-game variables

		You may also use RPG::System::variables, but the RPG::Variables class
		will also tolerate negative indexes and is less to type.
		\sa RPG::variables
	*/
	class Variables {
		public:
			//! Array access operator
			inline int &operator[](int index) {
				if(index > 0) return system->variables[index];
				return dummy = 0;
			}

			//! \cond
			int dummy;
			//! \endcond
	};

	/*! \ingroup game_objects
		\brief %Variables array
	*/
	static RPG::Variables variables __attribute__((unused));
}
