TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
#  Optimization of db files using dbst (DEFAULT: NO)
#DB_OPT = YES

#----------------------------------------------------
# Create and install (or just install)
# databases, templates, substitutions like this
# DB += dbExample1.db
USR_VPATH=../../../Bx9000_MBTApp/Db
DB += Bx9000.db
DB += KL1104.db
DB += KL1124.db
DB += KL1408.db
DB += KL2114.db
DB += KL2124.db
DB += KL2408.db
DB += KL2502.db
DB += KL2512.db
DB += KL2622.db
DB += KL3064.db
DB += KL3102.db
DB += KL3312.db
DB += KL3314.db
DB += KL3408.db
DB += KL3468.db
DB += KL4002.db
DB += KL4132.db

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_TEMPLATE = <templatename>

include $(TOP)/configure/RULES

#----------------------------------------
#  ADD RULES AFTER THIS LINE

dump_env:
	@echo USR_VPATH=$(USR_VPATH)
	@echo VPATH=$(VPATH)
