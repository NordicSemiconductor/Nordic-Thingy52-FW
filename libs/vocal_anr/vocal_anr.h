
/* ************************************************************************* */
/*
 *	anrdll.h
 *
 *	(C) 2015 VOCAL Technologies, Ltd.
 *
 *	ALL RIGHTS RESERVED.  PROPRIETARY AND CONFIDENTIAL.
 *
 *	VOCAL Technologies, Ltd.
 *	520 Lee Entrance, Suite 202
 *	Buffalo, NY  14228
 *
 *	Product:	MODEM 101
 *
 *	Module:		ANR
 *
 *	Entry points and structs for the adaptive noise canceller library.
 *
 *	Revision Number:	$Revision$
 *	Revision Status:	$State$
 *	Last Modified:		$Date$
 *	Identification:		$Id$
 *
 *	Revision History:	$Log$
 *
 */
/* ************************************************************************* */

#ifndef	_ANR_DLL_H
#define	_ANR_DLL_H

/* ************************************************************************* */

typedef struct anr_config_data_s {
	int anr_length;
	int anr_delay_length;
	int anr_mode;
	int debug_path_search;
	int debug_generate_noise;
	int debug_dump_stats;
	int debug_channel;
} anr_config_data_t;

int global_mdm_pre_reset(void);
int vocal_application_startup(void);
void anr__debug_dump_data(void);
void anr__initialize (anr_config_data_t *cfg);
sint15 anr__execute_tx (sint15 input);
sint15 anr__execute_rx (sint15 input);

/* ************************************************************************* */
#endif /* _ANR_DLL_H */
