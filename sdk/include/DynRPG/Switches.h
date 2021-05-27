namespace RPG {
	/*! \brief Provides easy access to in-game switches

		You may also use RPG::System::switches, but the RPG::Switches class
		will also tolerate negative indexes and is less to type.
		\sa RPG::switches
	*/
	class Switches {
		public:
			//! Array access operator
			inline bool &operator[](int index) {
				if(index > 0) return system->switches[index];
				return dummy = false;
			}

			//! \cond
			bool dummy;
			//! \endcond
	};

	/*! \ingroup game_objects
		\brief %Switches array
	*/
	static RPG::Switches switches __attribute__((unused));
}
