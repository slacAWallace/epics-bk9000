#include "Bx9000_MBT_Common.h"
#include "Bx9000_Watchdog.h"


class	BK_Coupler
{
public:
	BK_Coupler(	const char *	pNameOrIpAddr )
		:	m_fCreated(		FALSE		),
			m_pModBusLink(	NULL		)
	{
		if ( pNameOrIpAddr == NULL )
			return;

		Bx9000_COUPLER	*	pCplr	= NULL;
		pCplr	= Bx9000_Get_Coupler_By_Name( pNameOrIpAddr );
		if ( pCplr != NULL )
		{
			m_pModBusLink	= pCplr->mbt_link;
			return;
		}

		m_pModBusLink = MBT_Init( "BH1", pNameOrIpAddr, 502, 0xFF );
		if ( !m_pModBusLink )
			return;

		if ( MBT_Connect( m_pModBusLink, 3 ) < 0 )
		{
			MBT_Release( m_pModBusLink );
			m_pModBusLink	= NULL;
			return;
		}
		m_fCreated	= true;
	}

	virtual ~BK_Coupler( )
	{
		if ( m_fCreated && m_pModBusLink )
		{
			MBT_Release( m_pModBusLink );
			m_pModBusLink	= NULL;
		}
	}

	ModBusTCP_Link	GetModBusLink( ) const
	{
		return m_pModBusLink;
	}

private:
	bool					m_fCreated;
	struct ModBusTCP_CB	*	m_pModBusLink;
};

extern "C" unsigned int BK_CheckWatchdog( char * pNameOrIpAddr, unsigned int fQuiet )
{
	unsigned short			watchdogElapsed	= FALSE;
	BK_Coupler				bkCoupler(	pNameOrIpAddr ); 
	ModBusTCP_Link			mbt_link		= bkCoupler.GetModBusLink();

	if ( mbt_link )
		watchdogElapsed	= MBT_CheckWatchdog( mbt_link, fQuiet );
	return watchdogElapsed;
}

extern "C" unsigned int MBT_CheckWatchdog( ModBusTCP_Link mbt_link, unsigned int fQuiet )
{
	unsigned short			couplerStatus	= 0;
	unsigned short			watchdogElapsed	= FALSE;
	const unsigned short	timeoutSec		= 2;

	MBT_Function3( mbt_link, BK_REG_COUPLER_STATUS, 1, &couplerStatus, timeoutSec );
	if ( couplerStatus & BK_STATUS_WATCHDOG_ELAPSED )
		watchdogElapsed	= TRUE;
	if ( !fQuiet )
	{
		printf( "Watchdog status is %s.\n", ( watchdogElapsed ? "ELAPSED" : "OK" ) );
	}

	return watchdogElapsed;
}


/*
 *	BK_SetWatchdogType
 *	Sets the type to either R/W or Write mode.
 *	R/W mode resets the watchdog timer on either a read or a write telegram.
 *	Write mode resets the watchdog timer only on write telegrams.
 *	(A telegram is a message reading or writing a process value)
 *	tyWatchdog = 1 for R/W Telegram Mode
 *	tyWatchdog = 0 for Write Telegram Mode
 */
extern "C" int BK_SetWatchdogType( char * pNameOrIpAddr, unsigned int tyWatchdog )
{
	int						result		= 0;
	BK_Coupler				bkCoupler(	pNameOrIpAddr ); 
	ModBusTCP_Link			mbt_link	= bkCoupler.GetModBusLink();

	if ( mbt_link )
		result = MBT_SetWatchdogType( mbt_link, tyWatchdog );

	return result;
}


/*
 *	MBT_SetWatchdogType
 *	Sets the type to either R/W or Write mode.
 *	R/W mode resets the watchdog timer on either a read or a write telegram.
 *	Write mode resets the watchdog timer only on write telegrams.
 *	(A telegram is a message reading or writing a process value)
 *	tyWatchdog = 1 for R/W Telegram Mode
 *	tyWatchdog = 0 for Write Telegram Mode
 */
extern "C" int MBT_SetWatchdogType( ModBusTCP_Link mbt_link, unsigned int tyWatchdog )
{
	const unsigned short	timeoutSec	= 2;

	MBT_Function6(mbt_link, BK_REG_WATCHDOG_TYPE, tyWatchdog, timeoutSec );
	printf( "Watchdog type is now %s.\n", ( tyWatchdog ? "R/W Telegram" : "Write Telegram" ) );

	return 0;
}


extern "C" int BK_ResetWatchdog( char * pNameOrIpAddr )
{
	int						result		= 0;
	BK_Coupler				bkCoupler(	pNameOrIpAddr ); 
	ModBusTCP_Link			mbt_link	= bkCoupler.GetModBusLink();

	if ( mbt_link )
	{
		result = MBT_ResetWatchdog( mbt_link );
		MBT_CheckWatchdog( mbt_link, FALSE );
	}

	return result;
}


extern "C" int MBT_ResetWatchdog( ModBusTCP_Link mbt_link )
{
	const unsigned short	timeoutSec	= 2;

	MBT_Function6(mbt_link, BK_REG_WATCHDOG_RESET, BK_WATCHDOG_RESET_WORD_0, timeoutSec);
	MBT_Function6(mbt_link, BK_REG_WATCHDOG_RESET, BK_WATCHDOG_RESET_WORD_1, timeoutSec);

	return 0;
}


extern "C" int BK_SetWatchdogTimeout( char * pNameOrIpAddr, unsigned int timeout )
{
	int						result		= 0;
	BK_Coupler				bkCoupler(	pNameOrIpAddr ); 
	ModBusTCP_Link			mbt_link	= bkCoupler.GetModBusLink();

	if ( mbt_link )
		result = MBT_SetWatchdogTimeout( mbt_link, timeout );

	return result;
}


extern "C" int MBT_SetWatchdogTimeout( ModBusTCP_Link mbt_link, unsigned int timeout )
{
	const unsigned short	timeoutSec	= 2;
	int						result		= 0;

	result = MBT_ResetWatchdog( mbt_link );
	if ( result < 0 )
	{
		errPrintf( result, __FILE__, __LINE__, "MBT_SetWatchdogTimeout: Unable to reset watchdog!\n" );
		return result;
	}
	result = MBT_Function6(mbt_link, BK_REG_WATCHDOG_TIMEOUT, timeout, timeoutSec);
	if ( result < 0 )
	{
		errPrintf( result, __FILE__, __LINE__, "MBT_SetWatchdogTimeout: Error setting timeout!\n" );
	}
	else
	{
		if ( timeout == 0 )
		{
			printf( "Watchdog timeout is now disabled.\n" );
			printf( "Call BK_SetWatchdogTimeout() to enable.\n\n" );
		}
		else
		{
			printf( "Watchdog timeout is now %dms.\n\n", timeout );
		}
	}

	return result;
}


extern "C" int BK_DisableWatchdog( char * pNameOrIpAddr )
{
	return BK_SetWatchdogTimeout( pNameOrIpAddr, 0 );
}


extern "C" int MBT_DisableWatchdog( ModBusTCP_Link mbt_link )
{
	return MBT_SetWatchdogTimeout( mbt_link, 0 );
}
