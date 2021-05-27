namespace RPG {
	/*! \brief List of event script lines
		\note Do not ask me why this class is there, I didn't invent it.
	*/
	class EventScriptList {
		public:
			void **vTable;
			DList<EventScriptLine *> *list; //!< The actual RPG::DList
	};

	//! Class used for the event script data of an event
	class EventScriptData {
		public:
			void **vTable;
			int _unknown_4;
			int _unknown_8;
			int _unknown_C;
			int _unknown_10;
			EventScriptList *lines; //!< %Event script lines
			int currentLineId; //!< Current line number

			/*! \brief Returns a certain RPG::EventScriptLine
				\param index Line number (zero-based)
				\return %Event script line pointer
			*/
			RPG::EventScriptLine *line(int index);
	};
}
