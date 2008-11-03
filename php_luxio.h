/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header,v 1.16.2.1.2.1 2007/01/01 19:32:09 iliaa Exp $ */

#ifndef PHP_LUXIO_H
#define PHP_LUXIO_H

extern zend_module_entry luxio_module_entry;
#define phpext_luxio_ptr &luxio_module_entry

#ifdef PHP_WIN32
#define PHP_LUXIO_API __declspec(dllexport)
#else
#define PHP_LUXIO_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define LUXIO_GET_THIS(ce)         (getThis() ? (Z_OBJCE_P(getThis()) == ce ? getThis() : NULL) : NULL)
PHP_MINIT_FUNCTION(luxio);
PHP_MSHUTDOWN_FUNCTION(luxio);
PHP_RINIT_FUNCTION(luxio);
PHP_RSHUTDOWN_FUNCTION(luxio);
PHP_MINFO_FUNCTION(luxio);

PHP_FUNCTION(luxio_btree_ctor);
PHP_FUNCTION(luxio_btree_open);
PHP_FUNCTION(luxio_btree_put);
PHP_FUNCTION(luxio_btree_get);
PHP_FUNCTION(luxio_btree_del);

#ifdef ZTS
#define LUXIO_G(v) TSRMG(luxio_globals_id, zend_luxio_globals *, v)
#else
#define LUXIO_G(v) (luxio_globals.v)
#endif

#define luxio_objprop_get(zv, key, element, on_error) { \
    if (zend_hash_find(Z_OBJPROP(zv), key, strlen(key)+1, (void**)&element) != SUCCESS) { \
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "property [%s] is not set", key); \
        element = NULL; \
        on_error; \
    } \
}
#define luxio_objprop_get_p(zv_p, key, element, on_error)  luxio_objprop_get(*zv_p, key, element, on_error)

#endif	/* PHP_LUXIO_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
