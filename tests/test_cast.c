/*
 * Tests if casting within the json_object_get_* functions work correctly.
 * Also checks the json_object_get_type and json_object_is_type functions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json_inttypes.h"
#include "json_object.h"
#include "json_tokener.h"
#include "json_util.h"

static void getit(struct json_object *new_obj, const char *field);
static void checktype_header(void);
static void checktype(struct json_object *new_obj, const char *field);

int main(int argc, char **argv)
{
	const char *input = "{\n\
		\"string_of_digits\": \"123\",\n\
		\"regular_number\": 222,\n\
		\"decimal_number\": 99.55,\n\
		\"boolean_true\": true,\n\
		\"boolean_false\": false,\n\
		\"int64_number\": 2147483649,\n\
		\"negative_number\": -321321321,\n\
		\"a_null\": null,\n\
		\"empty_array\": [],\n\
		\"nonempty_array\": [ 123 ],\n\
		\"array_with_zero\": [ 0 ],\n\
		\"empty_object\": {},\n\
		\"nonempty_object\": { \"a\": 123 },\n\
		\"nan\": NaN,\n\
	}";
	/* Note: 2147483649 = INT_MAX + 2 */
	/* Note: 9223372036854775809 = INT64_MAX + 2 */
	/* Note: 18446744073709551617 = UINT64_MAX + 2 */

	struct json_object *new_obj;

	new_obj = json_tokener_parse(input);
	printf("Parsed input: %s\n", input);
	printf("Result is %s\n", (new_obj == NULL) ? "NULL (error!)" : "not NULL");
	if (!new_obj)
		return 1; // oops, we failed.

	getit(new_obj, "string_of_digits");
	getit(new_obj, "regular_number");
	getit(new_obj, "decimal_number");
	getit(new_obj, "boolean_true");
	getit(new_obj, "boolean_false");
	getit(new_obj, "int64_number");
	getit(new_obj, "negative_number");
	getit(new_obj, "a_null");
	getit(new_obj, "empty_array");
	getit(new_obj, "nonempty_array");
	getit(new_obj, "array_with_zero");
	getit(new_obj, "empty_object");
	getit(new_obj, "nonempty_object");
	getit(new_obj, "nan");

	// Now check the behaviour of the json_object_is_type() function.
	printf("\n================================\n");
	checktype_header();
	checktype(new_obj, NULL);
	checktype(new_obj, "string_of_digits");
	checktype(new_obj, "regular_number");
	checktype(new_obj, "decimal_number");
	checktype(new_obj, "boolean_true");
	checktype(new_obj, "boolean_false");
	checktype(new_obj, "int64_number");
	checktype(new_obj, "negative_number");
	checktype(new_obj, "a_null");
	checktype(new_obj, "nan");

	json_object_put(new_obj);

	return 0;
}

static void getit(struct json_object *new_obj, const char *field)
{
	struct json_object *o = NULL;
	if (!json_object_object_get_ex(new_obj, field, &o))
		printf("Field %s does not exist\n", field);

	enum json_type o_type = json_object_get_type(o);
	printf("new_obj.%s json_object_get_type()=%s\n", field, json_type_to_name(o_type));
	printf("new_obj.%s json_object_get_int()=%d\n", field, json_object_get_int(o));
	printf("new_obj.%s json_object_get_int64()=%" PRId64 "\n", field, json_object_get_int64(o));
	printf("new_obj.%s json_object_get_uint64()=%" PRIu64 "\n", field,
	       json_object_get_uint64(o));
	printf("new_obj.%s json_object_get_boolean()=%d\n", field, json_object_get_boolean(o));
	printf("new_obj.%s json_object_get_double()=%f\n", field, json_object_get_double(o));
}

static void checktype_header(void)
{
	printf("json_object_is_type: %s,%s,%s,%s,%s,%s,%s\n", json_type_to_name(json_type_null),
	       json_type_to_name(json_type_boolean), json_type_to_name(json_type_double),
	       json_type_to_name(json_type_int), json_type_to_name(json_type_object),
	       json_type_to_name(json_type_array), json_type_to_name(json_type_string));
}
static void checktype(struct json_object *new_obj, const char *field)
{
	struct json_object *o = new_obj;
	if (field && !json_object_object_get_ex(new_obj, field, &o))
		printf("Field %s does not exist\n", field);

	printf("new_obj%s%-18s: %d,%d,%d,%d,%d,%d,%d\n", field ? "." : " ", field ? field : "",
	       json_object_is_type(o, json_type_null), json_object_is_type(o, json_type_boolean),
	       json_object_is_type(o, json_type_double), json_object_is_type(o, json_type_int),
	       json_object_is_type(o, json_type_object), json_object_is_type(o, json_type_array),
	       json_object_is_type(o, json_type_string));
}
