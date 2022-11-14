// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"

/* Modify return point */
void UpdateProgramCounter()
{
	int prevPCReg;
	int currPCReg;
	int nextPCReg;

	prevPCReg = kernel->machine->ReadRegister(PCReg);
	currPCReg = prevPCReg + 4;
	nextPCReg = currPCReg + 4;

	/* set previous programm counter (debugging only)*/
	kernel->machine->WriteRegister(PrevPCReg, prevPCReg);

	/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	kernel->machine->WriteRegister(PCReg, currPCReg);

	/* set next programm counter for brach execution */
	kernel->machine->WriteRegister(NextPCReg, nextPCReg);
}

/**
 * @brief Copy system string to user string
 *
 * @param str string to copy
 * @param addr addess of user string
 * @param copy_len set max length of string to copy, leave
 * blank to copy all characters of system string
 * @return void
 */
void CopyStringOSToUser(char *str, int addr, int copy_len = -1)
{
	if (copy_len == -1)
	{
		copy_len = strlen(str);
	}
	for (int i = 0; i < copy_len; i++)
	{
		kernel->machine->WriteMem(addr + i, 1, str[i]);
	}
	kernel->machine->WriteMem(addr + copy_len, 1, '\0');
}

/**
 * @brief Copy user string to system string
 *
 * @param addr addess of user string
 * @param copy_len set max length of string to copy, leave
 * blank to copy all characters of user string
 * @return char*
 */
// char *CopyStringUserToOS(int addr, int copy_len = -1)
// {
// 	int len = 0;
// 	bool stop = false;
// 	char *str;

// 	do
// 	{
// 		int oneChar;
// 		kernel->machine->ReadMem(addr + len, 1, &oneChar);
// 		len++;
// 		// if copy_length == -1, we use '\0' to terminate the process
// 		// otherwise, we use copy_length to terminate the process
// 		stop = ((oneChar == '\0' && copy_len == -1) ||
// 				len == copy_len);
// 	} while (!stop);

// 	str = new char[len];
// 	for (int i = 0; i < len; i++)
// 	{
// 		int oneChar;
// 		kernel->machine->ReadMem(addr + i, 1,
// 								 &oneChar); // copy characters to kernel space
// 		str[i] = (unsigned char)oneChar;
// 	}
// 	return str;
// }
char *CopyStringUserToOS(int addr)
{
	int len = 0;
	bool stop = false;
	char *str;
	do
	{
		int oneChar;
		kernel->machine->ReadMem(addr + len, 1, &oneChar);
		len++;
		// if copy_length == -1, we use '\0' to terminate the process
		// otherwise, we use copy_length to terminate the process
		stop = (oneChar == '\0');
	} while (!stop);

	str = new char[len];
	for (int i = 0; i < len; i++)
	{
		int oneChar;
		kernel->machine->ReadMem(addr + i, 1,
								 &oneChar); // copy characters to kernel space
		str[i] = (unsigned char)oneChar;
	}
	return str;
}

//----------------------------------------------------------------------
// Handle System call Exceptions
//----------------------------------------------------------------------
void Handle_SC_Add()
{
	DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");

	/* Process SysAdd Systemcall*/
	int result;
	result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
					/* int op2 */ (int)kernel->machine->ReadRegister(5));

	DEBUG(dbgSys, "Add returning with " << result << "\n");

	/* Prepare Result */
	kernel->machine->WriteRegister(2, (int)result);

	/* Modify return point */
	UpdateProgramCounter();
}

/**
 * @brief Process when System call ReadString is called
 * @return void
 */
#define MAX_READ_STRING_LENGTH 255
void Handle_SC_ReadString()
{
	// store user string memory address and user length string
	int memPtr = kernel->machine->ReadRegister(4); // read address of C-string
	int length = kernel->machine->ReadRegister(5); // read max_length of C-string

	if (length > MAX_READ_STRING_LENGTH)
	{ // avoid allocating large memory
		DEBUG(dbgSys, "[Debug] String length exceeds " << MAX_READ_STRING_LENGTH);
		SysHalt();
	}

	// read string from os space
	char *buffer = SysReadString(length);

	DEBUG(dbgSys, "[Debug] String to read from console: '" << buffer << "'\n");

	// copy OS string (buffer) to user string in memPtr
	CopyStringOSToUser(buffer, memPtr);
	delete[] buffer;

	// go to next command
	UpdateProgramCounter();
}

/**
 * @brief Process when System call PrintString is called
 * @return void
 */
void Handle_SC_PrintString()
{
	// read string from user space
	int memPtr = kernel->machine->ReadRegister(4);

	// copy user string to os string
	char *buffer = CopyStringUserToOS(memPtr);

	DEBUG(dbgSys, "[Debug] String to print in console: '" << buffer << "'\n");

	// print os string to console
	SysPrintString(buffer);
	delete[] buffer;

	// go to next command
	UpdateProgramCounter();
}

/**
 * @brief Process when System call ReadNum is called
 * @return void
 */
void Handle_SC_ReadNum()
{
	int number;
	number = SysReadNum();
	DEBUG(dbgSys, "[Debug] Number to read from console: " << number << "\n");
	kernel->machine->WriteRegister(2, number);

	UpdateProgramCounter();
}

/**
 * @brief Process when System call PrintNum is called
 * @return void
 */
void Handle_SC_PrintNum()
{
	DEBUG(dbgSys, "[Debug] Number to print to console: " << kernel->machine->ReadRegister(4) << "\n");
	SysPrintNum(kernel->machine->ReadRegister(4));
	UpdateProgramCounter();
}

/**
 * @brief Process when System call RandomNum is called
 * @return void
 */
void Handle_SC_RandomNum()
{
	int number;
	number = SysRadomNum();

	DEBUG(dbgSys, "[Debug] Number to random from console: " << number << "\n");
	kernel->machine->WriteRegister(2, number);

	UpdateProgramCounter();
}

/**
 * @brief Process when System call ReadChar is called
 * @return void
 */
void Handle_SC_ReadChar()
{
	char oneChar;

	oneChar = SysReadChar();

	DEBUG(dbgSys, "[Debug] Char read from console: '" << oneChar << "'\n");
	kernel->machine->WriteRegister(2, oneChar);

	UpdateProgramCounter();
}

/**
 * @brief Process when System call PrintChar is called
 * @return void
 */
void Handle_SC_PrintChar()
{
	char oneChar = kernel->machine->ReadRegister(4);

	DEBUG(dbgSys, "[Debug] Char to print to console: '" << oneChar << "'\n");
	SysPrintChar(oneChar);

	UpdateProgramCounter();
}

/**
 * @brief Process when System call Create is called
 * @return void
*/
void Handle_SC_Create()
{
	DEBUG(dbgSys, "[Debug] System call create");

	// Take address of buffer file name
	DEBUG(dbgSys, "[Debug] Reading address of file name");
	int buffAddr = kernel->machine->ReadRegister(4);

	// Convert user to OS
	DEBUG(dbgSys, "[Debug] Reading file name");
	char *buffer = CopyStringUserToOS(buffAddr);

	if (buffer == NULL)
	{
		DEBUG(dbgSys, "[Debug] Not enough memory in system");
		kernel->machine->WriteRegister(2, -1);
	}
	else
	{
		DEBUG(dbgSys, "[Debug] Finish reading file name");

		if (!kernel->fileSystem->Create(buffer))
		{
			DEBUG(dbgSys, "[Debug] Can not create file");
			kernel->machine->WriteRegister(2, -1);
		}
		else
		{
			DEBUG(dbgSys, "[Debug] Create file successfully");
			kernel->machine->WriteRegister(2, 0);
		}
	}

	delete[] buffer;	
	UpdateProgramCounter();
}

/**
 * @brief Process when System call remove is called
 * @return void
 */
void Handle_SC_Remove()
{
	// Take address of buffer file name
	int buffAddr = kernel->machine->ReadRegister(4);

	// Convert user to OS
	char *buffer = CopyStringUserToOS(buffAddr);

	int result = SysRemoveFile(buffer);

	// Remove success
	if (result == 0)
	{
		DEBUG(dbgSys, "[Debug] Remove file " << buffer << " completed !!!\n");
	}
	// Remove fail
	else if(result == -1)
	{
		DEBUG(dbgSys, "[Debug] Can not remove file \n");
	}

	kernel->machine->WriteRegister(2, result);

	delete[] buffer;

	UpdateProgramCounter();
}

/**
 * @brief Process when System call Open is called
 * @return void
 */
void Handle_SC_Open()
{
	// Take address of buffer file name
	int buffAddr = kernel->machine->ReadRegister(4);

	// Convert user to OS
	char *buffer = CopyStringUserToOS(buffAddr);

	// Take OpenFileId of file 
	OpenFileId result = SysOpenFile(buffer);

	// Open succeed
	if (result == 0)
	{
		DEBUG(dbgSys, "[Debug] stdin mode \n");
	}
	else if(result == 1)
	{
		DEBUG(dbgSys, "[Debug] stdout mode \n");
	}
	// Open fail
	else if(result == -1)
	{
		DEBUG(dbgSys, "[Debug] Can not open file " << buffer << "\n");
	}
	else{
		DEBUG(dbgSys, "[Debug] Open file " << buffer << " completed !!!\n");
	}
	kernel->machine->WriteRegister(2, result);

	delete[] buffer;

	UpdateProgramCounter();
}

/**
 * @brief Process when System call Close is called
 * @return void
 */
void Handle_SC_Close()
{
	// Take OpenFileId of file
	int id = kernel->machine->ReadRegister(4);

	// Save address file

	// Take result of SysCloseFile process
	int result = SysCloseFile(id);

	// Close succeed
	if (result == 0)
	{
		DEBUG(dbgSys, "[Debug] Closed file !!! \n");
	}
	// Close fail
	else if(result == -1) {
		DEBUG(dbgSys, "[Debug] File at is not open !!! \n");	
	}

	kernel->machine->WriteRegister(2, result);

	UpdateProgramCounter();
}

void Handle_SC_Seek()
{
	int pos = kernel->machine->ReadRegister(4);
	int id = kernel->machine->ReadRegister(5);

	if(id == 0 || id == 1)
	{
		DEBUG(dbgSys, "Can not seek file in console !!!\n");
		kernel->machine->WriteRegister(2, -1);
		UpdateProgramCounter();
		return;
	}

	int result = SysSeekFile(pos, id);

	if(result != -1)
	{
		DEBUG(dbgSys, "Seek file completed !!!\n");
	}
	else{
		DEBUG(dbgSys, "Seek file fail !!!\n");
	}
	
	kernel->machine->WriteRegister(2, result);

	UpdateProgramCounter();
}

/*
Nguyễn Anh Tuấn - 20120395
Input:
	@bufAddr: User buffer address
	@sizeBuf: User buffer size
	@fileId: file Id to read
Output:
	@numRead: number characters have been read
*/
void Handle_SC_Read() {
	// Get parameters value
	int bufAddr = kernel->machine->ReadRegister(4);
	int sizeBuf = kernel->machine->ReadRegister(5);
	OpenFileId fileId = kernel->machine->ReadRegister(6);

	// If file is opened
	if (isOpen(fileId)) {		
		// Allocate Os buffer memory 
		char* buffer = new char[sizeBuf];
		
		OpenFile* opf = kernel->fileSystem->openf[fileId];

		DEBUG(dbgSys, "[Debug] File length " << opf->Length() <<  "\n");

		// Read file and store in OS buffer
		int numRead = opf->Read(buffer, sizeBuf);

		DEBUG(dbgSys, "[Debug] Number bytes is read: " << numRead << "' from fileId " << fileId << "\n");

		// copy string from OS buffer to User buf
		CopyStringOSToUser(buffer, bufAddr);

		DEBUG(dbgSys, "[Debug] Read a string '" << buffer << "' from fileId " << fileId << "\n");

		// deallocate Os buffer
		delete[] buffer;

		// Assign return value to register 2
		kernel->machine->WriteRegister(2, numRead);
	} 
	else {
		DEBUG(dbgSys, "[Debug] Error read to unopened file !!!\n");

		// Assign return value to register 2
		kernel->machine->WriteRegister(2, -1);
	}
	// go to next instruction
	UpdateProgramCounter();
}

/*
Nguyen Anh Tuan - 20120395
Input:
	@bufAddr: User buffer address
	@sizeBuf: User buffer size
	@fileId: file Id to write
Output:
	@numRead: number characters have been written
*/
void Handle_SC_Write() {
	// Get parameters value
	int bufAddr = kernel->machine->ReadRegister(4);
	int sizeBuf = kernel->machine->ReadRegister(5);
	OpenFileId fileId = kernel->machine->ReadRegister(6);

	// If file is opened
	if (isOpen(fileId)) {
		OpenFile* opf = kernel->fileSystem->openf[fileId];

		// Allocate Os buffer memory
		char* buffer = CopyStringUserToOS(bufAddr);

		DEBUG(dbgSys, "[Debug] Write a string '" << buffer << "' to fileId " << fileId << "\n");

		// calculate size of string to write
		int sizeWrite = (sizeBuf > StringLength(buffer))?StringLength(buffer):sizeBuf;

		// Write buffer to file
		int numWrite = opf->Write(buffer, sizeWrite);

		// deallocate Os buffer
		delete[] buffer;

		// Assign return value to register 2
		kernel->machine->WriteRegister(2, numWrite);
	} 
	else {
		DEBUG(dbgSys, "[Debug] Error write to unopened file !!!\n");

		// Assign return value to register 2
		kernel->machine->WriteRegister(2, -1);
	}
	// go to next instruction
	UpdateProgramCounter();
}


void Handle_SC_Halt()
{
	DEBUG(dbgSys, "[Debug] Shutdown, initiated by user program.\n");
	SysHalt();
	ASSERTNOTREACHED();
}

//----------------------------------------------------------------------
// Handle Non System call Exceptions | User mode Exceptions
//----------------------------------------------------------------------
// return control to kernel
void Handle_NoException()
{
	kernel->interrupt->setStatus(SystemMode);
	DEBUG(dbgSys, "Switch to system mode\n");
}

void Handle_PageFaultException()
{
	cerr << "Error " << PageFaultException << " occurs\n";
	SysHalt();
	ASSERTNOTREACHED();
}

void Handle_ReadOnlyException()
{
	cerr << "Error " << ReadOnlyException << " occurs\n";
	SysHalt();
	ASSERTNOTREACHED();
}

void Handle_BusErrorException()
{
	cerr << "Error " << BusErrorException << " occurs\n";
	SysHalt();
	ASSERTNOTREACHED();
}

void Handle_AddressErrorException()
{
	cerr << "Error " << AddressErrorException << " occurs\n";
	SysHalt();
	ASSERTNOTREACHED();
}

void Handle_OverflowException()
{
	cerr << "Error " << OverflowException << " occurs\n";
	SysHalt();
	ASSERTNOTREACHED();
}

void Handle_IllegalInstrException()
{
	cerr << "Error " << IllegalInstrException << " occurs\n";
	SysHalt();
	ASSERTNOTREACHED();
}

void Handle_NumExceptionTypes()
{
	cerr << "Error " << NumExceptionTypes << " occurs\n";
	SysHalt();
	ASSERTNOTREACHED();
}

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------

void ExceptionHandler(ExceptionType which)
{
	int type = kernel->machine->ReadRegister(2);

	DEBUG(dbgSys, "[Debug] Received Exception " << which << " type: " << type << "\n");

	switch (which)
	{
	case NoException:
		return Handle_NoException();

	case PageFaultException:
		return Handle_PageFaultException();

	case ReadOnlyException:
		return Handle_ReadOnlyException();

	case BusErrorException:
		return Handle_BusErrorException();

	case AddressErrorException:
		return Handle_AddressErrorException();

	case OverflowException:
		return Handle_OverflowException();

	case IllegalInstrException:
		return Handle_IllegalInstrException();

	case NumExceptionTypes:
		return Handle_NumExceptionTypes();

	case SyscallException:
		switch (type)
		{
		case SC_ReadString:
			return Handle_SC_ReadString();

		case SC_PrintString:
			return Handle_SC_PrintString();

		case SC_Halt:
			return Handle_SC_Halt();

		case SC_Add:
			return Handle_SC_Add();

		case SC_ReadNum:
			return Handle_SC_ReadNum();

		case SC_PrintNum:
			return Handle_SC_PrintNum();

		case SC_RandomNum:
			return Handle_SC_RandomNum();

		case SC_ReadChar:
			return Handle_SC_ReadChar();

		case SC_PrintChar:
			return Handle_SC_PrintChar();

		case SC_Open:
			return Handle_SC_Open();

		case SC_Close:
			return Handle_SC_Close();

		case SC_Seek:
			return Handle_SC_Seek();
			
		case SC_Create:
			return Handle_SC_Create();

		case SC_Read:
			return Handle_SC_Read(); 

		case SC_Write:
			return Handle_SC_Write(); 

		case SC_Remove:
			return Handle_SC_Remove();

		default:
			cerr << "Unexpected system call " << type << "\n";
			break;
		}
		break;
	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
		break;
	}
	ASSERTNOTREACHED();
}