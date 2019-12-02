#ifndef pragmautils_h
#define pragmautils_h


/* definition to expand macro then apply to pragma message */
#define VALUE_TO_STRING2(x) #x
#define VALUE_TO_STRING(x) VALUE_TO_STRING2(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE_TO_STRING(var)

#define DO_PRAGMA(x) _Pragma (#x)

#define PRAGMA_MESSAGE(desc) DO_PRAGMA( message(__FILE__ "(" VALUE_TO_STRING(__LINE__) ") :" desc) )

/** Some example here

//Some test definition here
#define DEFINED_BUT_NO_VALUE
#define DEFINED_INT 3
#define DEFINED_STR "ABC"

//sample usage
#pragma message(VAR_NAME_VALUE(NOT_DEFINED))
#pragma message(VAR_NAME_VALUE(DEFINED_BUT_NO_VALUE))
#pragma message(VAR_NAME_VALUE(DEFINED_INT))
#pragma message(VAR_NAME_VALUE(DEFINED_STR))

//Above definitions result in:
test.c:10:9: note: #pragma message: NOT_DEFINED=NOT_DEFINED
test.c:11:9: note: #pragma message: DEFINED_BUT_NO_VALUE=
test.c:12:9: note: #pragma message: DEFINED_INT=3
test.c:13:9: note: #pragma message: DEFINED_STR="ABC"
**/

#endif
