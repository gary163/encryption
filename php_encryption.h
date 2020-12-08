/* encryption extension for PHP */

#ifndef PHP_ENCRYPTION_H
# define PHP_ENCRYPTION_H

extern zend_module_entry encryption_module_entry;
# define phpext_encryption_ptr &encryption_module_entry

# define PHP_ENCRYPTION_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_ENCRYPTION)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_ENCRYPTION_H */

