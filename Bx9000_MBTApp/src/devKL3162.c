#include <Bx9000_MBT_Common.h>
#include <aiRecord.h>

/* KL3162 2-Ch 16 bit Analog Input Module */

extern	SINT32	Bx9000_DEV_DEBUG;
	
static long init_ai_KL3162(struct aiRecord * pai)
{
	if (pai->inp.type!=INST_IO)
	{
		recGblRecordError(S_db_badField, (void *)pai,
			"devAiKL3162 Init_record, Illegal INP");
		pai->pact=TRUE;
		return (S_db_badField);
	}

	if(Bx9000_Signal_Init((dbCommon *) pai, EPICS_RTYPE_AI, pai->inp.value.instio.string, BT_TYPE_KL3162, Bx9000_Dft_ProcFunc, NULL) != 0)
	{
		if(Bx9000_DEV_DEBUG)	errlogPrintf("Fail to init signal for record %s!!", pai->name);
		recGblRecordError(S_db_badField, (void *) pai, "Init signal Error");
		pai->pact = TRUE;
		return (S_db_badField);
	}
	/* KL3162 is single-ended 16-bit */
	pai->eslo = (pai->eguf - pai->egul)/(float)0xFFFF;
	/*	pai->roff = 0x8000; */
	pai->roff = 0x0;

	return 0;
}

static long read_ai_KL3162(struct aiRecord * pai)
{
	Bx9000_SIGNAL	* psignal = (Bx9000_SIGNAL *) (pai->dpvt);

	if (!pai->pact)
	{
		if(epicsMessageQueueTrySend(psignal->pdevdata->pcoupler->msgQ_id, (void *)&psignal, sizeof(Bx9000_SIGNAL *)) == -1)
		{
			recGblSetSevr(pai, READ_ALARM, INVALID_ALARM);
			if(Bx9000_DEV_DEBUG)	errlogPrintf("Send Message to Operation Thread Error [%s]", pai->name);
			return -1;
		}
		else
		{
			pai->pact = TRUE;
		}
	}
	else
	{
		if ( (!psignal->pdevdata->op_done) || psignal->pdevdata->err_code )
		{
			recGblSetSevr(pai, READ_ALARM, INVALID_ALARM);
			if(Bx9000_DEV_DEBUG)	errlogPrintf("Record [%s] receive error code [0x%08x]!\n", pai->name, psignal->pdevdata->err_code);
			return -1;
		}
		else
		{
			pai->udf = FALSE;
		        /* KL3162 is only 0 to 10V  */
			pai->rval = (UINT16)(psignal->pdevdata->value);
		}
	}
	return (CONVERT);
}

static long lincvt_ai_KL3162(struct aiRecord	*pai, int after)
{

	if(!after) return(0);
	/* set linear conversion slope; 3162 is single-ended unsigned 16-bit */
	pai->eslo = (pai->eguf - pai->egul)/(float)0xFFFF;
	/* pai->roff = 0x8000; */
	pai->roff = 0x0;
	return(0);
}


struct {
	long            number;
	DEVSUPFUN       report;
	DEVSUPFUN       init;
	DEVSUPFUN       init_ai;
	DEVSUPFUN       get_ioint_info;
	DEVSUPFUN       read_ai;
	DEVSUPFUN       special_linconv;
}	devAiKL3162 =
{
	6,
	NULL,
	NULL,
	init_ai_KL3162,
	NULL,
	read_ai_KL3162,
	lincvt_ai_KL3162
};
epicsExportAddress(dset, devAiKL3162);

