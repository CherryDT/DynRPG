namespace RPG {
	/*! \brief Wrapper class for RPG::DList which doesn't crash if the index
		is out of bounds
		\note Please do not ask me why this class is there. I didn't invent it.
		\warning This class has more members than the one which is documented
		(the others are used internally), so <tt>sizeof(RPG::Catalog\<T\>)</tt>
		is not equal to <tt>sizeof(RPG::DList\<T\> *)</tt>!
		\sa RPG::CatalogPtr
		\sa RPG::NamedCatalog
		\sa RPG::NamedCatalogPtr
		\sa RPG::DList
	*/
	template <class T = void *>
	class Catalog {
		public:
			void **vTable;
			//! \cond
			void **itemVTable;
			//! \endcond
			DListPtr<T> list; //!< Pointer to the actual RPG::DList

			//! Array access operator
			T operator[](int index) {
				return get(index);
			}

			//! Array access method
			T get(int index) {
				return list.get(index);
			}

			//! Returns the number of items in the list
			int count() {
				return list.list->count;
			}
	};

	/*! \brief Wrapper class for RPG::Catalog pointers (syntactic sugar)

		Allows you to write <tt>someCatalogPointer[i]</tt> instead of
		<tt>(*someCatalogPointer)[i]</tt>.
		\note The size of this class equals to
		<tt>sizeof(Catalog\<T\> *)</tt>.
		\sa RPG::Catalog
		\sa RPG::NamedCatalogPtr
	*/
	template <class T = void *>
	class CatalogPtr {
		public:
			Catalog<T> *ptr; //!< Pointer to the actual RPG::Catalog

			//! Array access operator
			T operator[](int index) {
				return ptr->get(index);
			}

			//! Array access method
			T get(int index) {
				return ptr->get(index);
			}

			//! Returns the number of items in the list
			int count() {
				return ptr ? ptr->count() : 0;
			}
	};

	/*! \brief Class inherited from RPG::Catalog in which elements with
		non-consecutive IDs are stored without waste of memory (i.e.
		events on a map)

		If you need to enumerate all items in an RPG::NamedCatalog, use
		\code
for(int i = 0; i < myCatalog.count(); i++) {
	SomeObjectType *item = myCatalog.list[i];
	// Processing here...
}
		\endcode
		If you have an RPG::NamedCatalogPtr, it works the same way.
		\sa RPG::Catalog
		\sa RPG::NamedCatalogPtr
	*/
	template <class T = void *>
	class NamedCatalog : public Catalog<T> {
		public:
			//! Array access operator
			T operator[](int index) {
				return get(index);
			}

			//! Array access method
			T get(int index) {
				T ret = NULL;
				asm volatile("call *%%esi" : "=a" (ret), "=d" (_edx) : "S" (0x475740), "a" (this), "d" (index) : "ecx", "cc", "memory"); // GetFromList
				return ret;
			}
	};

	/*! \brief Wrapper class for RPG::NamedCatalog pointers (syntactic sugar)

		Allows you to write <tt>RPG::actors[1]->hp</tt> instead of
		<tt>(*</tt><tt>RPG::actors)[1]->hp</tt>, for example.
		\note The size of this class equals to
		<tt>sizeof(NamedCatalog\<T\> *)</tt>.
		\sa RPG::NamedCatalog
	*/
	template <class T = void *>
	class NamedCatalogPtr {
		public:
			NamedCatalog<T> *ptr;

			//! Array access operator
			T operator[](int index) {
				return ptr->get(index);
			}

			//! Array access method
			T get(int index) {
				return ptr->get(index);
			}

			//! Returns the number of items in the list (not the highest ID!)
			int count() {
				return ptr ? ptr->count() : 0;
			}
	};
}
