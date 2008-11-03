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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_luxio.h"
}

#include "luxio/btree.h"
/* If you declare any globals in php_luxio.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(luxio)
*/

/* True global resources - no need for thread safety here */
static int le_luxio;
static zend_class_entry *luxio_entry_ptr = NULL;

static void _php_luxio_resource_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC);

/* {{{ luxio_functions[]
 *
 * Every user visible function must have an entry in luxio_functions[].
 */
zend_function_entry luxio_functions[] = {
	PHP_FE(luxio_btree_ctor,	NULL)
	PHP_FE(luxio_btree_open,	NULL)
	PHP_FE(luxio_btree_put,	NULL)
	PHP_FE(luxio_btree_get,	NULL)
	PHP_FE(luxio_btree_del,	NULL)
	{NULL, NULL, NULL}	/* Must be the last line in luxio_functions[] */
};
/* }}} */

zend_function_entry luxio_btree_functions[] = {
	PHP_FALIAS(luxiobtree, luxio_btree_ctor,	NULL)
	PHP_FALIAS(open, luxio_btree_open,	NULL)
	PHP_FALIAS(put, luxio_btree_put,	NULL)
	PHP_FALIAS(get, luxio_btree_get,	NULL)
	PHP_FALIAS(del, luxio_btree_del,	NULL)
	{NULL, NULL, NULL}
};

/* {{{ luxio_module_entry
 */
zend_module_entry luxio_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"luxio",
	luxio_functions,
	PHP_MINIT(luxio),
	PHP_MSHUTDOWN(luxio),
	PHP_RINIT(luxio),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(luxio),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(luxio),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_LUXIO
ZEND_GET_MODULE(luxio)
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(luxio)
{
    le_luxio = zend_register_list_destructors_ex(_php_luxio_resource_dtor, NULL, "luxio object", module_number);

    REGISTER_LONG_CONSTANT("LUX_DB_RDONLY", Lux::DB_RDONLY, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("LUX_DB_RDWR", Lux::DB_RDWR, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("LUX_DB_CREAT", Lux::DB_CREAT, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("LUX_DB_TRUNC", Lux::DB_TRUNC, CONST_CS | CONST_PERSISTENT);

    zend_class_entry luxio_entry;
    INIT_CLASS_ENTRY(luxio_entry, "luxiobtree", luxio_btree_functions);
    luxio_entry_ptr = zend_register_internal_class(&luxio_entry TSRMLS_CC);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(luxio)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(luxio)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(luxio)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(luxio)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "luxio support", "enabled");
	php_info_print_table_end();
}
/* }}} */

// {{{ _php_luxio_create()
static void _php_luxio_create(zval *obj TSRMLS_DC)
{   
    Lux::IO::Btree *bt = new Lux::IO::Btree(Lux::IO::CLUSTER);

    int ret = zend_list_insert(bt, le_luxio);
    add_property_resource(obj, "luxio", ret);
}
// }}}

// {{{ _php_luxio_connection_resource_dtor()
static void _php_luxio_resource_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC) {
    ((Lux::IO::Btree *)rsrc->ptr)->close();
    delete ((Lux::IO::Btree *)rsrc->ptr);
}
// }}}

// {{{ _php_luxio_get_instance()
static void *_php_luxio_get_instance(zval *obj TSRMLS_DC) {
    zval **tmp;
    luxio_objprop_get_p(obj, "luxio", tmp, 0);
    if (tmp == NULL) {
        return NULL;
    }
    int type;
    Lux::IO::Btree *bt = NULL;
    bt = (Lux::IO::Btree *)zend_list_find(Z_LVAL_PP(tmp), &type);
    return bt;
}
// }}}

// {{{ luxio_btree_ctor()
PHP_FUNCTION(luxio_btree_ctor) 
{
    zval *obj = LUXIO_GET_THIS(luxio_entry_ptr);
    if (!obj) {
        RETURN_FALSE;
    }
    _php_luxio_create(obj TSRMLS_CC);
}
// }}}

// {{{ luxio_btree_open()
PHP_FUNCTION(luxio_btree_open) 
{
    zval *obj = LUXIO_GET_THIS(luxio_entry_ptr);
    Lux::IO::Btree *bt = (Lux::IO::Btree *)_php_luxio_get_instance(obj);

    const char *filename = NULL;
    size_t filename_len = 0;
    int db_flags;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &filename, &filename_len, &db_flags) == FAILURE) {
        return;
    }

    if (bt->open(filename, db_flags)) {
        RETURN_TRUE
    }
    RETURN_FALSE
}
// }}}

// {{{ luxio_btree_put()
PHP_FUNCTION(luxio_btree_put) 
{
    zval *obj = LUXIO_GET_THIS(luxio_entry_ptr);
    Lux::IO::Btree *bt = (Lux::IO::Btree *)_php_luxio_get_instance(obj);

    const char *key = NULL, *value = NULL;
    size_t key_len = 0, value_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &key, &key_len, &value, &value_len) == FAILURE) {
        return;
    }

    Lux::IO::data_t lux_key = {key, key_len};
    Lux::IO::data_t lux_val = {value, value_len};
    if (bt->put(&lux_key, &lux_val)) {
        RETURN_TRUE;
    }
    RETURN_FALSE;
}
// }}}

// {{{ luxio_btree_get()
PHP_FUNCTION(luxio_btree_get) 
{
    zval *obj = LUXIO_GET_THIS(luxio_entry_ptr);
    Lux::IO::Btree *bt = (Lux::IO::Btree *)_php_luxio_get_instance(obj);

    const char *key = NULL, *value = NULL;
    size_t key_len = 0, value_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        return;
    }

    Lux::IO::data_t lux_key = {key, key_len};
    Lux::IO::data_t *lux_val = bt->get(&lux_key);
    if (!lux_val) {
        RETURN_FALSE;
    }

    ZVAL_STRINGL(return_value, (char *)lux_val->data, lux_val->size, 1);
    bt->clean_data(lux_val);
}
// }}}

// {{{ luxio_btree_del()
PHP_FUNCTION(luxio_btree_del) 
{
    zval *obj = LUXIO_GET_THIS(luxio_entry_ptr);
    Lux::IO::Btree *bt = (Lux::IO::Btree *)_php_luxio_get_instance(obj);

    const char *key = NULL, *value = NULL;
    size_t key_len = 0, value_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        return;
    }

    Lux::IO::data_t lux_key = {key, key_len};
    if (bt->del(&lux_key)) {
        RETURN_TRUE
    }
    RETURN_FALSE
}
// }}}
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
