/**
 * Copyright 2017 Shusheng Shao <iblackangel@163.com>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _RAPIDJSON_MAN_H_
#define _RAPIDJSON_MAN_H_

#include <rapidjson/document.h>

// ----------VERY IMPORTENT-----------
// 
// should be compiled with
//
//         RAPIDJSON_HAS_STDSTRING
//
// macro


RAPIDJSON_NAMESPACE_BEGIN

// Type of JSON value
enum Type{
	kNullType = 0,				// null
	kFalseType = 1,				// false
	kTrueType = 2,				// true
	kObjectType = 3,			// object
	kArrayType = 4, 			// array
	kStringType = 5,			// string
	kNumberType = 6				// number
};

// Runtime-specified UTF encoding type of a stream.
enum UTFType {
	kUTF8 = 0,					// UTF-8
	kUTF16LE = 1,				// UTF-16 little endian
	kUTF16BE = 2,				// UTF-16 big endian
	kUTF32LE = 3,				// UTF-32 little endian
	kUTF32BE = 4				// UTF-32 big endian
};

#ifndef RAPIDJSON_PARSE_DEFAULT_FLAGS
#define RAPIDJSON_PARSE_DEFAULT_FLAGS kParseNoFlags
#endif

enum ParseFlag {
	kParseNoFlags = 0,
	kParseInsituFlag = 1,
	kParseValidateEncodingFlag = 2,
	kParseIterativeFlag = 4,
	kParseStopWhenDoneFlag = 8,
	kParseFullPrecisionFlag = 16,
	kParseCommentsFlag = 32,
	kParseNumbersAsStringsFlag = 64,
	kParseTrailingCommasFlag = 128,
	kParseNanAndInfFlag = 256,
	kParseDefaultFlags = RAPIDJSON_PARSE_DEFAULT_FLAGS
};

// allocators.h
#include <rapidjson/allocators.h>
class CrtAllocator {
public:
	static const bool kNeedFree = true;
	void *Malloc(size_t size);
	void *Realloc(void *originalPtr, size_t originalSize, size_t newSize);
	static void Free(void *ptr);
};

template <typename BaseAllocator = CrtAllocator>
class MemoryPoolAllocator {
public:
	static const bool kNeedFree = false;
	MemoryPoolAllocator(size_t chunkSize = kDefaultChunkCapacity, BaseAllocator *baseAllocator = 0)
		: chunkHead_(0)
		, chunk_capacity_(chunkSize)
		, userBuffer_(0)
		, baseAllocator_(baseAllocator)
		, ownBaseAllocator_(0)
		{
		}

	MemoryPoolAllocator(void *buffer, size_t size, size_t chunkSize = kDefaultChunkCapacity, BaseAllocator *baseAllocator = 0)
		: chunkHead_(0)
		, chunk_capacity_(chunkSize)
		, userBuffer_(buffer)
		, baseAllocator_(baseAllocator)
		, ownBaseAllocator_(0)
		{
		}

	~MemoryPoolAllocator();
	void Clear();
	size_t Capacity() const;
	size_t Size() const;
	void *Malloc(size_t size);
	void *Realloc(void *originalPtr, size_t originalSize, size_t newSize);
	static void Free(void *ptr);
private:
	MemoryPoolAllocator(const MemoryPoolAllocator &rhs);
	MemoryPoolAllocator &operator=(const MemoryPoolAllocator &rhs);
	bool AddChunk(size_t capacity);
	static const int kDefaultChunkCapacity;
	struct ChunkHeader {
		size_t capacity;
		size_t size;
		ChunkHeader *next;
	};
	ChunkHeader *chunkHead_;
	size_t chunk_capacity_;
	void *userBuffer_;
	baseAllocator *baseAllocator_;
	baseAllocator *ownBaseAllocator_;
};

// memorystream.h
#include <rapidjson/memorystream.h>
struct MemoryStream {
	typedef char Ch;
	MemoryStream(const Ch *src, size_t size)
		: src_(src)
		, begin_(src)
		, end_(src + size)
		, size_(size)
		{}

	Ch Peek() const;
	Ch Take();
	size_t Tell() const;
	Ch *PutBegin();
	void Put(Ch);
	void Flush();
	size_t PutEnd(Ch *);
	const Ch *Peek4() const;

	const Ch *src_;
	const Ch *begin_;
	const Ch *end_;
	size_t size_;
};

// struct UTF8
#include <rapidjson/encodings.h>
template<typename CharType = char>
struct UTF8 {
	typedef CharType Ch;
	enum { supportUnicode = 1 };

	template <typename OutputStream>
	static void Encode(OutputStream &os, unsigned codepoint);

	template <typename OutputStream>
	static void EncodeUnsafe(OutputStream &os, unsigned codepoint);

	template <typename InputStream>
	static bool Decode(InputStream &is, unsigned *codepoint);

	template <typename InputStream, typename OutputStream>
	static bool Validate(InputStream &is, OutputStream &os);

	static unsigned char GetRange(unsigned char c);

	template <typename InputByteStream>
	static CharType TakeBOM(InputByteStream &is);

	template <typename InputByteStream>
	static Ch Take(InputByteStream &is);

	template <typename OutputByteStream>
	static void PutBOM(OutputByteStream &os);

	template <typename OutputByteStream>
	static void Put(OutputByteStream &os, Ch c);
};

// encodedstream.h
#include <rapidjson/encodedstream.h>
template <typename Encoding, typename InputByteStream>
class EncodedInputStream {
public:
	typedef typename Encoding::Ch Ch;

	EncodedInputStream(InputByteStream &is)
		: is_(is)
		{}

	Ch Peek() const;
	Ch Take();
	size_t Tell() const;
	void Put(Ch);
	void Flush();
	Ch *PutBegin();
	size_t PutEnd(Ch *);
private:
	EncodedInputStream(const EncodedInputStream &);
	EncodedInputStream &operator=(const EncodedInputStream &);

	InputByteStream &is_;
	Ch current_;
};

// Specialized for UTF8 MemoryStream.
template <>
class EncodedInputStream<UTF8<>, MemoryStream> {
public:
	typedef UTF8<>::Ch Ch;
	EncodedInputStream(MemoryStream &is)
		: is_(is)
		{}
	Ch Peek() const;
	Ch Take();
	size_t Tell() const;
	void Put(Ch) {}
	void Flush() {}
	Ch *PutBegin() { return 0; }
	size_t PutEnd(Ch *) { return 0; }
	
	MemoryStream &is_;
private:
	EncodedInputStream(const EncodedInputStream &);
	EncodedInputStream &operator=(const EncodedInputStream &);
};

template <typename Encoding, typename OutputByteStream>
class EncodedOutputStream {
public:
	typedef typename Encoding::Ch Ch;
	EncodedOutputStream(OutputByteStream &os, bool PutBOM = true)
		: os_(os)
		{}

	void Put(Ch ch);
	void Flush();
	Ch Peek() const;
	Ch Take();
	size_t Tell() const;
	Ch *PutBegin();
	size_t PutEnd(Ch *);
private:
	EncodedOutputStream(const EncodedOutputStream &);
	EncodedOutputStream &operator=(const EncodedOutputStream &);

	OutputByteStream &os_;
};

// stream.h
#include <rapidjson/stream.h>
template<typename Stream>
struct StreamTraits {			// StreamTraits
	enum { copyOptimization = 0 };
};

template<typename Stream>
inline void PutReserve(Stream &stream, size_t count);

template<typename Stream>
inline void PutUnsafe(Stream &stream, typename Stream::Ch c);

template<typename Stream, typename Ch>
inline void PutN(Stream &stream, Ch c, size_t n);

template <typename InputStream, typename Encoding = UTF8<> >
class GenericStreamWrapper {
public:
	typedef typename Encoding::Ch Ch;
	GenericStreamWrapper(InputStream &is)
		: is_(is)
		{}

	Ch Peek() const;
	Ch Take();
	size_t Tell();
	Ch *PutBegin();
	void Put(Ch ch);
	void Flush();
	size_t PutEnd(Ch *ch);
	const Ch *Peek4() const;
	UTFType GetType() const;
	bool HasBOM() const;
protected:
	InputStream &is_;
};

template <typename Encoding>
struct GenericStringStream {
	typedef typename Encoding::Ch Ch;
	GenericStringStream(const Ch *src)
		: src_(src)
		, head_(src)
		{}

	Ch Peek() const;
	Ch Take();
	size_t Tell() const;
	Ch *PutBegin();
	void Put(Ch);
	void Flush();
	size_t PutEnd(Ch *);

	const Ch *src_;
	const Ch *head_;
};

template <typename Encoding>
struct StreamTraits<GenericStringStream<Encoding> > {
	enum { copyOptimization = 1 };
};

template <typename Encoding>
struct GenericInsituStringStream {
	typedef typename Encoding::Ch Ch;

	GenericInsituStringStream(Ch *src)
		: src_(src)
		, dst_(0)
		, head_(src)
		{}

	Ch Peek();
	Ch Take();
	size_t Tell();
	void Put(Ch c);
	Ch *PutBegin();
	size_t PutEnd(Ch *begin);
	void Flush();
	Ch *Push(size_t count);
	void Pop(size_t count);

	Ch *src_;
	Ch *dst_;
	Ch *head_;
};

template <typename Encoding>
struct StreamTraits<GenericInsituStringStream<Encoding> > {
	enum { copyOptimization = 1 };
};

// reader.h
#include <rapidjson/reader.h>
template <typename Encoding = UTF8<>, typename Derived = void>
struct BaseReaderHandler {
	typedef typename Encoding::Ch Ch;
	typedef typename internal::SelectIf<internal::IsSame<Derived, void>, BaseReaderHandler, Derived>::Type Override;

	bool Default();
	bool Null();
	bool Bool(bool);
	bool Int(int);
	bool Uint(unsigned);
	bool Int64(int64_t);
	bool Uint64(uint64_t);
	bool Double(double);
	bool RawNumber(const Ch *str, SizeType len, bool copy);
	bool String(const Ch*, SizeType, bool);
	bool StartObject();
	bool Key(const Ch *str, SizeType len, bool copy);
	bool EndObject(SizeType);
	bool StartArray();
	bool EndArray(SizeType);
};

template <typename SourceEncoding, typename TargetEncoding, typename StackAllocator = CrtAllocator>
class GenericReader {
public:
	typedef typename SourceEncoding::Ch Ch;
	
    GenericReader(StackAllocator *stackAllocator = 0, size_t stackCapacity = kDefaultStackCapacity)
		: stack_(stackAllocator, stackCapacity)
		, parseResult_()
		, state_(IterativeParsingStartState)
		{}

	template <unsigned parseFlags, typename InputStream, typename Handler>
	ParseResult Parse(InputStream &is, Handler &handler);

	template <typename InputStream, typename Handler>
	ParseResult Parse(InputStream &is, Handler &handler);

	void IterativeParseInit();

	template <unsigned parseFlags, typename InputStream, typename Handler>
	bool IterativeParseNext(InputStream &is, Handler &handler);

	bool IterativeParseComplete() const;

	bool HasParseError() const;
	ParseErrorCode GetParseErrorCode() const;
	size_t GetErrorOffset() const;
};

// document.h
template <typename Encoding, typename Allocator>
struct GenericMember {
	GenericValue<Encoding, Allocator> name; // name of member
	GenericValue<Encoding, Allocator> value; // value of member
};

template <typename CharType>
struct GenericStringRef {
	typedef CharType Ch;

	template<SizeType N>
	GenericStringRef(const CharType (&str)[N]);
	explicit GenericStringRef(const CharType *str);
	GenericStringRef(const CharType *str, SizeType len);
	GenericStringRef(const GenericStringRef &rhs);
	operator const Ch *() const { return s; }

	const Ch *const s;
	const SizeType length;	
};

// Forward declarations
template <bool, typename> class GenericArray;
template <bool, typename> class GenericObject;

templat <typename Encoding, typename Allocator = MemoryPoolAllocator<> >
class GenericValue {
public:
	typedef GenericMember<Encoding, Allocator> Member;
	typedef Encoding EncodingType;
	typedef Allocator AllocatorType;
	typedef typename Encoding::Ch Ch;
	typedef GenericStringRef<Ch> StringRefType;
	typedef typename GenericMemberIterator<false,Encoding,Allocator>::Iterator MemberIterator;
	typedef typename GenericMemberIterator<true,Encoding,Allocator>::Iterator ConstMemberIterator;
	typedef GenericValue *ValueIterator;
	typedef const GenericValue *ConstValueIterator;
	typedef GenericValue<Encoding, Allocator> ValueType;
	typedef GenericArray<false, ValueType> Array;
	typedef GenericArray<true, ValueType> ConstArray;
	typedef GenericObject<false, ValueType> Object;
	typedef GenericObject<true, ValueType> ConstObject;

	GenericValue() : data_() { data_.f.flags = kNullFlag; }
	explicit GenericValue(Type type);

	template <typename SourceAllocator>
	GenericValue(const GenericValue<Encoding,SourceAllocator>& rhs, Allocator &allocator, bool copyConstStrings = false);

	explicit GenericValue(int i);
	explicit GenericValue(unsigned u);
	explicit GenericValue(int64_t i64);
	explicit GenericValue(uint64_t u64);
	explicit GenericValue(double d);
	explicit GenericValue(float f);
	GenericValue(const Ch *s, SizeType length);
	explicit GenericValue(StringRefType s);
	GenericValue(const Ch *s, SizeType length, Allocator &allocator);
	GenericValue(const Ch*s, Allocator& allocator);
	GenericValue(const std::basic_string<Ch> &s, Allocator &allocator);
	GenericValue(Array a);
	GenericValue(Object o);
	~GenericValue();
	GenericValue &operator=(GenericValue &rhs);
	GenericValue &operator=(StringRefType str);

	template <typename SourceAllocator>
	GenericValue &CopyFrom(const GenericValue<Encoding, SourceAllocator> &rhs, Allocator &allocator, bool copyConstStrings = false);
	GenericValue &Swap(GenericValue &other);
	// friend function
	friend inline swap(GenericValue &a, GenericValue &b);

	GenericValue &Move();
	template<typename SourceAllocator>
	bool operator==(const GenericValue<Encoding, SourceAllocator> &rhs) const;
	bool operator==(const Ch *rhs) const;
	bool operator==(const std::basic_string<Ch> &rhs) const;

	template<typename SourceAllocator>
	bool operator!=(const GenericValue<Encoding, SourceAllocator> &rhs) const;
	bool operator!=(const Ch *rhs) const;

	Type GetType() const;
	bool IsNull() const;
	bool IsFalse() const;
	bool IsTrue() const;
	bool IsBool() const;
	bool IsObject() const;
	bool IsArray() const;
	bool IsNumber() const;
	bool IsInt() const;
	bool IsUint() const;
	bool IsInt64() const;
	bool IsUint64() const;
	bool IsDouble() const;
	bool IsString() const;
	bool IsLosslessDouble() const;
	bool IsFloat() const;
	bool IsLosslessFloat() const;
	GenericValue &SetNull();
	bool GetBool() const;
	GenericValue &SetBool(bool b);
	GenericValue &SetObject();
	SizeType MemberCount() const;
	SizeType MemberCapacity() const;
	bool ObjectEmpty() const;

	template<typename SourceAllocator>
	GenericValue &operator[](const GenericValue<Encoding,SourceAllocator> &name);
	template<typename SourceAllocator>
	const GenericValue &operator[](const GenericValue<Encoding, SourceAllocator> &name) const;
	GenericValue &operator[](const std::basic_string<Ch> &name);
	const GenericValue &operator[](const std::basic_string<Ch> &name) const;
	
	ConstMemberIterator MemberBegin() const;
	ConstMemberIterator MemberEnd() const;
	MemberIterator MemberBegin();
	MemberIterator MemberEnd();
	GenericValue &MemberReserve(SizeType newCapacity, Allocator &allocator);
	bool HasMember(const Ch *name) const;
	bool HasMember(const std::basic_string<Ch> &name) const;

	template <typename SourceAllocator>
	bool HasMember(const GenericValue<Encoding, SourceAllocator> &name) const;
	MemberIterator FindMember(const Ch *name);
	ConstMemberIterator FindMember(const Ch *name) const;

	template <typename SourceAllocator>
	MemberIterator FindMember(const GenericValue<Encoding, SourceAllocator> &name);

	template <typename SourceAllocator>
	ConstMemberIterator FindMember(const GenericValue<Encoding, SourceAllocator> &name) const;
	MemberIterator FindMember(const std::basic_string<Ch> &name);
	ConstMemberIterator FindMember(const std::basic_string<Ch> &name) const;
	
	GenericValue &AddMember(GenericValue &name, GenericValue &value, Allocator &allocator);
	GenericValue &AddMember(GenericValue &name, StringRefType value, Allocator &allocator);
	GenericValue &AddMember(GenericValue &name, std::basic_string<Ch> &value, Allocator &allocator);
	template <typename T>
	GenericValue &AddMember(GenericValue &name, T value, Allocator &allocator);
	GenericValue &AddMember(StringRefType name, GenericValue &value, Allocator &allocator);
	GenericValue &AddMember(StringRefType name, StringRefType value, Allocator &allocator);
	template <typename T>
	GenericValue &AddMember(StringRefType name, T value, Allocator &allocator);
	void RemoveAllMembers();
	bool RemoveMember(const Ch *name);
	bool RemoveMember(const std::basic_string<Ch> &name);

	template <typename SourceAllocator>
	bool RemoveMember(const GenericValue<Encoding,SourceAllocator> &name);

	MemberIterator RemoveMember(MemberIterator m);
	MemberIterator EraseMember(ConstMemberIterator pos);
	MemberIterator EraseMember(ConstMemberIterator first, ConstMemberIterator last);
	bool EraseMember(const Ch *name);
	bool EraseMember(const std::basic_string<Ch> &name);

	template <typename SourceAllocator>
	bool EraseMember(const GenericValue<Encoding,SourceAllocator> &name);

	Object GetObject();
	ConstObject GetObject() const;

	GenericValue &SetArray();
	SizeType Size() const;
	SizeType Capacity() const;
	bool Empty() const;
	void Clear();

	GenericValue &operator[](SizeType index);
	const GenericValue &operator[](SizeType index) const;
	ValueIterator Begin();
	ValueIterator End();
	ConstValueIterator Begin() const;
	ConstValueIterator End() const;
	GenericValue &Reserve(SizeType newCapacity, Allocator &allocator);
	GenericValue &PushBack(GenericValue &value, Allocator &allocator);
	GenericValue &PushBack(StringRefType value, Allocator &allocator);

	template <typename T>
	GenericValue &PushBack(T value, Allocator &allocator);

	GenericValue &PopBack();
	ValueIterator Erase(ConstValueIterator pos);
	ValueIterator Erase(ConstValueIterator first, ConstValueIterator last);
	Array GetArray();
	ConstArray GetArray() const;

	int GetInt() const;
	unsigned GetUint() const;
	int64_t GetInt64() const;
	uint64_t GetUint64() const;
	double GetDouble() const;
	float GetFloat() const;
	GenericValue &SetInt(int i);
	GenericValue &SetUint(unsigned u);
	GenericValue &SetInt64(int64_t i64);
	GenericValue &SetUint64(uint64_t u64);
	GenericValue &SetDouble(double d);
	GenericValue &SetFloat(float f);

	const Ch *GetString() const;
	SizeType GetStringLength() const;
	GenericValue &SetString(const Ch *s, SizeType length);
	GenericValue &SetString(StringRefType s);
	GenericValue &SetString(const Ch* s, SizeType length, Allocator &allocator);
	GenericValue &SetString(const Ch* s, Allocator &allocator);
	GenericValue &SetString(StringRefType s, Allocator &allocator);
	GenericValue &SetString(const std::basic_string<Ch> &s, Allocator &allocator);

	template <typename T>
	bool Is() const;

	template <typename T>
	T Get() const;

	template <typename T>
	T Get();

	template <typename T>
	ValueType &Set(const T& data);

	template <typename T>
	ValueType &Set(const T &data, AllocatorType &allocator);

	template <typename Handler>
	bool Accept(Handler &handler) const;
private:
	template <typename, typename> friend class GenericValue;
	template <typename, typename, typename> friend class GenericDocument;

	GenericValue(const GenericValue &rhs);
};

template <typename Encoding, typename Allocator = MemoryPoolAllocator<>, typename StackAllocator = CrtAllocator>
class GenericDocument : public GenericValue<Encoding, Allocator> {
public:
	typedef typename Encoding::Ch Ch;
	typedef GenericValue<Encoding, Allocator> ValueType;
	typedef Allocator AllocatorType;
	
    explicit GenericDocument(Type type, Allocator *allocator = 0, size_t stackCapacity = kDefaultStackCapacity, StackAllocator *stackAllocator = 0);
	GenericDocument(Allocator *allocator = 0, size_t stackCapacity = kDefaultStackCapacity, StackAllocator *StackAllocator = 0);
	~GenericDocument();

	GenericDocument &Swap(GenericDocument &rhs);

	using ValueType::Swap;

	friend inline void swap(GenericDocument &a, GenericDocument &b);

	template<typename Generator>
	GenericDocument &Populate(Generator &g);

	template<unsigned parseFlags, typename SourceEncoding, typename InputStream>
	GenericDocument &ParseStream(InputStream &is);

	template <unsigned parseFlags, typename InputStream>
	GenericDocument &ParseStream(InputStream &is);

	template <typename InputStream>
	GenericDocument &ParseStream(InputStream &is);

	template <unsigned parseFlags>
	GenericDocument &ParseInsitu(Ch *str);
	GenericDocument &ParseInsitu(Ch *str);

	template <unsigned parseFlags, typename SourceEncoding>
	GenericDocument &Parse(const typename SourceEncoding::Ch *str);

	template <unsigned parseFlags>
	GenericDocument &Parse(const Ch *str);
	GenericDocument &Parse(const Ch *str);

	template <unsigned parseFlags, typename SourceEncoding>
	GenericDocument &Parse(const typename SourceEncoding::Ch *str, size_t length);

	template <unsigned parseFlags>
	GenericDocument &Parse(const Ch *str, size_t length);
	GenericDocument &Parse(const Ch *str, size_t length);

	template <unsigned parseFlags, typename SourceEncoding>
	GenericDocument &Parse(const std::basic_string<typename SourceEncoding::Ch> &str);

	template <unsigned parseFlags>
	GenericDocument &Parse(const std::basic_string<Ch> &str);
	GenericDocument &Parse(const std::basic_string<Ch> &str);

	bool HasParseError() const;
	ParseErrorCode GetParseError() const;
	size_t GetErrorOffset() const;
	operator ParseResult() const;

	Allocator &GetAllocator();
	size_t GetStackCapacity() const;

public:
	bool Null();
	bool Bool(bool b);
	bool Int(int i);
	bool Uint(unsigned i);
	bool Int64(int64_t i);
	bool Uint64(uint64_t i);
	bool Double(double d);
	bool RawNumber(const Ch *str, SizeType length, bool copy);
	bool String(const Ch *str, SizeType length, bool copy);
	bool StartObject();
	bool Key(const Ch *str, SizeType length, bool copy);
	bool EndObject(SizeType memberCount);
	bool StartArray();
	bool EndArray(SizeType elementCount);
};

// Helper class for accessing Value of array type.
template <bool Const, typename ValueT>
class GenericArray
{
public:
	typedef GenericArray<true, ValueT> ConstArray;
	typedef GenericArray<false, ValueT> Array;
	typedef ValueT PlainType;
	typedef typename internal::MaybeAddConst<Const,PlainType>::Type ValueType;
	typedef ValueType *ValueIterator;
	typedef const ValueT* ConstValueIterator;
	typedef typename ValueType::AllocatorType AllocatorType;
	typedef typename ValueType::StringRefType StringRefType;

	template <typename, typename>
	friend class GenericValue;
	
    GenericArray(const GenericArray &rhs);
	GenericArray &operator=(const GenericArray &rhs);
    ~GenericArray() {}

	SizeType Size() const;
	SizeType Capacity() const;
	bool Empty() const;
	void Clear() const;
	ValueType &operator[](SizeType index) const;
	ValueIterator Begin() const;
	ValueIterator End() const;
	GenericArray Reserve(SizeType newCapacity, AllocatorType &allocator) const;
	GenericArray PushBack(ValueType &value, AllocatorType &allocator) const;
	GenericArray PushBack(StringRefType value, AllocatorType &allocator) const;
	GenericArray PopBack() const;
	ValueIterator Erase(ConstValueIterator pos) const;
	ValueIterator Erase(ConstValueIterator first, ConstValueIterator last) const;
private:
	GenericArray();
	GenericArray(ValueType &value);
	ValueType &value_;
};

template <bool Const, typename ValueT>
class GenericObject {
public:
	typedef GenericObject<true, ValueT> ConstObject;
	typedef GenericObject<false, ValueT> Object;
	typedef ValueT PlainType;
	typedef typename internal::MaybeAddConst<Const,PlainType>::Type ValueType;
	typedef GenericMemberIterator<Const,typename ValueT::EncodingType,typename ValueT::AllocatorType> MemberIterator;
	typedef GenericMemberIterator<true,typename ValueT::EncodingType,typename ValueT::AllocatorType> ConstMemberIterator;
	typedef typename ValueType::AllocatorType AllocatorType;
	typedef typename ValueType::StringRefType StringRefType;
	typedef typename ValueType::EncodingType EncodingType;
	typedef typename ValueType::Ch Ch;

	template <typename, typename>
	friend class GenericValue;

	GenericObject(const GenericObject &rhs);
	GenericObject &operator=(const GenericObject &rhs);
	~GenericObject() {}

	SizeType MemberCount() const;
	bool ObjectEmpty() const;

	template <typename T>
	ValueType &operator[](T *name) const;

	template <typename SourceAllocator>
	ValueType &operator[](const GenericValue<EncodingType,SourceAllocator> &name) const;

	ValueType &operator[](const std::basic_string<Ch> &name) const;
	MemberIterator MemberBegin() const;
	MemberIterator MemberEnd() const;
	bool HasMember(const Ch *name) const;
	bool HasMember(const std::basic_string<Ch> &name) const;

	template <typename SourceAllocator>
	bool HasMember(const GenericValue<EncodingType,SourceAllocator> &name) const;

	MemberIterator FindMember(const Ch *name) const;

	template <typename SourceAllocator>
	MemberIterator FindMember(const GenericValue<EncodingType,SourceAllocator> &name) const;

	MemberIterator FindMember(const std::basic_string<Ch> &name) const;
	GenericObject AddMember(ValueType &name, ValueType &value, AllocatorType &allocator) const;
	GenericObject AddMember(ValueType &name, StringRefType value, AllocatorType &allocator) const;
	GenericObject AddMember(ValueType &name, std::basic_string<Ch> &value, AllocatorType &allocator) const;
	GenericObject AddMember(StringRefType name, ValueType &value, AllocatorType &allocator) const;
	void RemoveAllMembers();
	bool RemoveMember(const Ch *name) const;
	bool RemoveMember(const std::basic_string<Ch> &name) const;

	template <typename SourceAllocator>
	bool RemoveMember(const GenericValue<EncodingType,SourceAllocator> &name) const;
	MemberIterator RemoveMember(MemberIterator m) const;
	MemberIterator EraseMember(ConstMemberIterator pos) const;
	MemberIterator EraseMember(ConstMemberIterator first, ConstMemberIterator last) const;
	bool EraseMember(const Ch *name) const;
	bool EraseMember(const std::basic_string<Ch> &name) const;

	template <typename SourceAllocator>
	bool EraseMember(const GenericValue<EncodingType,SourceAllocator> &name) const;
private:
	GenericObject();
	GenericObject(ValueType &value);
	ValueType &value_;
};

// stringbuffer.h
#include <rapidjson/stringbuffer.h>
template <typename Encoding, typename Allocator = CrtAllocator>
class GenericStringBuffer {
public:
	typedef typename Encoding::Ch Ch;

	GenericStringBuffer(Allocator *allocator = 0, size_t capacity = kDefaultCapacity)
		: stack_(allocator, capacity) {}

	void Put(Ch c);
	void PutUnsafe(Ch c);
	void Flush() {}
	void Clear();
	void ShrinkToFit();
	void Reserve(size_t count);
	Ch *Push(size_t count)	;
	Ch *PushUnsafe(size_t count);
	void Pop(size_t count);
	const Ch *GetString() const;
	size_t GetSize() const;

	static const size_t kDefaultCapacity = 256;
	mutable internal::Stack<Allocator> stack_;
private:
	GenericStringBuffer(const GenericStringBuffer &);
	GenericStringBuffer &operator=(const GenericStringBuffer &);
};

enum PrettyFormatOptions {
	kFormatDefault = 0,
	kFormatSingleLineArray = 1
};

#ifndef RAPIDJSON_WRITE_DEFAULT_FLAGS
#define RAPIDJSON_WRITE_DEFAULT_FLAGS kWriteNoFlags
#endif

enum WriteFlag {
	kWriteNoFlags = 0,
	kWriteValidateEncodingFlag = 1,
	kWriteNanAndInfFlag = 2,
	kWriteDefaultFlags = RAPIDJSON_WRITE_DEFAULT_FLAGS
};

// JSON writer
template<typename OutputStream, typename SourceEncoding = UTF8<>, typename TargetEncoding = UTF8<>, typename StackAllocator = CrtAllocator, unsigned writeFlags = kWriteDefaultFlags>
class Writer {
public:
	typedef typename SourceEncoding::Ch Ch;
	static const int kDefaultMaxDecimalPlaces = 324;
	
    explicit Writer(OutputStream &os, StackAllocator *stackAllocator = 0, size_t levelDepth = kDefaultLevelDepth);

	explicit Writer(StackAllocator *allocator = 0, size_t levelDepth = kDefaultLevelDepth);

	void Reset(OutputStream &os);
	bool IsComplete() const;
	int GetMaxDecimalPlaces() const;
	void SetMaxDecimalPlaces(int maxDecimalPlaces);
	bool Null();
	bool Bool(bool b);
	bool Int(int i);
	bool Uint(unsigned u);
	bool Int64(int64_t i64);
	bool Uint64(uint64_t u64);
	bool Double(double d);
	bool RawNumber(const Ch *str, SizeType length, bool copy = false);
	bool String(const Ch *str, SizeType length, bool copy = false);
	bool String(const std::basic_string<Ch> &str);
	bool StartObject();
	bool Key(const Ch *str, SizeType length, bool copy = false);
	bool EndObject(SizeType memberCount = 0);
	bool StartArray();
	bool EndArray(SizeType elementCount = 0);
	bool String(const Ch *str);
	bool Key(const Ch *str);
	bool RawValue(const Ch *json, size_t length, Type type);
protected:
	OutputStream *os_;
};


template <typename OutputStream, typename SourceEncoding = UTF8<>, typename TargetEncoding = UTF8<>, typename StackAllocator = CrtAllocator, unsigned writeFlags = kWriteDefaultFlags>
class PrettyWriter : public Writer<OutputStream, SourceEncoding, TargetEncoding, StackAllocator, writeFlags> {
public:
	typedef Writer<OutputStream, SourceEncoding, TargetEncoding, StackAllocator, writeFlags> Base;
	typedef typename Base::Ch Ch;

	explicit PrettyWriter(OutputStream &os, StackAllocator *allocator = 0, size_t levelDepth = Base::kDefaultLevelDepth);
	explicit PrettyWriter(StackAllocator *allocator = 0, size_t levelDepth = Base::kDefaultLevelDepth);

	PrettyWriter &SetIndent(Ch indentChar, unsigned indentCharCount);
	PrettyWriter &SetFormatOptions(PrettyFormatOptions options);
	bool Null();
	bool Bool(bool b);
	bool Int(int i);
	bool Uint(unsigned u);
	bool Int64(int64_t i64);
	bool Uint64(uint64_t u64);
	bool Double(double d);
	bool RawNumber(const Ch *str, SizeType length, bool copy = false);
	bool String(const Ch *str, SizeType length, bool copy = false);
	bool String(const std::basic_string<Ch> &str);
	bool StartObject();
	bool Key(const Ch *str, SizeType length, bool copy = false);
	bool Key(const std::basic_string<Ch> &str);
	bool EndObject(SizeType memberCount = 0);
	bool StartArray();
	bool EndArray(SizeType memberCount = 0);
	bool String(const Ch *str);
	bool Key(const Ch *str);
	bool RawValue(const Ch *json, size_t length, Type type);
private:
	PrettyWriter(const PrettyWriter &);
	PrettyWriter &operator=(const PrettyWriter &);
};

typedef GenericReader<UTF8<>, UTF8<> > Reader;
typedef GenericStringStream<UTF8<> > StringStream;
typedef GenericValue<UTF8<> > Value;
typedef GenericDocument<UTF8<> > Document;
typedef GenericStringBuffer<UTF8<> > StringBuffer;
typedef GenericInsituStringStream<UTF8<> > InsituStringStream;

RAPIDJSON_NAMESPACE_END

#endif /* _RAPIDJSON_MAN_H_ */
