/*
  mcp_can.h
  adaptation for visual dummy emulator.
  2016 Copyright (c) Thierry Paris.  All right reserved.

  Author:Thierry Paris
  14/04/2016
*/
#ifndef _MCP2515_H_
#define _MCP2515_H_

#include "mcp_can_dfs.h"

#define MAX_CHAR_IN_MESSAGE 8

class MCP_CAN
{
public:
	MCP_CAN(INT8U _CS) {}
	INT8U begin(INT8U speedset) { return 0; }												/* init can                     */
    INT8U init_Mask(INT8U num, INT8U ext, INT32U ulData) { return 0; }						/* init Masks                   */
    INT8U init_Filt(INT8U num, INT8U ext, INT32U ulData) { return 0; }						/* init filters                 */
    INT8U sendMsgBuf(INT32U id, INT8U ext, INT8U rtr, INT8U len, INT8U *buf) { return 0; }	/* send buf                     */
    INT8U sendMsgBuf(INT32U id, INT8U ext, INT8U len, INT8U *buf) { return 0; }				/* send buf                     */
	INT8U readMsgBuf(INT8U *len, INT8U *buf) { return 0; }									/* read buf                     */
    INT8U readMsgBufID(INT32U *ID, INT8U *len, INT8U *buf) { return 0; }					/* read buf with object ID      */
	INT8U checkReceive(void) { return 0; }													/* if something received        */
	INT8U checkError(void) { return 0; }													/* if something error           */
	INT32U getCanId(void) { return 0; }														/* get can id when receive      */
	INT8U isRemoteRequest(void) { return 0; }												/* get RR flag when receive     */
	INT8U isExtendedFrame(void) { return 0; }												/* did we recieve 29bit frame?  */
};

#endif
