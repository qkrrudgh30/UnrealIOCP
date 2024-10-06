// Fill out your copyright notice in the Description page of Project Settings.


#include "CReadableBuffer.h"

CReadableBuffer::CReadableBuffer()
{
}

CReadableBuffer::CReadableBuffer(BYTE* InBuffer, uint32 InCapacity, uint32 InCurrentReadIndex)
	: Buffer(InBuffer)
	, Capacity(InCapacity)
	, CurrentReadIndex(InCurrentReadIndex)
{

}

CReadableBuffer::~CReadableBuffer()
{
}

bool CReadableBuffer::Peek(void* InStartPtrToRead, uint32 InReadLength)
{
	if (GetSlot() < InReadLength)
		return false;

	::memcpy(InStartPtrToRead, &Buffer[CurrentReadIndex], InReadLength);
	return true;
}

bool CReadableBuffer::Read(void* InStartPtrToRead, uint32 InReadLength)
{
	if (Peek(InStartPtrToRead, InReadLength) == false)
		return false;

	CurrentReadIndex += InReadLength;
	return true;
}
