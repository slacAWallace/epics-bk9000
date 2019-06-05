#ifndef		_BX9000_WATCHDOG_H_
#define		_BX9000_WATCHDOG_H_

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * These Beckhoff registers are 16 bits wide
 */
static const unsigned short int	BK_REG_COUPLER_STATUS		=	0x100C;	/* Read only */
static const unsigned short int	BK_REG_WATCHDOG_CURTIME		=	0x1020;	/* Read only */
static const unsigned short int	BK_REG_WATCHDOG_TIMEOUT		=	0x1120;
static const unsigned short int	BK_REG_WATCHDOG_RESET		=	0x1121;
static const unsigned short int	BK_REG_WATCHDOG_TYPE		=	0x1122;

static const unsigned short int	BK_STATUS_WATCHDOG_ELAPSED	=	0x8000;
static const unsigned short int	BK_STATUS_CONFIG_ERROR		=	0x0002;
static const unsigned short int	BK_STATUS_BUS_TERM_ERROR	=	0x0001;

static const unsigned short int	BK_WATCHDOG_RESET_WORD_0	=	0xBECF;
static const unsigned short int	BK_WATCHDOG_RESET_WORD_1	=	0xAFFE;


/**
 *	BK_CheckWatchdog
 *	Checks the status of the watchdog timer.
 *	If fQuiet is FALSE or 0, it prints status to stdout
 *	Returns TRUE if watchdog has elapsed, FALSE if OK
 */
extern unsigned int BK_CheckWatchdog( char * pNameOrIpAddr, unsigned int fQuiet );

/**
 *	MBT_CheckWatchdog
 *	Checks the status of the watchdog timer.
 *	If fQuiet is FALSE or 0, it prints status to stdout
 *	Returns TRUE if watchdog has elapsed, FALSE if OK
 */
extern unsigned int MBT_CheckWatchdog( ModBusTCP_Link mbt_link, unsigned int fQuiet );

/**
 *	BK_SetWatchdogType
 *	Sets the type to either R/W or Write mode.
 *	R/W mode resets the watchdog timer on either a read or a write telegram.
 *	Write mode resets the watchdog timer only on write telegrams.
 *	(A telegram is a message reading or writing a process value);
 *	tyWatchdog = 1 for R/W Telegram Mode
 *	tyWatchdog = 0 for Write Telegram Mode
 */
extern int BK_SetWatchdogType( char * pNameOrIpAddr, unsigned int tyWatchdog );

/**
 *	MBT_SetWatchdogType
 *	Sets the type to either R/W or Write mode.
 *	R/W mode resets the watchdog timer on either a read or a write telegram.
 *	Write mode resets the watchdog timer only on write telegrams.
 *	(A telegram is a message reading or writing a process value);
 *	tyWatchdog = 1 for R/W Telegram Mode
 *	tyWatchdog = 0 for Write Telegram Mode
 */
extern int MBT_SetWatchdogType( ModBusTCP_Link mbt_link, unsigned int tyWatchdog );

/**
 *	BK_ResetWatchdog
 *	Resets the watchdog timer
 */
extern int BK_ResetWatchdog( char * pNameOrIpAddr );

/**
 *	MBT_ResetWatchdog
 *	Resets the watchdog timer
 */
extern int MBT_ResetWatchdog( ModBusTCP_Link mbt_link );

/**
 *	BK_SetWatchdogTimer
 *	Set the watchdog timer to the desired timeout in ms
 */
extern int BK_SetWatchdogTimeout( char * pNameOrIpAddr, unsigned int timeout );

/**
 *	MBT_SetWatchdogTimer
 *	Set the watchdog timer to the desired timeout in ms
 */
extern int MBT_SetWatchdogTimeout( ModBusTCP_Link mbt_link, unsigned int timeout );

/**
 *	BK_DisableWatchdog
 *	Disable the watchdog timer by setting the timeout to zero
 *	To re-enable, use BK_SetWatchdogTimeout()
 */
extern int BK_DisableWatchdog( char * pNameOrIpAddr );

/**
 *	MBT_DisableWatchdog
 *	Disable the watchdog timer by setting the timeout to zero
 *	To re-enable, use MBT_SetWatchdogTimeout()
 */
extern int MBT_DisableWatchdog( ModBusTCP_Link mbt_link );

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif	/*	_BX9000_WATCHDOG_H_	*/
