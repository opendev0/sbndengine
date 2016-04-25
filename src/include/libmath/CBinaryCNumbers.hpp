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

/*
 * CGlSlMath.hpp
 *
 *  Created on: Oct 18, 2010
 *      Author: martin
 */

#ifndef CBINARY_C_NUMBERS_HPP_
#define CBINARY_C_NUMBERS_HPP_

#define bin_number(number)					\
	(	\
	((number >> 0) & 1) * (1 << 0)	+	\
	((number >> 1) & 1) * (1 << 1)	+	\
	((number >> 2) & 1) * (1 << 2)	+	\
	((number >> 3) & 1) * (1 << 3)	+	\
	((number >> 4) & 1) * (1 << 4)	+	\
	((number >> 5) & 1) * (1 << 5)	+	\
	((number >> 6) & 1) * (1 << 6)	+	\
	((number >> 7) & 1) * (1 << 7)	+	\
	0	)

#endif
