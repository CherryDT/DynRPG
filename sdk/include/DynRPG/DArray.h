namespace RPG {
	//! Array class
	template <class T = void *, int base_ = 0>
	class DArray {
		public:
			int size; //!< Size of the array
			T *items; //!< Pointer to array items

			//! Returns the base of the array (zero in most cases)
			inline int base() {
				return base_;
			}

			/*! \brief Array access operator
				\note This operator will throw an error if you try to access
				negative indexes. However, if you use an index which is higher
				than the current maximum, the array is resized.
			*/
			T &operator[] (int index) {
				index -= base_;
				if(index < 0) throw "DArray index negative";
				if(index >= size) resize(index + 1);
				return items[index];
			}

			/*! \brief Resizes the array

				New elements will be initialized with zero.
				\param newSize The new number of elements in the array
			*/
			void resize(int newSize) {
				int bytes = newSize * sizeof(T);
				asm volatile("call *%%esi" : "=a" (_eax), "=d" (_edx) : "S" (0x4027AC), "a" (&items), "d" (bytes) : "ecx", "cc", "memory");
				for(int i = size; i < newSize; i++) items[i] = 0;
				size = newSize;
			}
	};
}
