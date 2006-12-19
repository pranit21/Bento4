/*****************************************************************
|
|    AP4 - ByteStream Interface
|
|    Copyright 2002-2006 Gilles Boccon-Gibod & Julien Boeuf
|
|
|    This file is part of Bento4/AP4 (MP4 Atom Processing Library).
|
|    Unless you have obtained Bento4 under a difference license,
|    this version of Bento4 is Bento4|GPL.
|    Bento4|GPL is free software; you can redistribute it and/or modify
|    it under the terms of the GNU General Public License as published by
|    the Free Software Foundation; either version 2, or (at your option)
|    any later version.
|
|    Bento4|GPL is distributed in the hope that it will be useful,
|    but WITHOUT ANY WARRANTY; without even the implied warranty of
|    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|    GNU General Public License for more details.
|
|    You should have received a copy of the GNU General Public License
|    along with Bento4|GPL; see the file COPYING.  If not, write to the
|    Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
|    02111-1307, USA.
|
 ****************************************************************/

#ifndef _AP4_BYTE_STREAM_H_
#define _AP4_BYTE_STREAM_H_

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "Ap4Types.h"
#include "Ap4Interfaces.h"
#include "Ap4Results.h"
#include "Ap4DataBuffer.h"

/*----------------------------------------------------------------------
|   AP4_ByteStream
+---------------------------------------------------------------------*/
class AP4_ByteStream : public AP4_Referenceable
{
 public:
    // methods
    virtual AP4_Result Read(void*     buffer, 
                            AP4_Size  bytes_to_read, 
                            AP4_Size* bytes_read = 0) = 0;
    AP4_Result ReadUI64(AP4_UI64& value);
    AP4_Result ReadUI32(AP4_UI32& value);
    AP4_Result ReadUI24(AP4_UI32& value);
    AP4_Result ReadUI16(AP4_UI16& value);
    AP4_Result ReadUI08(AP4_UI08& value);
    AP4_Result ReadString(char* buffer, AP4_Size size);
    virtual AP4_Result Write(const void* buffer, 
                             AP4_Size    bytes_to_write, 
                             AP4_Size*   bytes_written = 0) = 0;
    AP4_Result WriteString(const char* stringBuffer);
    AP4_Result WriteUI64(AP4_UI64 value);
    AP4_Result WriteUI32(AP4_UI32 value);
    AP4_Result WriteUI24(AP4_UI32 value);
    AP4_Result WriteUI16(AP4_UI16 value);
    AP4_Result WriteUI08(AP4_UI08 value);
    virtual AP4_Result Seek(AP4_Position position) = 0;
    virtual AP4_Result Tell(AP4_Position& position) = 0;
    virtual AP4_Result GetSize(AP4_LargeSize& size) = 0;
    virtual AP4_Result CopyTo(AP4_ByteStream& stream, AP4_LargeSize size);
};

/*----------------------------------------------------------------------
|   AP4_SubStream
+---------------------------------------------------------------------*/
class AP4_SubStream : public AP4_ByteStream
{
 public:
    AP4_SubStream(AP4_ByteStream& container, AP4_Position position, AP4_Size size);

    // AP4_ByteStream methods
    AP4_Result Read(void*    buffer, 
                    AP4_Size  bytes_to_read, 
                    AP4_Size* bytes_read = 0);
    AP4_Result Write(const void* buffer, 
                     AP4_Size    bytes_to_write, 
                     AP4_Size*   bytes_written = 0);
    AP4_Result Seek(AP4_Position position);
    AP4_Result Tell(AP4_Position& position) {
        position = m_Position;
        return AP4_SUCCESS;
    }
    AP4_Result GetSize(AP4_LargeSize& size) {
        size = m_Size;
        return AP4_SUCCESS;
    }

    // AP4_Referenceable methods
    void AddReference();
    void Release();

 protected:
    virtual ~AP4_SubStream();

 private:
    AP4_ByteStream& m_Container;
    AP4_Position    m_Offset;
    AP4_LargeSize   m_Size;
    AP4_Position    m_Position;
    AP4_Cardinal    m_ReferenceCount;
};

/*----------------------------------------------------------------------
|   AP4_MemoryByteStream
+---------------------------------------------------------------------*/
class AP4_MemoryByteStream : public AP4_ByteStream
{
public:
    AP4_MemoryByteStream(AP4_Size size); // filled with zeros
    AP4_MemoryByteStream(AP4_UI08* buffer, AP4_Size size);

    // AP4_ByteStream methods
    AP4_Result Read(void*     buffer, 
                    AP4_Size  bytes_to_read, 
                    AP4_Size* bytes_read = 0);
    AP4_Result Write(const void* buffer, 
                     AP4_Size    bytes_to_write, 
                     AP4_Size*   bytes_written = 0);
    AP4_Result Seek(AP4_Position position);
    AP4_Result Tell(AP4_Position& position) {
        position = m_Position;
        return AP4_SUCCESS;
    }
    AP4_Result GetSize(AP4_LargeSize& size) {
        size = m_Buffer.GetDataSize();
        return AP4_SUCCESS;
    }

    // AP4_Referenceable methods
    void AddReference();
    void Release();

    // methods
    const AP4_UI08* GetData() { return m_Buffer.GetData(); }
    AP4_UI08*       UseData() { return m_Buffer.UseData(); }
    AP4_Size        GetSize() { return m_Buffer.GetDataSize(); }

protected:
    virtual ~AP4_MemoryByteStream() {};

private:
    AP4_DataBuffer m_Buffer;
    AP4_Position   m_Position;
    AP4_Cardinal   m_ReferenceCount;
};

#endif // _AP4_BYTE_STREAM_H_
