#ifndef _PSXVM_H_
#define _PSXVM_H_

#include "Types.h"
#include "MIPS.h"
#include "PsxBios.h"
#include "Spu.h"
#include "Dmac.h"
#include "Intc.h"
#include "VirtualMachine.h"
#include "MipsExecutor.h"
#include <boost/thread.hpp>

class CPsxVm : public CVirtualMachine
{
public:
						CPsxVm();
	virtual				~CPsxVm();

	void				Reset();
	void				LoadExe(uint8*);
	void				Step();

	CMIPS&				GetCpu();

	virtual STATUS		GetStatus() const;
    virtual void		Pause();
    virtual void		Resume();

private:
	struct EXEHEADER
	{
		uint8	id[8];
		uint32	text;
		uint32	data;
		uint32	pc0;
		uint32	gp0;
		uint32	textAddr;
		uint32	textSize;
		uint32	dataAddr;
		uint32	dataSize;
		uint32	bssAddr;
		uint32	bssSize;
		uint32	stackAddr;
		uint32	stackSize;
		uint32	savedSp;
		uint32	savedFp;
		uint32	savedGp;
		uint32	savedRa;
		uint32	savedS0;
	};

	enum RAMSIZE
	{
		RAMSIZE = 0x00200000
	};

	enum
	{
		HW_REG_BEGIN	= 0x1F801000,
		HW_REG_END		= 0x1F802FFF
	};

	uint32			ReadIoRegister(uint32);
	uint32			WriteIoRegister(uint32, uint32);

	void			ExecuteCpu(bool);
	void			ThreadProc();

	STATUS			m_status;
	uint8*			m_ram;
	Psx::CIntc		m_intc;
	CSpu			m_spu;
	Psx::CDmac		m_dmac;
	CMIPS			m_cpu;
	CMipsExecutor	m_executor;
	CPsxBios		m_bios;
	boost::thread	m_thread;
	bool			m_singleStep;
};

#endif