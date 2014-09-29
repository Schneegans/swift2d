/*
 *  .file oglplus/enums/named_string_type_names.ipp
 *
 *  Automatically generated header file. DO NOT modify manually,
 *  edit 'source/enums/oglplus/named_string_type.txt' instead.
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

namespace enums {
OGLPLUS_LIB_FUNC StrCRef ValueName_(
	NamedStringType*,
	GLenum value
)
#if (!OGLPLUS_LINK_LIBRARY || defined(OGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(OGLPLUS_IMPL_EVN_NAMEDSTRINGTYPE)
#define OGLPLUS_IMPL_EVN_NAMEDSTRINGTYPE
{
switch(value)
{
#if defined GL_SHADER_INCLUDE_ARB
	case GL_SHADER_INCLUDE_ARB: return StrCRef("SHADER_INCLUDE_ARB");
#endif
	default:;
}
OGLPLUS_FAKE_USE(value);
return StrCRef();
}
#else
;
#endif
} // namespace enums

