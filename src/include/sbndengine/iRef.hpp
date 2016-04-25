/*
 * Copyright 2010 Martin Schreiber
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __I_REF_HPP__
#define __I_REF_HPP__

#include <iostream>
#include <assert.h>

/**
 * in combination with the iBase class, this implements a garbage collector
 * for acyclic dependencies
 *
 * IMPORTANT: classname has to implement iBase for reference counter!!!
 *
 * create iref without reference to a class:
 * iRef<classname> iref_to_classname;
 *
 * create iref and class:
 * iRef<classname> iref_to_classname = new classname;
 *
 * call class method:
 * iref_to_classname->some_class_method();
 *
 * iref_to_classname = new classname;
 */
template <typename T>
class iRef
{
public:
	T *ref_class;


	iRef()
	{
		ref_class = NULL;
	}


	/**
	 * create reference by reference
	 */
	iRef(T &p_ref_class)
	{
		ref_class = &p_ref_class;
		ref_class->incRef();
	}


	/**
	 * create reference by pointer
	 */
	iRef(T *p_ref_class)
	{
		ref_class = p_ref_class;
		ref_class->incRef();
	}


	/**
	 * create reference by reference
	 */
	iRef(const iRef<T> &ref)
	{
		ref_class = ref.ref_class;
		ref_class->incRef();
	}


	/**
	 * return pointer to referenced class
	 */
	inline T* operator-> () const
	{
		return ref_class;
	}



	inline iRef<T> &operator=(iRef<T> &ref)
	{
		assert(ref.ref_class != NULL);

		if (ref_class != NULL)
		{
			T *foo = ref_class;

			ref_class = ref.ref_class;
			ref_class->incRef();

			if (foo->decRef())
				delete foo;

			return *this;
		}

		ref_class = ref.ref_class;
		ref_class->incRef();
		return *this;
	}

	/**
	 * assign reference to class
	 */
	inline T &operator=(T &p_ref_class)
	{
		if (ref_class != NULL)
		{
			T *foo = ref_class;

			ref_class = &p_ref_class;
			p_ref_class.incRef();

			if (foo->decRef())
				delete foo;

			return p_ref_class;
		}

		ref_class = &p_ref_class;
		ref_class->incRef();
		return *ref_class;
	}

	inline T *operator=(T *p_ref_class)
	{
		return &operator=(*p_ref_class);
	}

	inline T &operator*()
	{
		return *ref_class;
	}

	virtual ~iRef()
	{
		if (ref_class == NULL)
			return;

		if (ref_class->decRef())
			delete ref_class;
	}

	inline void release()
	{
		if (ref_class == NULL)
		{
			std::cerr << "iRef::release: release() called, but nothing to release!" << std::endl;
			return;
		}

		if (ref_class->decRef())
			delete ref_class;

		ref_class = NULL;
	}

	inline bool isNotNull()	const
	{
		return ref_class != NULL;
	}


	inline bool isNull()	const
	{
		return ref_class == NULL;
	}

	inline T &getClass()	const
	{
		return *ref_class;
	}

	inline bool operator!()	const
	{
		return ref_class == NULL;
	}

	inline bool operator==(const iRef<T> &object)	const
	{
		// compare pointers
		return ref_class == object.ref_class;
	}

	inline bool operator==(const T &object)	const
	{
		// compare pointers
		return ref_class == &object;
	}
};

#endif //__I_OBJECT_HPP__
