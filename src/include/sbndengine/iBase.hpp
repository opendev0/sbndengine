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

#ifndef __I_BASE_HPP__
#define __I_BASE_HPP__


#include <iostream>
#include <assert.h>

#ifdef DEBUG
	#include <list>
	static std::list<class iBase*> debug_ibase_list;
#endif

/**
 * iBase has to be inherited by all classes which want to make use of iRef
 *
 * This is a kind of garbage collectors without the ability to
 * handle cyclic dependencies
 */
class iBase
{
private:
	int ref_counter;

public:
	iBase()
	{
		// set reference counter to 0 for initialization
		ref_counter = 0;
#ifdef DEBUG
		debug_ibase_list.push_back(this);
#endif
	}

	virtual ~iBase()
	{
		assert(ref_counter == 0);

#ifdef DEBUG
		debug_ibase_list.remove(this);
#endif
	}

	void incRef()
	{
		ref_counter++;
	}

	bool decRef()
	{
		ref_counter--;
		return ref_counter == 0;
	}
};

#endif //__I_BASE_HPP__
