/**
 * @file	error_handling.h
 *
 * @date Mar 6, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef ERROR_HANDLING_H_
#define ERROR_HANDLING_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "obd_error_codes.h"
#include "efifeatures.h"
#include "stdbool.h"
#include "rusefi_types.h"

void addWarningCode(obd_code_e code);

/**
 * Something is wrong, but we can live with it: some minor sensor is disconnected
 * or something like that
 *
 */
bool warning(obd_code_e code, const char *fmt, ...);
bool isWarningNow(efitimesec_t now, bool forIndicator);

typedef uint8_t fatal_msg_t[200];
/**
 * Something really bad had happened - firmware cannot function
 *
 * todo: better method name?
 */
void firmwareError(obd_code_e code, const char *fmt, ...);

#define hasFirmwareError() hasFirmwareErrorFlag

char *getFirmwareError(void);

/**
 * declared as a macro so that this code does not use stack
 * so that it would not crash the error handler in case of stack issues
 */
#if CH_DBG_SYSTEM_STATE_CHECK
#define hasFatalError() (dbg_panic_msg != NULL)
#else
#define hasFatalError() (FALSE)
#endif

void chDbgPanic3(const char *msg, const char * file, int line);

void initErrorHandling(void);
char *getWarning(void);

// todo: better place for this shared declaration?
int getRusEfiVersion(void);

/**
 * @deprecated Global panic is inconvenient because it's hard to deliver the error message while whole instance
 * is stopped. Please use firmwareWarning() instead
 */
#if EFI_ENABLE_ASSERTS
  #define efiAssert(condition, message, result) { if (!(condition)) { firmwareError(OBD_PCM_Processor_Fault, message); return result; } }
  #define efiAssertVoid(condition, message) { if (!(condition)) { firmwareError(OBD_PCM_Processor_Fault, message); return; } }
#else /* EFI_ENABLE_ASSERTS */
  #define efiAssert(condition, message, result) { }
  #define efiAssertVoid(condition, message) { }
#endif /* EFI_ENABLE_ASSERTS */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ERROR_HANDLING_H_ */
